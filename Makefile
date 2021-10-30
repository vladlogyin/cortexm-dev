# Figure out what OS we're running on
ifeq ($(OS),Windows_NT)
  HOST= Windows
else
# uname returns the name of the kernel (eg Linux, Darwin, FreeBSD etc)
  HOST= $(shell uname -s)
  
endif

# Change this to a source file containing main()
PROJECT_SRC:= main.cpp
PROJECT:= $(basename $(PROJECT_SRC))
PROJECT_OBJ:= $(PROJECT).o
PROJECT_ELF:= $(PROJECT).elf
PROJECT_BIN:= $(PROJECT).bin

SOURCES_CPP= lib/ringbuffer/ringbuffer.cpp
SOURCES_CPP+= drivers/lm75/lm75.cpp drivers/bme280/bme280.cpp drivers/rc522/rc522.cpp drivers/ds3231/ds3231.cpp drivers/max7219/max7219.cpp drivers/ili9341/ili9341.cpp drivers/cdcacm/cdcacm.cpp
SOURCES_CPP+= lib/systemutils.cpp lib/ringbuffer/ringbuffer.cpp
SOURCES_C= tinyprintf/tinyprintf.c

OBJECTS:= $(patsubst %.cpp, %.o, $(SOURCES_CPP)) $(patsubst %.c, %.o, $(SOURCES_C))
DEPENDS:= $(patsubst %.cpp,%.d,$(SOURCES_CPP)) $(patsubst %.c, %.d, $(SOURCES_C)) $(PROJECT).d
# setup
INCLUDE_DIRS= -I. -I./drivers -I./libopencm3/include
LIBRARY_DIRS= -L./libopencm3/lib
LIBRARIES= -lgcc -lm -lopencm3_stm32f1 -lstdc++

# Select c++17 and turn optimizations on
COMPILE_OPTIONS= -fno-exceptions -fno-non-call-exceptions -fno-common -ffunction-sections -fdata-sections -flto -std=c++17 -O2 -fno-rtti -finline-small-functions -findirect-inlining
# Include debugging symbols and and turn off optimizations
#COMPILE_OPTIONS+= -g3 -O0

CROSS= arm-none-eabi-

CC= $(CROSS)gcc
CXX= $(CROSS)g++
AS= $(CROSS)as
LD= $(CROSS)g++
OBJCP= $(CROSS)objcopy
AR= $(CROSS)ar
SIZE= $(CROSS)size

# Define MCU specific flags here
# Example for an STM32F103 MCU with a cortex m3 core
# -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
ARCHITECTURE_FLAGS= -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1

CXXFLAGS= $(COMPILE_OPTIONS) $(ARCHITECTURE_FLAGS) $(INCLUDE_DIRS)
CFLAGS= $(COMPILE_OPTIONS) $(ARCHITECTURE_FLAGS) $(INCLUDE_DIRS)
ASFLAGS= $(COMPILE_OPTIONS) -c

LDFLAGS= -mthumb -nostartfiles --static --specs=nosys.specs --specs=nano.specs
# Select linker script
LDFLAGS+= -T toolchain/ldscripts/stm32f103.ld
# Add linker optimizations
LDFLAGS+= $(CXXFLAGS)
# Add libraries
LDFLAGS+= $(LIBRARY_DIRS) $(LIBRARIES)
OBJCPFLAGS= -O binary
ARFLAGS= cr



# all

all: build

install: flash

# main
# Include header dependencies
-include $(DEPENDS)
# Recompile objects when Makefile changes
%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(PROJECT_OBJ) $(OBJECTS): libopencm3/lib/libopencm3_stm32f1.a

$(PROJECT_ELF): $(PROJECT_OBJ) $(OBJECTS) libopencm3/lib/libopencm3_stm32f1.a
	$(LD) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(PROJECT_BIN): $(PROJECT_ELF)
	$(OBJCP) $(OBJCPFLAGS) $< $@


# flash
build: $(PROJECT_BIN)

flash: build
	-pkill openocd -9
	-rm target.bin
	ln -sr $(PROJECT_BIN) target.bin
	openocd -f toolchain/openocd/stm32f1_flash.cfg
debug: flash
	-pkill openocd -9
	openocd -f toolchain/openocd/stm32f1_debug.cfg &
	gdb -iex "target extended-remote :3333" $(PROJECT_ELF)
# libopencm3

libopencm3/Makefile:
	@echo "Initializing libopencm3 submodule"
	git submodule update --init
newlib/Makefile:
	@echo "Initializing newlib submodule"
	git submodule update --init

libopencm3/lib/libopencm3_%.a: libopencm3/Makefile
	$(MAKE) -C libopencm3

clean:
	-rm $(PROJECT_ELF) $(PROJECT_BIN) $(PROJECT_OBJ) *.bin *.elf *.elf.map *.o $(OBJECTS) $(DEPENDS)

checktoolchain:
	@sh toolchain/checktoolchain.sh $(HOST)

.PHONY: all clean build install flash debug checktoolchain

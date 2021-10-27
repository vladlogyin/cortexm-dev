# Figure out what OS we're running on
ifeq ($(OS),Windows_NT)
  HOST= Windows
else
# uname returns the name of the kernel (eg Linux, Darwin, FreeBSD etc)
  HOST= $(shell uname -s)
  
endif



SOURCES_CPP= lm75/lm75.cpp bme280/bme280.cpp rc522/rc522.cpp ds3231/ds3231.cpp max7219/max7219.cpp main.cpp systems.cpp ili9341/ili9341.cpp
SOURCES_C= printf/printf.c
HEADERS= lm75/lm75.h bme280/bme280.h rc522/rc522.h ds3231/ds3231.h max7219/max7219.h main.h systems.h printf/printf.h ili9341/ili9341.h
OBJECTS:= $(patsubst %.cpp, %.o, $(SOURCES_CPP)) $(patsubst %.c, %.o, $(SOURCES_C))
DEPENDS:= $(patsubst %.cpp,%.d,$(SOURCES_CPP)) $(patsubst %.c, %.d, $(SOURCES_C))
# setup
INCLUDE_DIRS= -I. -I./printf -I./libopencm3/include
LIBRARY_DIRS= -L./libopencm3/lib
LIBRARIES= -lgcc -lm -lopencm3_stm32f1 -lnosys

# Select c++17 and turn optimizations on
COMPILE_OPTIONS= -fno-exceptions -fno-common -flto -std=c++17 -O2
# Include debugging symbols and and turn off optimizations
#COMPILE_OPTIONS+= -g3 -O0

CROSS= arm-none-eabi-

CC= $(CROSS)gcc
CXX= $(CROSS)g++
AS= $(CROSS)as
LD= $(CROSS)gcc
OBJCP= $(CROSS)objcopy
AR= $(CROSS)ar
SIZE= $(CROSS)size

# Define MCU specific flags here
# Example for an STM32F1 with a cortex m3
# -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
ARCHITECTURE_FLAGS= -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1

CXXFLAGS= $(COMPILE_OPTIONS) $(ARCHITECTURE_FLAGS) $(INCLUDE_DIRS)
CFLAGS= $(COMPILE_OPTIONS) $(ARCHITECTURE_FLAGS) $(INCLUDE_DIRS)
ASFLAGS= $(COMPILE_OPTIONS) -c

LDFLAGS= -flto -mthumb -nostartfiles --static --specs=nano.specs --specs=nosys.specs
# Select linker script
LDFLAGS+= -T stm32basic.ld
# Add linker optimizations
LDFLAGS+= -ffunction-sections -fdata-sections
# Add libraries
LDFLAGS+= $(LIBRARY_DIRS) $(LIBRARIES)
OBJCPFLAGS= -O binary
ARFLAGS= cr

# Output binary name
MAIN_OUT:= main
MAIN_OUT_ELF:= $(MAIN_OUT).elf
MAIN_OUT_BIN:= $(MAIN_OUT).bin

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

$(MAIN_OUT_ELF): $(OBJECTS) libopencm3/lib/libopencm3_stm32f1.a
	$(LD) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(MAIN_OUT_BIN): $(MAIN_OUT_ELF)
	$(OBJCP) $(OBJCPFLAGS) $< $@


# flash
build: $(MAIN_OUT_BIN)

flash: build
	openocd -f flash/stm32f1_flash.cfg
debug: flash
	openocd -f flash/stm32f1_debug.cfg

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
	-rm $(MAIN_OUT_ELF) $(MAIN_OUT_BIN) $(OBJECTS) $(DEPENDS)

checktoolchain:
	@sh utils/checktoolchain.sh $(HOST)

.PHONY: all clean build install flash debug checktoolchain

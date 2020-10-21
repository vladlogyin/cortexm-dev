SOURCEFILES= bme280/bme280.cpp rc522/rc522.cpp ds3231/ds3231.cpp max7219/max7219.cpp main.cpp systems.cpp printf/printf.c ili9341/ili9341.cpp
HEADERFILES= bme280/bme280.h rc522/rc522.h ds3231/ds3231.h max7219/max7219.h main.h systems.h printf/printf.h ili9341/ili9341.h
# setup
SFLAGS= --static -nostartfiles -std=c++11 -g3 -O0
SFLAGS+= -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I. -I./printf -I./libopencm3/include -L./libopencm3/lib 
LFLAGS+=-Wl,--start-group -lgcc -lm --specs=nano.specs --specs=nosys.specs -Wl,--end-group
M3_FLAGS= $(SFLAGS) -mcpu=cortex-m3 -mthumb -msoft-float
LFLAGS_STM32=$(LFLAGS) $(SOURCEFILES) -T stm32basic.ld
STM32F1_CFLAGS=$(M3_FLAGS) -DSTM32F1 -DLITTLE_BIT=200000 $(LFLAGS_STM32) -lopencm3_stm32f1

CC = arm-none-eabi-gcc
#CFLAGS = $(COMPILE_OPTS) $(INCLUDE_DIRS)

CXX = arm-none-eabi-g++
CXXFLAGS = $(COMPILE_OPTS) $(INCLUDE_DIRS)

AS = arm-none-eabi-gcc
ASFLAGS = $(COMPILE_OPTS) -c

LD = arm-none-eabi-gcc
#LDFLAGS = -Wl,--gc-sections,-Map=$@.map,-cref,-u,Reset_Handler $(INCLUDE_DIRS) $(LIBRARY_DIRS) -T stm32basic.ld

OBJCP = arm-none-eabi-objcopy
OBJCPFLAGS = -O binary

AR = arm-none-eabi-ar
ARFLAGS = cr

MAIN_OUT = main
MAIN_OUT_ELF = $(MAIN_OUT).elf
MAIN_OUT_BIN = $(MAIN_OUT).bin

# all

all: $(MAIN_OUT_ELF) $(MAIN_OUT_BIN)

# main

$(MAIN_OUT_ELF): clean $(SOURCEFILES) $(HEADERFILES) libopencm3/lib/libopencm3_stm32f1.a
	$(LD) $(STM32F1_CFLAGS) -o $@

$(MAIN_OUT_BIN): $(MAIN_OUT_ELF)
	$(OBJCP) $(OBJCPFLAGS) $< $@


# flash
build: $(MAIN_OUT_BIN)

flash: build
	openocd -f flash/flash.cfg
debug: flash
	openocd -f flash/debug.cfg

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
	-rm *.o $(LIBSTM32_OUT) $(MAIN_OUT_ELF) $(MAIN_OUT_BIN)

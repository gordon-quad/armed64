#TARGET ?= master

OBJS = clock.o debug.o led.o matrix.o

ifeq (${TARGET},slave)
	BINARY = armed64-slave
	CFLAGS += -DSLAVE
	OBJS += i2c-slave.o
else
	BINARY = armed64
	CFLAGS += -DMASTER
	OBJS += usb.o keyboard.o i2c.o keymap.o mouse.o extrakey.o serial.o elog.o ring.o command.o
endif

OPENCM3_DIR = libopencm3
STLINK_PORT = 4242
OPENOCD_PORT = 3333
LDSCRIPT = stm32f103x8.ld
include libopencm3.stm32f1.mk

flash: libopencm3 flash_dfu

debug: debug_stlink

libopencm3:
	git submodule init
	git submodule update

size: $(BINARY).elf
	$(Q)./checksize $(LDSCRIPT) $(BINARY).elf

flash_stlink: $(BINARY).bin
	st-flash write $(BINARY).bin 0x8000000

debug_stlink: .gdb_config_stlink
	st-util -p $(STLINK_PORT) &
	$(GDB) --command=.gdb_config_stlink
	pkill -9 st-util

flash_dfu: $(BINARY).bin
	dfu-util -a1 -d 1EAF:0003 -D $(BINARY).bin -R

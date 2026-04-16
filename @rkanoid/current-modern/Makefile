ifeq ($(DEVKITARM),)
TOOLROOT := ../.tools/arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi/bin
else
TOOLROOT := $(DEVKITARM)/bin
endif

CC := $(TOOLROOT)/arm-none-eabi-gcc
OBJCOPY := $(TOOLROOT)/arm-none-eabi-objcopy

TARGET := rkanoid-modern
OBJS := start_gnu.o wait_vbl_done.o data_gnu.o demo.o juego.o niveles.o

CPPFLAGS := -I.
CFLAGS := -marm -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -std=gnu89 -Wall
ASFLAGS := -marm -mcpu=arm7tdmi
LDFLAGS := -T linker.ld -nostartfiles -Wl,-Map,$(TARGET).map -Wl,--gc-sections -specs=nosys.specs

all: $(TARGET).bin

assets:
	python3 tools/rebuild_assets.py

start_gnu.o: start_gnu.s
	$(CC) $(ASFLAGS) -c $< -o $@

wait_vbl_done.o: wait_vbl_done.s
	$(CC) $(ASFLAGS) -c $< -o $@

data_gnu.o: data_gnu.S
	$(CC) $(ASFLAGS) -c $< -o $@

demo.o: demo.c assets.h gameplay.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c demo.c -o $@

juego.o: juego.c gameplay.h levels.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c juego.c -o $@

niveles.o: niveles.c levels.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c niveles.c -o $@

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

publish: $(TARGET).bin
	/bin/cp -f $(TARGET).bin ../../public/roms/@rkanoid\ MODERN\ -\ RECOMPILED.gba

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all assets publish clean

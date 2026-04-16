ifeq ($(DEVKITARM),)
TOOLROOT := ../.tools/arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi/bin
else
TOOLROOT := $(DEVKITARM)/bin
endif

CC := $(TOOLROOT)/arm-none-eabi-gcc
OBJCOPY := $(TOOLROOT)/arm-none-eabi-objcopy

TARGET := rkanoid-modern
OBJS := start_gnu.o wait_vbl_done.o data_gnu.o app_flow.o menu.o screens.o game_sprites.o game_loop.o levels_data.o levels_runtime.o

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

app_flow.o: app_flow.c gameplay.h game_sprites.h menu.h regs.h screens.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c app_flow.c -o $@

menu.o: menu.c menu.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c menu.c -o $@

screens.o: screens.c assets.h regs.h screens.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c screens.c -o $@

game_sprites.o: game_sprites.c game_sprites.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c game_sprites.c -o $@

game_loop.o: game_loop.c gameplay.h game_sprites.h game_state.h levels_data.h levels_runtime.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c game_loop.c -o $@

levels_data.o: levels_data.c levels.h levels_data.h game_types.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c levels_data.c -o $@

levels_runtime.o: levels_runtime.c levels.h levels_data.h levels_runtime.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c levels_runtime.c -o $@

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

publish: $(TARGET).bin
	/bin/cp -f $(TARGET).bin ../../public/roms/@rkanoid\ MODERN\ -\ RECOMPILED.gba

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all assets publish clean

ifeq ($(DEVKITARM),)
TOOLROOT := ../.tools/arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi/bin
else
TOOLROOT := $(DEVKITARM)/bin
endif

CC := $(TOOLROOT)/arm-none-eabi-gcc
OBJCOPY := $(TOOLROOT)/arm-none-eabi-objcopy

TARGET := rkanoid-modern
OBJS := start_gnu.o wait_vbl_done.o data_gnu.o app_flow.o menu.o screens.o game_sprites.o \
	ball_motion.o physics.o collisions.o bonuses.o serve_state.o frame_present.o \
	game_loop.o levels_data.o level_state.o level_render.o

CPPFLAGS := -I.
CFLAGS := -marm -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -std=gnu89 -Wall
ASFLAGS := -marm -mcpu=arm7tdmi
LDFLAGS := -T linker.ld -nostartfiles -Wl,-Map,$(TARGET).map -Wl,--gc-sections -specs=nosys.specs

all: $(TARGET).bin

assets:
	python3 tools/rebuild_assets.py

levels-data:
	python3 tools/generate_levels_data.py

levels-data-check:
	python3 tools/generate_levels_data.py --check

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

game_loop.o: game_loop.c gameplay.h game_sprites.h game_state.h game_constants.h levels_data.h \
	level_state.h level_render.h physics.h collisions.h bonuses.h serve_state.h frame_present.h \
	ball_motion.h fixed_point.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c game_loop.c -o $@

ball_motion.o: ball_motion.c ball_motion.h fixed_point.h game_state.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ball_motion.c -o $@

physics.o: physics.c regs.h game_constants.h physics.h game_state.h ball_motion.h collisions.h fixed_point.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c physics.c -o $@

collisions.o: collisions.c game_constants.h collisions.h levels.h game_state.h ball_motion.h fixed_point.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c collisions.c -o $@

bonuses.o: bonuses.c game_constants.h bonuses.h game_sprites.h levels.h level_state.h game_state.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c bonuses.c -o $@

serve_state.o: serve_state.c regs.h game_constants.h serve_state.h game_sprites.h physics.h game_state.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c serve_state.c -o $@

frame_present.o: frame_present.c frame_present.h game_constants.h game_sprites.h gba_wait.h game_state.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c frame_present.c -o $@

levels_data.o: levels_data.c levels.h levels_data.h game_types.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c levels_data.c -o $@

level_state.o: level_state.c level_state.h levels.h levels_data.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c level_state.c -o $@

level_render.o: level_render.c level_render.h levels.h levels_data.h regs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c level_render.c -o $@

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

publish: $(TARGET).bin
	/bin/cp -f $(TARGET).bin ../../public/roms/@rkanoid\ MODERN\ -\ RECOMPILED.gba

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all assets levels-data levels-data-check publish clean

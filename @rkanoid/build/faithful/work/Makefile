TOOLROOT := ../../../.tools/arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi/bin
CC := $(TOOLROOT)/arm-none-eabi-gcc
OBJCOPY := $(TOOLROOT)/arm-none-eabi-objcopy

TARGET := rkanoid-faithful
OBJS := start_gnu.o wait_vbl_done.o data_gnu.o demo.o

CFLAGS := -marm -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -std=gnu89 -Wall
ASFLAGS := -marm -mcpu=arm7tdmi
LDFLAGS := -T linker.ld -nostartfiles -Wl,-Map,$(TARGET).map -Wl,--gc-sections -specs=nosys.specs

all: $(TARGET).bin

start_gnu.o: start_gnu.s
	$(CC) $(ASFLAGS) -c $< -o $@

wait_vbl_done.o: wait_vbl_done.s
	$(CC) $(ASFLAGS) -c $< -o $@

data_gnu.o: data_gnu.S
	$(CC) $(ASFLAGS) -c $< -o $@

demo.o: demo.c juego.c niveles.c regs.h stdio.h
	$(CC) $(CFLAGS) -c demo.c -o $@

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all clean

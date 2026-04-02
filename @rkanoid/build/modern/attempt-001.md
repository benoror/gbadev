# Attempt 001 - modern local toolchain build

Date: 2026-04-02

## Goal

Produce a working compiled build in this environment using a modern toolchain while preserving original sources.

## Steps executed

1. Downloaded Arm GNU Toolchain for Darwin ARM64 into `@rkanoid/.tools/`.
2. Created isolated workspace at `@rkanoid/build/modern/work` by copying `@rkanoid/current/`.
3. Added GCC-compatible build components:
   - `crt0.s` (startup + data/bss init + `C_Entry` jump)
   - `data_gnu.S` (`.incbin` assets and exported symbols)
   - `linker.ld` (ROM at `0x08000000`, IWRAM data/bss)
   - `Makefile`
4. Applied minimal source compatibility in modern workspace only:
   - replaced ARMCC inline asm VBlank wait in `regs.h` with portable C polling
   - changed `#include "stdio.h"` to `#include <stdio.h>` in `juego.c`
5. Built successfully via:
   - `make clean && make`

## Build output

- `rkanoid-modern.bin` (227 KB)
- `rkanoid-modern.elf` (252 KB)
- `rkanoid-modern.map` (204 KB)

ELF checks:

- machine: ARM
- entry point: `0x08000000`

## Known caveats

- Current environment has no installed GBA emulator binary, so runtime execution was not validated here.
- Asset fallback alias used in modern workspace:
  - `bitmaps/gameover.raw -> fin.raw`
  - `palettes/gameover.pal -> fin.pal`
  This is temporary and should be replaced with authentic Game Over assets when recovered.


# Current modern lane

This folder is the maintainable modernization lane derived from the working faithful rebuild.

## Purpose

- preserve the behavior learned in the faithful lane
- compile with a public open-source ARM bare-metal toolchain
- use a reproducible asset pipeline from preserved BMP sources
- progressively refactor the original one-translation-unit structure into normal multi-file C modules

## Build

Preferred public stack:

- `devkitARM` / `libgba` if installed (`DEVKITARM` in environment)

Fallback stack already supported in-repo:

- local Arm GNU Toolchain under `@rkanoid/.tools/`

Build commands:

```sh
make clean
make
make publish
```

## Asset pipeline

Rebuild generated `.raw` / `.pal` assets from preserved BMPs:

```sh
make assets
```

This uses:

- `tools/rebuild_assets.py`

and reads BMPs from:

- `project/source_art/`

## Structure

- `app_flow.c` - entry flow, fades, and top-level screen sequencing
- `menu.c` - title menu input and cursor sprites
- `screens.c` - shared full-screen presenters and background/sprite asset loading
- `game_loop.c` - gameplay orchestration, serve state, collisions, and bonuses
- `game_sprites.c` - OAM layout, HUD digits, paddle, ball, and bonus sprite helpers
- `levels_data.c` - data-driven level seeds and tile maps
- `levels_runtime.c` - runtime block initialization and background map synchronization
- `assets.h`, `gameplay.h`, `levels.h`, `levels_data.h`, `levels_runtime.h` - explicit gameplay and level module boundaries
- `gba_*.h`, `regs.h` - split platform helpers for registers, input, DMA, waits, fades, and audio
- `start_gnu.s`, `wait_vbl_done.s`, `data_gnu.S`, `linker.ld` - public-toolchain runtime/build files

## Translation pass

This lane now treats English names as the canonical source names for files, runtime identifiers, level structures, and asset symbols while preserving gameplay behavior. See:

- `RENAMES.md`

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

- `proyecto/backup_t/`

## Structure

- `demo.c` - front-end flow and screen transitions
- `juego.c` - gameplay loop, OAM updates, HUD, collisions
- `niveles.c` - level/block definitions and runtime block state
- `assets.h`, `gameplay.h`, `levels.h` - explicit module boundaries
- `start_gnu.s`, `wait_vbl_done.s`, `data_gnu.S`, `linker.ld` - public-toolchain runtime/build files

## Translation pass

This lane begins the English-oriented cleanup by exposing translated entry points and helper names while preserving original gameplay behavior. See:

- `RENAMES.md`


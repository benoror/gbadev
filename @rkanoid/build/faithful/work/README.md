# Current snapshot (`current/`)

This folder contains the most recent top-level `@rkanoid` snapshot as preserved during reorganization.

## Contents

- Core gameplay source (`demo.c`, `juego.c`, `niveles.c`)
- Platform/runtime code (`start.asm`, `regs.h`, `stdio.h`)
- Build scripts (`make.bat`, `make_mb.bat`)
- Embedded asset declaration (`data.asm`)
- Available assets (`bitmaps/`, `palettes/`, `proyecto/`)

## Build expectation

Original scripts expect legacy ARM tools:

- `zarmasm`
- `zarmcc`
- `zarmlink`

## Asset naming convention note

`data.asm` references `Bitmaps/` and `Palettes/` (capitalized), while this preserved snapshot currently has `bitmaps/` and `palettes/` (lowercase).

On case-sensitive filesystems this causes build failures unless compatibility aliases or path updates are added in a later phase.


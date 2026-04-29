# Phase 0 Baseline Capture (`@rkanoid`)

This document executes **Phase 0 - Safety and baseline capture** from `preservation-plan.md`.

Date: 2026-04-02
Scope: `@rkanoid/` directory only

---

## 1) Preservation inventory

## 1.1 Current tree inventory (present files)

Top-level active snapshot files:

- `demo.c`
- `juego.c`
- `niveles.c`
- `regs.h`
- `start.asm`
- `data.asm`
- `make.bat`
- `make_mb.bat`
- `stdio.h`
- `bloques.txt`
- docs: `research.md`, `preservation-plan.md`

Historical variants (kept in-place):

- `18_ene_0/` (source + build scripts + partial assets)
- `20_ago_0/` (source + build scripts + partial assets)
- `bakup_12/` (source + build scripts + partial assets)

Total files currently visible in `@rkanoid/`: 48

## 1.2 Asset references expected by assembler

From `data.asm` (top-level snapshot), expected files are:

- `Bitmaps/sprites.raw`
- `Palettes/sprites.pal`
- `Bitmaps/bg.raw`
- `Palettes/bg.pal`
- `Bitmaps/titulo.raw`
- `Palettes/titulo.pal`
- `Bitmaps/gameOver.raw`
- `Palettes/gameOver.pal`
- `Bitmaps/fin.raw`
- `Palettes/fin.pal`
- `Bitmaps/dream.raw`
- `Palettes/dream.pal`

Current top-level files actually present:

- `bitmaps/dream.raw` only

Note:

- Expected path uses `Bitmaps/` + `Palettes/` (capitalized), but present path is `bitmaps/` (lowercase) and no palette folder/files currently exist in this snapshot.

## 1.3 Historical snapshot asset completeness

Observed raw/pal files by variant:

- top-level: `bitmaps/dream.raw`
- `18_ene_0/`: `dream.raw`
- `20_ago_0/`: `dream.raw`, `titulo.raw`
- `bakup_12/`: `bitmaps/dream.raw`

Conclusion:

- All snapshots are **asset-incomplete** relative to what their `data.asm` expects.

---

## 2) Build/runtime assumptions baseline

## 2.1 Toolchain assumptions in scripts

From `make.bat`:

- assembler: `zarmasm`
- compiler: `zarmcc`
- linker: `zarmlink`
- target CPU: `ARM7TDMI`
- output: `demo.bin`
- link bases:
  - `-ro-base 0x08000000`
  - `-rw-base 0x03000000`

From `make_mb.bat`:

- same ARM tools but absolute path:
  - `C:\agb\Amateur\ARM250\Bin\...`
- output: `demo.mb`
- MultiBoot-oriented `-ro-base 0x02000000`

## 2.2 Runtime assumptions from source

- Startup is custom (`start.asm`) and jumps to `C_Entry` in `demo.c`.
- Asset bundling is assembler-driven via `INCBIN`.
- Input is active-low GBA keypad register.
- Graphics path relies on direct VRAM/OAM register access and specific memory layout.

## 2.3 Host environment assumptions

- Original scripts assume Windows `.bat` execution.
- Path casing likely relied on case-insensitive filesystem behavior.
- Legacy ARM tools from old Amateur/AGB SDK era are expected.

---

## 3) Safety capture outputs created in Phase 0

- Baseline technical report already present: `research.md`
- Phase 0 execution log: `phase0-baseline.md` (this file)
- Gap register: `KNOWN_GAPS.md`

---

## 4) Phase 0 outcome summary

Status: **completed**

We can now answer the Phase 0 exit question:

- What exists? -> inventoried above.
- What is missing? -> most required image/palette assets for all snapshots.
- What do build scripts assume? -> legacy ARM250 tools + Windows batch flow + specific ROM/RAM bases.


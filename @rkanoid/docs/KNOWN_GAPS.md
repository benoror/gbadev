# Known Gaps (`@rkanoid`)

This file tracks objective blockers discovered during Phase 0 baseline capture.

## Critical build blockers

1. Missing assets referenced by `data.asm`
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
   - `Palettes/dream.pal`
   - (only `dream.raw` currently exists in top-level snapshot)

2. Path/case mismatch risk
   - code expects `Bitmaps/` and `Palettes/`
   - current tree uses `bitmaps/` and has no palette directory
   - breaks on case-sensitive filesystems

3. Legacy toolchain dependency
   - build scripts require `zarmasm`, `zarmcc`, `zarmlink`
   - `make_mb.bat` hardcodes `C:\agb\Amateur\ARM250\Bin\...`

## Medium-risk preservation gaps

1. Versioned snapshots are partially incomplete in assets:
   - `18_ene_0/` incomplete
   - `20_ago_0/` incomplete
   - `bakup_12/` incomplete

2. Build reproducibility not yet validated:
   - no confirmed successful build in current environment

3. Historical docs mention old known gameplay bug:
   - infinite corner-collision cycle in older beta notes
   - should be tracked as behavioral reference during future modernization

## Assumptions to verify in next phase

1. Which snapshot is authoritative as "current release candidate" for preservation (`@rkanoid/` root vs dated folders).
2. Whether missing assets exist elsewhere in repository history or external backups.
3. Whether legacy builds should prioritize:
   - original Windows legacy toolchain reproduction first, or
   - immediate modern toolchain migration.


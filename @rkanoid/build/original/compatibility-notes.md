# Minimal compatibility adjustments (Phase 2)

These changes are intentionally non-invasive and do not modify gameplay logic.

## Applied in `current/`

1. Asset case alias for `gameOver` naming
   - Added `bitmaps/gameOver.raw -> gameover.raw`
   - Added `palettes/gameOver.pal -> gameover.pal`

Reason:

- `data.asm` references `gameOver.raw`/`gameOver.pal`, while extracted assets use lowercase `gameover.*`.

## Not applied yet

1. Toolchain substitution
   - No replacement wrappers for `zarmasm`/`zarmcc`/`zarmlink` were introduced in Phase 2.
   - This remains for later phases (modern toolchain iteration).

2. Source edits
   - No C/ASM source logic changes were introduced.


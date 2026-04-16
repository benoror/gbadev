# Faithful build lane

This folder contains a comparison-focused rebuild path for `@rkanoid`.

## Goal

Get closer to the original ARM250/ARMCC-era ROM behavior than the `build/modern/` lane by:

- preserving original source behavior
- porting the original startup more literally
- avoiding showcase-specific patches

## Current status

- first GNU faithful build succeeds
- output lives in `work/`
- artifacts are archived in `../../archive/build-artifacts/faithful/`

## Important notes

- This lane still links against modern newlib because `sprintf()` is unresolved otherwise.
- `gameOver` assets are still recovered via temporary `fin.*` fallback because the original `gameOver` raw/pal files are not present in the preserved source tree.
- This lane is for binary/runtime comparison, not yet for website showcase use.


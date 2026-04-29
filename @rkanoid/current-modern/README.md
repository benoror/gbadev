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

One-shot manual-test pipeline (regen levels → verify → build ROM → publish):

```sh
make ship         # incremental (fast)
make ship-clean   # clean rebuild
```

Standalone verification (physics tests + `levels_data.c` drift check), ideal for CI or
pre-commit hooks:

```sh
make check
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

## Level data generation

Level tile maps + block seeds are generated into `levels_data.c` from:

- `tools/levels_source/tiles.map` — shared glyph legend (ASCII)
- `tools/levels_source/levelXX.level` — 18×9 glyph grid per level
- `tools/levels_source/levelXX.seeds` — block seeds per level

Commands:

```sh
make levels-data
make levels-data-check
```

Generator:

- `tools/generate_levels_data.py`

## Structure

- `app_flow.c` - entry flow, fades, and top-level screen sequencing
- `menu.c` - title menu input and cursor sprites
- `screens.c` - shared full-screen presenters and background/sprite asset loading
- `game_loop.c` - gameplay loop orchestration (round/level flow), calling focused helpers
- `physics.c` - paddle movement + world/ball bounds rules
- `collisions.c` - paddle + brick collision resolution
- `bonuses.c` - falling bonus motion + pickup rules
- `serve_state.c` - pre-serve paddle/ball positioning + input
- `frame_present.c` - per-frame sprite/HUD presentation sequencing
- `game_sprites.c` - OAM layout, HUD digits, paddle, ball, and bonus sprite helpers
- `levels_data.c` - generated (committed) C tables for tile maps + block seeds
- `level_state.c` - mutable block runtime (`gBlocks`), level init/clear queries
- `level_render.c` - background composition (`gBackgroundMap`) + VRAM upload helpers
- `ball_motion.c` - fixed-point ball position / velocity helpers + trail-offset math
- `gba_audio.c` - PSG SFX engine (square 1/2 + noise) with non-blocking single-shot triggers
- `assets.h`, `gameplay.h`, `levels.h`, `levels_data.h`, `level_state.h`, `level_render.h` - explicit gameplay and level module boundaries
- `fixed_point.h`, `ball_motion.h` - fixed-point primitives shared by physics and rendering
- `gba_*.h`, `regs.h` - split platform helpers for registers, input, DMA, waits, fades, and audio
- `start_gnu.s`, `wait_vbl_done.s`, `data_gnu.S`, `linker.ld` - public-toolchain runtime/build files

### Sprite module note

`game_sprites.c` is intentionally still a **single module** after the gameplay split: it is ~175 LOC,
mostly static tables + small OAM writers, and splitting further would mostly churn include edges
without improving readability yet.

## Audio

`gba_audio.{c,h}` exposes a tiny non-blocking PSG SFX engine. `InitAudio()`
runs once at boot (called from `app_flow.c`); each `Sfx*()` is a fire-and-forget
register write that triggers a hardware channel and returns immediately, so the
game loop never stalls.

Wired events:

- `SfxPaddleHit` — `collisions.c::ApplyPaddleCollision` (every redirect)
- `SfxBrickHit` — `collisions.c::ProcessBlockCollisions` (non-pierce hits only,
  to avoid spam during the comet trail)
- `SfxWallBounce` — `physics.c::ApplyWorldBounds` (walls / ceiling / shield)
- `SfxBonusPickup` — `bonuses.c::ApplyCollectedBonus`
- `SfxLifeLost` — `physics.c::ApplyWorldBounds` (death floor)
- `SfxLevelClear` — `game_loop.c` when `IsLevelCleared()` is `TRUE`

Host tests use `tests/audio_stub.c` to no-op these symbols so the linker stays
happy on the test runner.

## Translation pass

This lane now treats English names as the canonical source names for files, runtime identifiers, level structures, and asset symbols while preserving gameplay behavior. See:

- `RENAMES.md`

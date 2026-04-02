# @rkanoid (`@rkanoid/`) - deep technical research report

## Scope and method

This report is based on a full read-through of the current `@rkanoid/` code snapshot:

- `demo.c` (entry point, menu, front-end screens)
- `juego.c` (main gameplay loop, controls, collisions, OAM updates)
- `niveles.c` (level maps, brick metadata, bonus assignment, completion checks)
- `regs.h` (hardware register bindings and low-level helpers)
- `start.asm` and `data.asm` (boot/runtime init + asset linking)
- build scripts (`make.bat`, `make_mb.bat`)
- historical notes from `dreamsoft.es.fm/web.archive.org-20020112174419.mht`

I also cross-checked historical snapshots (`18_ene_0`, `20_ago_0`, `bakup_12`) to understand evolution and intent.

---

## 1) What this program is

`@rkanoid/` is a complete GBA homebrew brick-breaker ("@rkanoid") with:

- title + intro splash + game-over screen flow
- single-player and two-player alternating mode
- 5 handcrafted levels
- breakable and unbreakable bricks
- powerups ("bonuses"): comet/trail ball, long paddle, extra life, big ball, "mesias"
- score/lives HUD rendered with sprites
- direct register-level programming (no SDK abstractions)

Design style is classic early-2000s GBA demo/homebrew:

- almost all logic in C source files included directly (`#include "juego.c"`, `#include "niveles.c"`)
- no separate headers for game modules
- global mutable state + `goto`-driven state transitions
- heavy direct VRAM/OAM writes

---

## 2) Historical context discovered in archive

From `dreamsoft.es.fm/web.archive.org-20020112174419.mht`:

- Mentions `@rkanoid Beta 2.0` (2001-08-26): promoted as 3 levels + MultiBoot build.
- Mentions `@rkanoid Beta 2.5` (2001-10-13): introduces bonuses and L/R key usage.
- Mentions a known "infinite cycle" corner-collision bug in old builds (especially with "Titan" unbreakable blocks), with a manual escape workaround.

This aligns with code structure:

- MultiBoot-target build script exists (`make_mb.bat`).
- Bonus system is deeply integrated.
- Physics are deterministic grid-based equalities, which can produce corner-path artifacts.

---

## 3) Build/runtime architecture

## 3.1 Boot chain

- `start.asm` sets CPU modes/stacks for IRQ/SVC/USR.
- Copies RW initialized data from ROM to RAM.
- Zeroes ZI section.
- Branches to `C_Entry` in `demo.c`.

This is a custom CRT startup suitable for old ARM toolchains (`zarmasm`, `zarmcc`, `zarmlink`).

## 3.2 Asset linking model

`data.asm` exports symbols and uses `INCBIN` for:

- sprite sheet + sprite palette
- game background + palette
- title, game over, ending, and intro ("dream") images + palettes

`demo.c` imports these symbols as extern arrays and pushes them to VRAM/PLTT via loops and DMA.

## 3.3 Rendering mode and register setup

From `Inicia()`:

- `DISPCNT = 0x1140`
- `BG0CNT = 0x00C7`
- `BG1CNT = 0x0285`

The project is effectively using Mode 0 tiled background + objects, with sprite engine enabled and 1D OBJ mapping (`DISPCNT` bit 6).

Observations:

- BG0 is the active tilemap layer for playfield and static full-screen images.
- BG1 register is configured but not actively used in current flow.

---

## 4) Input, timing, and low-level helpers

## 4.1 Input semantics

`joypad()` reads `P1` and uses active-low logic:

- pressed button -> bit cleared
- code checks presses with `if (!(boton & J_X))`

This pattern is used consistently.

## 4.2 Frame/timing

- `wait_vbl_done()` busy-waits scanline until VBlank starts (line 160).
- `Wait(n)` stacks `n` VBlanks plus extra inner loop delay.
- Main gameplay updates per VBlank (`wait_vbl_done()` once per frame).

This keeps gameplay deterministic and simple.

## 4.3 DMA usage

DMA3 is used for OBJ data/palette loading:

- sprite palette to `OBJ_PLTT`
- sprite tile graphics to `OBJCHARDATA`

Large BG image transfers are mostly manual loops instead of DMA in this snapshot.

---

## 5) High-level game flow (`demo.c`)

Entry (`C_Entry`) sequence:

1. Initialize video registers.
2. Show "dream" splash, fade in/out.
3. Show title screen, fade in.
4. Load sprite assets, open menu.
5. Fade out, set game BG.
6. Run `Main(PLAYERS)`.
7. Fade out, show game-over screen, wait Start, fade out.

Notes:

- `MuestraFin()` exists but is never called in this build path.
- Even if all levels are beaten (`nivel` reaches 6), control returns to generic game-over sequence.

---

## 6) Menu and player mode behavior

`Menu()`:

- shows pointer sprites (`OAM[0]`, `OAM[1]`) beside options
- supports up/down wrap among 3 entries
- Start confirms
- option 1 -> one player
- option 2 -> two-player alternating (`PLAYERS=2`)

In 2-player mode, turns alternate on each reset/life cycle by toggling `PLAYER` in `Main()`.

---

## 7) Core gameplay model (`juego.c`)

## 7.1 Coordinate system and boundaries

- horizontal playable range: `MIN_X=16` to `MAX_X=160`
- top limit: `MIN_Y=8`
- bottom limit: `MAX_Y` mutable (160 normally, reduced by "mesias")

Ball and paddle dimensions are mutable due to bonuses:

- ball: normal `6x6`, big ball `8x8`
- paddle: normal width 32, long width 48

## 7.2 Launch phase

`IniciaMovimientoRaqueta()`:

- attaches ball to paddle before launch
- loop continues while A is **not** pressed
- allows positioning:
  - left/right moves paddle + carried ball
  - L/R nudges ball offset over paddle before launch
  - B changes movement speed

The chosen launch x positions are saved in globals (`Inicio_Bx`, `Inicio_Rx`).

## 7.3 Main loop state machine

`Main()` uses labels and gotos:

- `GANA_NIVEL` -> level setup and transition
- `REINICIA` -> reset life/turn state, relaunch
- inner `while (LifeCount > 0 && nivel<=5)` -> per-frame simulation

Loop includes:

- wall and paddle collisions
- brick collision pass
- bonus-fall update/catch
- player input and paddle movement
- sprite/HUD refresh
- level completion check

---

## 8) Brick/level data model (`niveles.c`)

## 8.1 Data structures

`Bloques L[162]` stores per-brick runtime state:

- geometry (`x`, `y`, `xM`, `yM`)
- tile indices (`tile1`, `tile2`) into level map
- breakability (`destruye`)
- hit points (`golpes`)
- bonus type and bonus runtime (`BONUS`, `activoBonus`, `bonX/Y`, `bonN`)

`bgMap` is the full UI/map template (playfield + side HUD border).
`blockMap_1..5` are 18x18 tile slices inserted into `bgMap`.

## 8.2 Level initialization flow

`IniciaPropiedadesBloques(nivel)`:

1. Clears all `L[]` entries to defaults.
2. Calls `PropNivelX()` with handcrafted brick metadata.
3. Computes derived fields:
   - `xM = x+16`, `yM = y+8`
   - `tile2 = tile1+1`
4. Assigns falling-bonus sprite slots (`bonN`) in pairs.

Each `PropNivelX()` manually places bricks and assigns:

- breakable/unbreakable behavior
- multi-hit counts (`golpes`, commonly 5 on harder bricks)
- bonus payloads

## 8.3 Level completion

`ChecaTerminadoNivel()` returns TRUE when no bricks remain where:

- `x != 0` and `destruye == TRUE`

Unbreakable blocks do not block level completion.

---

## 9) Collision and physics details

## 9.1 Ball-wall and fail conditions

- Hits side walls: invert horizontal direction.
- Hits top: invert vertical direction.
- Reaches bottom:
  - normal: lose life and reset
  - "mesias" active: consume protection and continue

## 9.2 Paddle collision partition

Paddle top surface is split into left/right half:

- left half sends ball left-up
- right half sends ball right-up

Side contacts also force directional correction.
Behavior is duplicated for short and long paddle widths.

## 9.3 Brick collision strategy

For each block entry, checks all 4 contact faces using equality tests:

- top contact
- bottom contact
- left contact
- right contact

On hit:

- direction adjusted
- optional bonus activation if brick is near destruction
- `golpes` decremented if >0
- if destruction condition met, tilemap overwritten with "pressed/empty" tiles (`O1/O2`) and score added

---

## 10) Bonus system specifics

Bonus constants map to sprite tile IDs in this snapshot:

- `COMET_BONUS` (trail/comet mode)
- `LONG_R_BONUS` (long paddle)
- `LIFE_BONUS` (+1 life)
- `BIGBALL_BONUS` (8x8 ball)
- `MESIAS_BONUS` (bottom grace/protection mode)

Flow:

1. Brick collision sets `activoBonus=TRUE` when appropriate.
2. Bonus sprite pair falls 1 px/frame.
3. If overlaps paddle rectangle, applies effect and hides bonus sprite.
4. If falls below limit, despawns.

Important interaction details:

- Comet sets `rastro=TRUE` (trail rendering uses OAM 17..22).
- Big ball cancels comet (`rastro=FALSE`) and changes ball dimensions/tile.
- Mesias reduces active bottom boundary (`MAX_Y=160-12`) and draws a special floor strip (`do_mesias()`).

---

## 11) OAM/sprite allocation map (effective)

By inspection, sprite slots are used roughly as:

- `0..1`: menu arrows (title menu context)
- `1..7`: score digits in gameplay (overwrites prior menu use)
- `8`: current level digit
- `9..14`: paddle segments (4 or 6 sprites depending on long mode)
- `15..16`: life digits
- `17`: ball core
- `18..22`: comet trail pieces
- `23+`: falling bonus sprite pairs

`limpia_oam()` pushes sprites off-screen to hide them.

---

## 12) Important quirks, bugs, and fragilities

These are key findings that explain weird behavior and historical instability.

## 12.1 Out-of-bounds array access on `L[162]` loops

`L` is declared as `Bloques L[162]` (valid indices `0..161`), but many loops use `i<=162`.

Impact:

- reads/writes one element past array end
- undefined behavior, possibly intermittent corruption

## 12.2 Asymmetric brick destruction check (left-face collision bug)

In brick collision logic:

- top, bottom, and right face usually require `golpes == 0` before removal
- left-face branch destroys whenever `destruye == TRUE` (without `golpes==0`)

Impact:

- multi-hit bricks can be destroyed early when hit from left side
- gameplay inconsistency hard to diagnose by feel

## 12.3 `mesias` used before initialization in first level setup

At `GANA_NIVEL`, `RefrescaBG(nivel,mesias)` can run before `mesias` is set to `FALSE` in `REINICIA`.

Impact:

- undefined first-level background variant (rare but possible)

## 12.4 Ending flow not wired

`MuestraFin()` exists but no call site in final game sequence.

Impact:

- finishing level 5 does not show dedicated "fin" screen; returns to game-over flow.

## 12.5 Asset path/case sensitivity mismatch in current repo state

`data.asm` expects `Bitmaps/*` and `Palettes/*`, but current `@rkanoid/` snapshot does not contain that full tree (and surviving folders are lowercase in repo history).

Impact:

- as-is build is not reproducible without reconstructing assets/paths.
- historically this likely worked on case-insensitive Windows setups.

## 12.6 VRAM overwrite-prone full-screen copy strategy

Full-screen image loops copy very large buffers into BG VRAM region while code later reloads sprite graphics again.

Interpretation:

- likely relied on "overwrite then reload OBJ" sequencing intentionally
- works but is memory-layout-fragile and mode-dependent

---

## 13) Evolution across snapshots

From `20_ago_0` -> newer snapshots:

- bonus constants changed from small enums (`0..4`) to sprite-tile IDs (`52..68`), simplifying direct `Attrib2` use for bonus sprites.
- `Main()` signature changed from no params to `Main(u16 PLAYERS)` with explicit one/two-player selection flow.
- broad architecture remained stable (same labels, same collision structure, same level/property style).

`bakup_12` and top-level `@rkanoid/` are very close in gameplay logic, indicating this is a mature late-iteration snapshot.

---

## 14) Why this code still works despite "bad code"

Even with fragilities, the project has strong practical design choices:

- deterministic single-step frame loop (simple to reason about)
- data-driven levels (manual but clear)
- direct and cheap collision checks aligned to tile-sized objects
- minimal abstraction overhead (good fit for constrained hardware and era tools)
- robust fallback controls (manual relaunch/reset during play)

It is "rough" code, but architecturally coherent for a solo teen homebrew project of that period.

---

## 15) If you ever want to modernize/preserve it

High-value preservation steps:

1. Fix bounds (`<162` everywhere).
2. Normalize brick-destruction condition across all collision sides.
3. Initialize all state (`mesias`, etc.) immediately on function entry.
4. Separate `*.h` declarations and `*.c` definitions.
5. Replace `#include "*.c"` with proper compilation units.
6. Recreate a buildable asset tree with canonical casing.
7. Add a tiny compatibility layer (or devkitARM port) while preserving original behavior.

---

## Appendix A - notable controls

- D-pad: paddle movement
- A: launch ball from paddle
- B: alternate paddle speed
- L/R (pre-launch): fine-tune ball start offset on paddle
- Select: immediate reset/relaunch state (in-game)
- Start: menu confirm / game-over continue

---

## Appendix B - short file responsibilities map

- `start.asm`: runtime bootstrap and memory init
- `regs.h`: register constants, VBlank waiting, DMA helper, fades, simple sound helper
- `data.asm`: binary asset embedding
- `demo.c`: front-end flow and scene transitions
- `juego.c`: simulation and rendering of active gameplay
- `niveles.c`: brick maps, per-level metadata, bonus assignment, level completion


# Changelog

All notable changes to this repository — across the **@rkanoid** game series, the **GBA Dev Museum** showcase, and the surrounding tooling — are recorded here.

Format loosely follows [Keep a Changelog](https://keepachangelog.com/). Game versions are aligned with [`public/catalog.json`](./public/catalog.json). Dates are commit dates from the `preserve` branch.

---

## [Unreleased]

Nothing yet — latest tip is `1f70e3e` (audio integration).

---

## [Museum v0.3] · @rkanoid v3.0.0 + SFX — 2026-04-28

### Added
- **PSG audio engine** (`@rkanoid/current-modern/gba_audio.{c,h}`):
  - `InitAudio()` master enable + channel routing.
  - Six fire-and-forget effects on Square 1, Square 2, and Noise channels:
    `SfxPaddleHit`, `SfxBrickHit`, `SfxWallBounce`, `SfxBonusPickup`,
    `SfxLifeLost`, `SfxLevelClear`.
  - Wired into `collisions.c`, `bonuses.c`, `physics.c`, `game_loop.c`,
    `app_flow.c`. Host-side stub (`tests/audio_stub.c`) keeps `make test` green.
- **Canvas-based museum player** — direct `<canvas>` + `mGBA` WASM swap,
  hot-swappable ROMs without full page reloads.
- **Shared player core** (`public/player/core.js`) reused by the standalone
  player and the museum stage.
- Stage controls: pause, reset, screenshot, save/load slot, volume slider.

### Changed
- Iframe player replaced by canvas; museum and standalone share one mGBA boot path.
- Keyboard hints + every player control relocated from the iframe to the
  museum's `.stage-controls`.
- Standalone player rebranded its fullscreen button as **Open in new window**.

### Fixed
- COI (cross-origin isolation) reload guard hardened with a `sessionStorage`
  counter to avoid reload loops in private browsing.
- Reset now hard-boots: `quitGame()` → unlink auto-save → `loadGame()`.

---

## [Museum v0.2] · @rkanoid v3.0.0 — 2026-04-19

### Added
- **GBA Dev Museum** showcase (`public/index.html`, `museum.css`, `museum.js`,
  `catalog.json`) — a versioned cartridge rack styled after a real GBA console.
- GBA console + cartridge SVGs (`public/images/gba-console.svg`,
  `gba-cart.svg`).
- LED-style indicators on stage buttons (CRT shader, fullscreen, etc.).
- Save/load state slots with `alert()` feedback; explicit hard reset.
- Dynamic ball-speed ramp: +20 % every 30 s with on-screen `T mm:ss` and
  `S dd` HUD indicators (`game_loop.c`, `game_sprites.c`).
- HUD layout pass: left-aligned score, time-of-life-paddle row,
  consistent `L {DD}` / `T {MM:SS}` / `S {DD}` formatting.

### Changed
- **Dropped `gbajs3`** in favor of a minimal client-only player wrapping
  `mgba-wasm` (no Node/Python server requirement).
- ES-module bootstrap: `<script type="module">` + `import mGBA from './mgba.js'`.
- Splash fade x2 faster, all other fades x4 faster (`FadeIn/Out 28 → 7`,
  `WaitVBlanks 100 → 20`).
- ROM fetches now use `cache: 'no-store'` to avoid stale builds during dev.

### Fixed
- `Module.FSInit()` is now awaited (no more "Loading ROM…" hang).
- ROM path now passes the full VFS path to `Module.loadGame()`.
- COOP/COEP service worker (`coi-sw.js`) moved to root scope so both
  `/index.html` and `/player/` get cross-origin isolation.

---

## [@rkanoid v3.0.0 — Modern Recompile] — 2026-04-16

The `current-modern` lane: a from-scratch modernization built with a real
toolchain, structured codebase, host-side tests, and a level pipeline.

### Added
- **Q8.8 fixed-point physics** (`fixed_point.h`, `ball_motion.{c,h}`):
  sub-pixel position, constant-magnitude velocity, integer `u32_isqrt` for
  normalization without floats.
- **Variable-angle paddle launch**: hit offset → launch angle, constant speed
  preserved across bounces.
- **AABB + MTV collision resolution** with positional snapping
  (`collisions.c`) — eliminates tunneling, trapping, and screen-warps.
- **Sub-step integrator** (`IntegrateBallMotionThisFrame`): up to 16
  sub-steps per frame to keep steep angles physically sound.
- **Velocity-driven ball trail** (`BallComputeTrailOffset`) — comet trail
  follows arbitrary angles, not just 45°.
- **10 levels** (5 new, all reordered by difficulty) with a human-editable
  source pipeline:
  - `tools/levels_source/*.level` (whitespace-separated glyph grids)
  - `tools/levels_source/*.seeds` (block properties)
  - `tools/levels_source/tiles.map` (glyph → tile mapping)
  - `tools/generate_levels_data.py [--check]` regenerates `levels_data.c`.
- **Working-copy tile map** so broken bricks don't reappear after a life loss.
- **Host-side test harness** (`tests/test_physics.c`) — 117 checks across
  motion, collisions, corners, and integration. Compiles with plain `gcc`.
- **Build pipeline targets** (`Makefile`): `assets`, `levels-data`,
  `levels-data-check`, `test`, `check`, `publish`, `ship`, `ship-clean`.
- Plan documents preserved in `.cursor/plans/`.

### Changed
- Code split into focused modules: `app_flow`, `menu`, `screens`,
  `game_sprites`, `ball_motion`, `physics`, `collisions`, `bonuses`,
  `serve_state`, `frame_present`, `game_loop`, `level_state`,
  `level_render`, `levels_data`.
- Renamed Spanish identifiers to English (`Inicia → InitializeDisplay`, etc. —
  see `RENAMES.md`).

### Fixed
- Tunneling on steep angles, ball trapping inside bricks, ball-warps into
  unreachable screen regions, teleport-on-upward-block-hit, zero-velocity
  fallback in `BallNormalizeVelocity`, `u16` wrap-around when snapping near
  playfield bounds.

---

## [@rkanoid Faithful 1.0] — 2026-04-02

### Added
- **Faithful rebuild** (Phase 2): reverse-engineered from the surviving
  sources, compiled with an original-compatible toolset for
  side-by-side comparison against the 2005 demo ROM.
- Phase-2 docs: compatibility notes, attempt logs, toolchain profile.

---

## [Preservation Layout] — 2026-04-02

### Added
- Phase 0 baseline capture (`@rkanoid/PHASE_0_BASELINE.md`,
  asset inventory, build assumptions, known gaps).
- Preservation plan (`@rkanoid/PRESERVATION_PLAN.md`) covering goals,
  phases, and immediate tasks.
- Detailed technical research report on program scope, history,
  build architecture, gameplay mechanics, and collision physics.
- Repository layout: `@rkanoid/current/` and `@rkanoid/archive/`
  (with READMEs), preliminary build scripts.
- GBA Dev Museum scaffold: showcase HTML, COOP/COEP service worker,
  initial emulator integration with playable ROM links.

---

## [@rkanoid Historical Releases] — 2003 – 2005 _(curated 2020 – 2021)_

Original ROMs by **Ducker**, preserved as archival objects in
`public/roms/archive/` and surfaced through the museum catalog.

| Version  | Year | Notes |
|----------|------|----------------------------------------------|
| v0.01    | 2003 | Earliest surviving demo; proto paddle, single block row |
| v0.02    | 2003 | Bounce geometry tuning |
| v0.09    | 2003 | Initial level layouts |
| Beta 1.0 | 2004 | First public beta; full HUD |
| Beta 1.5 | 2004 | Bonus items introduced |
| v2.00    | 2004 | Major revision; level overhaul |
| v2.50    | 2004 | Polish pass; paddle physics tuned |
| v2.80    | 2005 | Last archival pre-release build |
| **LATEST DEMO** | 2005 | The shipped demo ROM (PDRoms / freeware) |

Listed historically on PDRoms:
<https://pdroms.de/files/nintendo-gameboyadvance-gba/arkanoid-v2-8-beta>

---

## Other Games (archival)

- **Cebelix** — Dreamsoft platformer prototype (2003).
- **DexGex** _(formerly Mystic Quest)_ — top-view RPG, Dreamsoft (2004).
- **Mystic Q** — earlier Dreamsoft puzzle prototype (2004).
- **Pik Roll** — rolling action prototype, Dreamsoft (2004).

All are surfaced through the museum's catalog with their distributed demo ROMs.

---

## Repository origin — 2020-12 / 2021-01

Initial GitHub upload of the historical demo ROMs, screenshots, and
`dreamsoft.es.fm` artifacts; first README, `.gitignore`, and `public/images/`
collateral. Predates any source-level work — the repository was a museum
of compiled binaries before it became a museum *and* an active port.

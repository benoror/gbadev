# @rkanoid Preservation Plan

## Goal

Preserve `@rkanoid` as a historical project while making it reproducible, understandable, and incrementally modernizable without losing original behavior.

Core principles:

- Preserve history first, modernize second.
- Keep the original source and assets immutable once archived.
- Prefer small, reversible changes.
- Validate behavior on every phase.

---

## Phase 0 - Safety and baseline capture

Objective: Freeze what exists before restructuring.

Actions:

1. Create a preservation inventory:
   - list all currently present source files, binaries, and assets
   - identify missing assets referenced by `data.asm` (`Bitmaps/*`, `Palettes/*`)
2. Capture current runtime/build assumptions:
   - documented toolchain names (`zarmasm`, `zarmcc`, `zarmlink`)
   - expected case-sensitive vs case-insensitive path behavior
3. Store baseline docs:
   - keep `research.md` as the technical baseline
   - add a short `KNOWN_GAPS.md` for missing files and build blockers

Exit criteria:

- We can answer: "what exists, what is missing, what exact build scripts assume."

---

## Phase 1 - Organize code and assets for long-term readability

Objective: Make repository layout sane while preserving all historical iterations.

Proposed structure:

- `@rkanoid/current/` - most recent playable source + assets
- `@rkanoid/legacy/18_ene_0/`
- `@rkanoid/legacy/20_ago_0/`
- `@rkanoid/legacy/bakup_12/`
- `@rkanoid/archive/` - optional snapshots, binaries, raw historical dumps
- `@rkanoid/docs/` - research and preservation docs

Actions:

1. Move the current top-level code into `@rkanoid/current/`.
2. Move dated variants into `@rkanoid/legacy/` and keep each self-contained.
3. Normalize asset folder conventions in each version:
   - pick canonical casing (`bitmaps/`, `palettes/`) and document it
   - keep compatibility aliases if needed for old toolchains
4. Add per-version readmes:
   - source provenance
   - known differences
   - expected build command

Important preservation rule:

- Do not "merge" old versions into one codebase; preserve each version as a historical artifact.

Exit criteria:

- Any person can identify latest vs historical versions in under 1 minute.

---

## Phase 2 - Rebuild with original/compatible toolsets (minimal changes)

Objective: Reproduce original build with as few code edits as possible.

Actions:

1. Create a dedicated `build/original/` notes file:
   - tool versions
   - host OS assumptions
   - exact commands from `make.bat` / `make_mb.bat`
2. Attempt build in compatibility environments:
   - Windows VM or Wine-like setup for legacy tools
   - path/case behavior matching original assumptions
3. Apply only "build unblock" changes:
   - file path casing fixes
   - folder location updates
   - script path corrections
4. Keep these changes isolated:
   - use a compatibility patch file or wrapper scripts
   - avoid gameplay logic edits in this phase

Validation:

- Produce `demo.bin` (or equivalent) and run in emulator.
- Verify title, menu, gameplay start, and at least one level completion.

Exit criteria:

- A reproducible "legacy-compatible" build path is documented and works.

---

## Phase 3 - Iterate with modern toolsets

Objective: Build with current open toolchains while preserving behavior.

Candidate targets:

- devkitARM + modern `arm-none-eabi-*`
- modern assembler/linker flow compatible with GBA ROM output

Actions:

1. Create `current-modern/` build configuration (non-destructive):
   - modern Makefile or CMake-like minimal build
   - separate from original scripts
2. Port incrementally:
   - startup (`start.asm`) compatibility
   - asset linking (`INCBIN` paths/symbol exports)
   - C compiler compatibility flags
3. Replace deprecated/non-portable syntax only when required.
4. Track semantic parity:
   - compare frame behavior and input feel in emulator
   - maintain a "behavior differences" log

Validation checklist per iteration:

- boots to intro/title
- menu navigation works
- ball launch and collisions behave as expected
- bonuses trigger correctly
- level transition and game-over flow still work

Exit criteria:

- Project builds on modern toolchain with documented diffs and no major gameplay regressions.

---

## Phase 4 - Evaluate and adopt open-source libraries carefully

Objective: Use public OSS libraries where they improve maintainability without erasing the original project identity.

Likely categories:

- GBA hardware abstraction (register wrappers, DMA helpers)
- sprite/OAM helper routines
- asset conversion pipeline utilities
- testing/emulation automation helpers

Adoption strategy:

1. Research candidate libraries (license + maintenance + community trust).
2. Introduce adapters first, not full rewrites:
   - keep original game logic mostly untouched
   - route low-level helpers through wrappers
3. Preserve "pure original" path:
   - keep legacy build and source untouched in parallel
4. Record attribution and license notices in `THIRD_PARTY.md`.

Exit criteria:

- At least one OSS dependency adopted with clear value and no behavior regression.

---

## Cross-phase quality gates

These should be true after every phase:

- Historical versions remain recoverable and intact.
- Changes are documented (what changed, why, and risk).
- A person unfamiliar with the repo can still build at least one version.
- No silent gameplay logic drift.

---

## Suggested execution order (high level)

1. Baseline inventory + missing-assets map.
2. Repository reorganization (`current` + `legacy` + `docs`).
3. Legacy-compatible build reproduction.
4. Modern toolchain build prototype.
5. Selective OSS library adoption via adapters.

---

## Immediate next tasks (practical)

1. Create `@rkanoid/docs/` and move `research.md` there.
2. Draft `@rkanoid/docs/version-matrix.md`:
   - version
   - date folder
   - build script
   - asset completeness
   - known issues
3. Build a first `asset-manifest.csv` (expected vs present files).
4. Try one "minimal-change" legacy build attempt and document blockers.


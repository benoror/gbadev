# @rkanoid - preservation layout

This directory is organized for long-term readability and historical preservation.

## Structure

- `current/` - most recent top-level snapshot (source + currently available assets)
- `legacy/` - historical variants kept as separate artifacts
- `docs/` - research and preservation documentation
- `current-modern/` - maintainable modernization lane derived from the faithful rebuild
- `build/original/` - original toolchain rebuild notes and attempt logs
- `build/modern/` - modern-toolchain parallel build path
- `build/faithful/` - comparison-focused rebuild lane closer to original startup/runtime
- `archive/` - build artifacts, emulator saves, and non-source historical outputs

## Quick navigation

- Main technical analysis: `docs/research.md`
- Preservation roadmap: `docs/preservation-plan.md`
- Baseline capture (Phase 0): `docs/phase0-baseline.md`
- Known blockers: `docs/KNOWN_GAPS.md`
- Version matrix: `docs/version-matrix.md`
- Phase 2 rebuild notes: `build/original/README.md`
- Modern build result: `build/modern/README.md`
- Faithful comparison lane: `build/faithful/README.md`
- Active modernization lane: `current-modern/README.md`

## Preservation rules

- Do not merge historical variants into one codebase.
- Keep version folders self-contained.
- Prefer additive documentation over destructive cleanup.


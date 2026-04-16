# Attempt 001 - faithful GNU comparison build

Date: 2026-04-16

## Setup

Created a fresh comparison worktree from `@rkanoid/current/`:

- `@rkanoid/build/faithful/work`

Added only minimum GNU compatibility/build pieces:

- `start_gnu.s` - closer port of original `start.asm`
- `wait_vbl_done.s` - external ARM assembly implementation matching original polling semantics
- `data_gnu.S` - asset embedding in original order
- `linker.ld`
- `Makefile`

Minimal source adaptation:

- replaced ARMCC inline-asm body in `regs.h` with external declaration for `wait_vbl_done()`

## Temporary gaps

- `gameOver.raw` / `gameOver.pal` still missing in preserved assets
- faithful lane uses temporary fallback aliases to `fin.raw` / `fin.pal`

## Output

- `rkanoid-faithful.bin`
- `rkanoid-faithful.elf`
- `rkanoid-faithful.map`

Archived at:

- `@rkanoid/archive/build-artifacts/faithful/`

## Comparison snapshot

- original ROM size: `203K`
- modern ROM size: `228K`
- faithful ROM size: `227K`

SHA-256:

- original: `d9520404396ae3908ac159ee7d1e105a597ea9fa01f9aec2cb902949b55a78f2`
- modern: `0f276bc7f4fe5a22d31801679e72861a42c9218cfb20313c50db2ddd95a937b3`
- faithful: `4220eee656d22d2543ba98e154adf2fb5afeccf44c982a17192674d3644f2311`

First differing byte offset versus original:

- modern: `192`
- faithful: `292`

Interpretation:

- the more literal startup path moves the rebuilt ROM closer to the original binary layout than the current modern lane
- however, newlib/runtime drift is still present (`_svfprintf_r`, `_impure_ptr`, etc.)


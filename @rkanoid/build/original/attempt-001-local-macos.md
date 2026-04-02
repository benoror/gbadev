# Attempt 001 - local macOS compatibility check

Date: 2026-04-02  
Working directory: `@rkanoid/current`

## Goal

Attempt a minimal-change build using the original command chain and capture blockers.

## Environment probe

Tool availability check result:

- `zarmasm`: no
- `zarmcc`: no
- `zarmlink`: no
- `arm-none-eabi-as`: no
- `arm-none-eabi-gcc`: no
- `make`: yes
- `wine`: no
- `cmd`: no
- `dosbox`: no
- `qemu-system-arm`: no

## Build command executed

```sh
zarmasm -CPU ARM7TDMI -Littleend start.asm && \
zarmasm -CPU ARM7TDMI -Littleend data.asm && \
zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst demo.c -o demo.o && \
zarmlink -bin -first start.o start.o data.o demo.o -map -ro-base 0x08000000 -rw-base 0x03000000 -o demo.bin
```

## Result

- Exit code: `127`
- Failure point: first command
- Error: `command not found: zarmasm`

## Conclusion

Phase 2 is partially complete:

- Original build profile documented.
- Minimal compatibility asset aliases applied.
- First compatibility attempt executed and logged.

Primary blocker for reproducible local build is missing legacy ARM250 toolchain binaries.


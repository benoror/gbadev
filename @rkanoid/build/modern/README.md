# Modern build path (`build/modern`)

This folder contains a parallel, non-destructive modern build bootstrap for `@rkanoid`.

## Result

A successful ARM build is produced at:

- `work/rkanoid-modern.bin`
- `work/rkanoid-modern.elf`
- `work/rkanoid-modern.map`

Artifacts are also copied to:

- `../../archive/build-artifacts/modern/`

## How to build

From `@rkanoid/build/modern/work`:

```sh
make clean
make
```

## Notes

- This path keeps `current/` untouched.
- It uses a local toolchain under `@rkanoid/.tools/`.
- Runtime smoke test in emulator is not yet automated because no emulator binary is installed on this machine.


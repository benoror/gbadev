# Legacy snapshot: `bakup_12`

Historical backup snapshot preserved as-is.

## Provenance

- Backup-style checkpoint (`bakup_12`) with source/build scripts and partial assets.
- Very close to the later top-level logic, but retained as independent artifact.

## Known differences vs `current/`

- Similar gameplay structure with minor iteration drift.
- Asset paths in `data.asm` expect `Bitmaps/` and `Palettes/`.
- Asset completeness is partial in current repository state.

## Expected build entry point

- `make.bat` (ROM)
- `make_mb.bat` (MultiBoot)

Both expect legacy ARM250-era toolchain executables.


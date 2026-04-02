# Version Matrix (`@rkanoid`)

| Version folder | Role | Build scripts | Asset completeness | Known notes |
| --- | --- | --- | --- | --- |
| `current/` | latest preserved top-level snapshot | `make.bat`, `make_mb.bat` | partial | `data.asm` uses `Bitmaps/` + `Palettes/` while preserved folders are lowercase |
| `legacy/18_ene_0/` | historical checkpoint | `make.bat`, `make_mb.bat` | partial | early bonus/physics iteration differences |
| `legacy/20_ago_0/` | historical checkpoint | `make2.bat`, `make_mb.bat` | partial | includes `copia_de.c` variant |
| `legacy/bakup_12/` | historical backup checkpoint | `make.bat`, `make_mb.bat` | partial | close to later logic; kept independent |

## Toolchain expectation (all versions)

- Legacy ARM250-era executables (`zarmasm`, `zarmcc`, `zarmlink`) are expected by scripts.

## Related docs

- `docs/research.md`
- `docs/preservation-plan.md`
- `docs/phase0-baseline.md`
- `docs/KNOWN_GAPS.md`


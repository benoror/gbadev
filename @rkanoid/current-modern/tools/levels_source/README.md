# Level source files

These files are the **source of truth** for `levels_data.c`.

- `levelNN.tilemap`: 18 rows × 9 tab-separated cells
  - each cell is `LEFT,RIGHT` where each half is a `u8` tile index (`TILE_*` macros from `levels.h`)
- `levelNN.seeds`: one block seed per line
  - `slot,x,y,tileIndex,destructible,hitsRemaining,bonusType`

Regenerate C tables:

```sh
make levels-data
```

Verify the generated output matches the committed `levels_data.c` (useful for CI / pre-commit):

```sh
make levels-data-check
```

# Level source files

These files are the **source of truth** for `levels_data.c`.

- `tiles.map`: shared legend mapping a single **ASCII glyph** to the two `u8` tile halves
  (`LEFT`, `RIGHT`) that make up one brick.
- `levelNN.level`: **18 rows × 9 glyphs** (whitespace-separated). Each glyph represents one
  brick. Lines starting with `#` and blank lines are ignored.
- `levelNN.seeds`: block seeds — `slot,x,y,tileIndex,destructible,hitsRemaining,bonusType`.

Example row:

```
.  T  T  T  T  T  T  T  .
```

## Built-in glyphs

| Glyph | Tile pair |
|-------|-----------|
| `.`   | `TILE_EMPTY` |
| `B`   | `TILE_BLUE` |
| `R`   | `TILE_RED` |
| `G`   | `TILE_GREEN` |
| `Y`   | `TILE_YELLOW` |
| `O`   | `TILE_BROWN` |
| `A`   | `TILE_GRAY` |
| `P`   | `TILE_PURPLE` |
| `K`   | `TILE_PINK` |
| `M`   | `TILE_SMILE` |
| `T`   | `TILE_TITANIUM` (indestructible) |
| `I`   | `TILE_IRIS` |
| `S`   | `TILE_SHIELD` |

Add your own alias by appending a line to `tiles.map`.

## Workflow

Regenerate C tables from source:

```sh
make levels-data
```

Verify the generated output matches the committed `levels_data.c` (useful for CI / pre-commit):

```sh
make levels-data-check
```

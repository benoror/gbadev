#!/usr/bin/env python3
"""
Deterministically regenerate `levels_data.c` from small text sources.

Inputs (committed):
  tools/levels_source/level01.tilemap
  tools/levels_source/level01.seeds
  ... level05.*

Tilemap format:
  - 18 non-empty lines
  - each line has 9 tab-separated cells
  - each cell is `LEFT,RIGHT` where LEFT/RIGHT are either decimal integers or C identifiers
    (typically `TILE_*` macros from `levels.h`)

Note: a logical row is 9 brick-columns wide, but each column is stored as two `u8` tile indices
(LEFT/RIGHT halves). That matches the 18-wide `u8` indexing used by `CopyLevelTileMapToBackground()`.

Seeds format:
  - one seed per non-empty, non-comment line
  - `slot,x,y,tileIndex,destructible,hitsRemaining,bonusType`
  - booleans may be `TRUE`/`FALSE` (preferred) or `0`/`1`

This generator is intentionally boring: stable ordering, stable whitespace, and no cleverness.
"""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path


CELL_RE = re.compile(r"^([^,]+),([^,]+)$")


@dataclass(frozen=True)
class Paths:
    repo_root: Path
    out_c: Path
    source_dir: Path


def read_tilemap(path: Path) -> list[list[tuple[str, str]]]:
    lines = [ln.rstrip("\n") for ln in path.read_text(encoding="utf-8").splitlines()]
    lines = [ln for ln in lines if ln.strip() and not ln.lstrip().startswith("#")]
    if len(lines) != 18:
        raise ValueError(f"{path}: expected 18 tilemap rows, got {len(lines)}")

    grid: list[list[tuple[str, str]]] = []
    for row_index, ln in enumerate(lines, start=1):
        cells = ln.split("\t")
        if len(cells) != 9:
            raise ValueError(f"{path}: row {row_index} expected 9 tab-separated cells, got {len(cells)}")
        row: list[tuple[str, str]] = []
        for col_index, cell in enumerate(cells, start=1):
            c = cell.strip()
            m = CELL_RE.match(c)
            if not m:
                raise ValueError(f"{path}: row {row_index} col {col_index} bad cell {cell!r}")
            left, right = m.group(1).strip(), m.group(2).strip()
            if not left or not right:
                raise ValueError(f"{path}: row {row_index} col {col_index} empty half in {cell!r}")
            row.append((left, right))
        grid.append(row)
    return grid


def read_seeds(path: Path) -> list[tuple[str, str, str, str, str, str, str]]:
    rows: list[tuple[str, str, str, str, str, str, str]] = []
    for raw in path.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        parts = [p.strip() for p in line.split(",")]
        if len(parts) != 7:
            raise ValueError(f"{path}: expected 7 comma-separated fields, got {len(parts)} in {raw!r}")
        rows.append(tuple(parts))  # type: ignore[arg-type]
    if not rows:
        raise ValueError(f"{path}: no seeds")
    return rows


def emit_tilemap(name: str, grid: list[list[tuple[str, str]]]) -> list[str]:
    out: list[str] = []
    out.append(f"static u8 {name}[] = {{")
    for y in range(18):
        parts: list[str] = []
        for x in range(9):
            left, right = grid[y][x]
            parts.append(f"{left},{right},")
        if len(parts) != 9:
            raise RuntimeError("internal: expected 9 cells")
        out.append("\t".join(parts))
    out.append("};")
    out.append("")
    return out


def emit_seeds(name: str, seeds: list[tuple[str, str, str, str, str, str, str]]) -> list[str]:
    out: list[str] = []
    out.append(f"static const BlockSeed {name}[] = {{")
    for slot, x, y, tile_index, destructible, hits, bonus in seeds:
        out.append(
            "    {"
            + f"{slot}, {x}, {y}, {tile_index}, {destructible}, {hits}, {bonus}"
            + "},"
        )
    out.append("};")
    out.append("")
    return out


def emit_getters() -> list[str]:
    return [
        "const BlockSeed *GetLevelBlockSeeds(u16 level, u16 *count)",
        "{",
        "    switch (level) {",
        "    case 1:",
        "        *count = sizeof(level1BlockSeeds) / sizeof(level1BlockSeeds[0]);",
        "        return level1BlockSeeds;",
        "    case 2:",
        "        *count = sizeof(level2BlockSeeds) / sizeof(level2BlockSeeds[0]);",
        "        return level2BlockSeeds;",
        "    case 3:",
        "        *count = sizeof(level3BlockSeeds) / sizeof(level3BlockSeeds[0]);",
        "        return level3BlockSeeds;",
        "    case 4:",
        "        *count = sizeof(level4BlockSeeds) / sizeof(level4BlockSeeds[0]);",
        "        return level4BlockSeeds;",
        "    case 5:",
        "        *count = sizeof(level5BlockSeeds) / sizeof(level5BlockSeeds[0]);",
        "        return level5BlockSeeds;",
        "    default:",
        "        *count = 0;",
        "        return level1BlockSeeds;",
        "    }",
        "}",
        "",
        "u8 *GetLevelTileMap(u16 level)",
        "{",
        "    switch (level) {",
        "    case 1: return level1TileMap;",
        "    case 2: return level2TileMap;",
        "    case 3: return level3TileMap;",
        "    case 4: return level4TileMap;",
        "    case 5: return level5TileMap;",
        "    default: return level1TileMap;",
        "    }",
        "}",
        "",
    ]


def generate(paths: Paths) -> str:
    chunks: list[str] = []
    chunks.append('#include "levels_data.h"')
    chunks.append('#include "levels.h"')
    chunks.append("")

    tilemaps: list[tuple[str, list[list[tuple[str, str]]]]] = []
    seed_arrays: list[tuple[str, list[tuple[str, str, str, str, str, str, str]]]] = []

    for level in range(1, 6):
        tilemap_path = paths.source_dir / f"level{level:02d}.tilemap"
        seeds_path = paths.source_dir / f"level{level:02d}.seeds"
        tilemaps.append((f"level{level}TileMap", read_tilemap(tilemap_path)))
        seed_arrays.append((f"level{level}BlockSeeds", read_seeds(seeds_path)))

    for name, grid in tilemaps:
        chunks.extend(emit_tilemap(name, grid))
    for name, seeds in seed_arrays:
        chunks.extend(emit_seeds(name, seeds))

    chunks.extend(emit_getters())
    return "\n".join(chunks).rstrip() + "\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--root",
        default=".",
        help="Path to @rkanoid/current-modern (default: cwd)",
    )
    parser.add_argument(
        "--stdout",
        action="store_true",
        help="Write generated C to stdout (do not modify levels_data.c)",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Exit non-zero if output would change levels_data.c",
    )
    args = parser.parse_args()

    repo_root = Path(args.root).resolve()
    paths = Paths(
        repo_root=repo_root,
        out_c=repo_root / "levels_data.c",
        source_dir=repo_root / "tools" / "levels_source",
    )

    rendered = generate(paths)
    if args.stdout:
        print(rendered, end="")
        return 0
    if args.check:
        existing = paths.out_c.read_text(encoding="utf-8")
        if existing != rendered:
            raise SystemExit("levels_data.c is out of date vs tools/levels_source (run `make levels-data`)")
        return 0

    paths.out_c.write_text(rendered, encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

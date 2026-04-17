#!/usr/bin/env python3
"""
Deterministically regenerate `levels_data.c` from small text sources.

Inputs (committed):
  tools/levels_source/tiles.map       shared glyph -> (left, right) tile mapping
  tools/levels_source/level01.level   9x18 glyph grid (whitespace-separated cells)
  tools/levels_source/level01.seeds   block seeds (bonuses, hit counts)
  ... level05.*

Level format:
  - 18 non-empty, non-comment rows
  - each row has 9 whitespace-separated tokens (one brick per token)
  - each token is a glyph defined in tiles.map (ASCII letter, '.', or emoji)

Tiles.map format:
  - `<glyph> <left_tile> <right_tile>` per non-comment line
  - multiple glyphs may map to the same tile pair (aliases)

Seeds format:
  - one seed per non-empty, non-comment line
  - `slot,x,y,tileIndex,destructible,hitsRemaining,bonusType`
  - booleans may be `TRUE`/`FALSE` (preferred) or `0`/`1`

This generator is intentionally boring: stable ordering, stable whitespace, and no cleverness.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Paths:
    repo_root: Path
    out_c: Path
    source_dir: Path


def read_tiles_map(path: Path) -> dict[str, tuple[str, str]]:
    mapping: dict[str, tuple[str, str]] = {}
    for raw in path.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split()
        if len(parts) != 3:
            raise ValueError(f"{path}: expected `<glyph> <left> <right>` in {raw!r}")
        glyph, left, right = parts
        if glyph in mapping and mapping[glyph] != (left, right):
            raise ValueError(f"{path}: glyph {glyph!r} already maps to {mapping[glyph]}, cannot remap to {(left, right)}")
        mapping[glyph] = (left, right)
    if not mapping:
        raise ValueError(f"{path}: no glyph mappings found")
    return mapping


def read_level(path: Path, tiles: dict[str, tuple[str, str]]) -> list[list[tuple[str, str]]]:
    lines = [ln.rstrip("\n") for ln in path.read_text(encoding="utf-8").splitlines()]
    lines = [ln for ln in lines if ln.strip() and not ln.lstrip().startswith("#")]
    if len(lines) != 18:
        raise ValueError(f"{path}: expected 18 rows, got {len(lines)}")

    grid: list[list[tuple[str, str]]] = []
    for row_index, ln in enumerate(lines, start=1):
        tokens = ln.split()
        if len(tokens) != 9:
            raise ValueError(f"{path}: row {row_index} expected 9 glyphs, got {len(tokens)} ({ln!r})")
        row: list[tuple[str, str]] = []
        for col_index, glyph in enumerate(tokens, start=1):
            pair = tiles.get(glyph)
            if pair is None:
                raise ValueError(f"{path}: row {row_index} col {col_index} unknown glyph {glyph!r}")
            row.append(pair)
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
    # `const`: keeps the source of truth in ROM and lets collision code mutate the working
    # copy (gActiveTileMap) without trashing the original for subsequent lives.
    out.append(f"static const u8 {name}[] = {{")
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
    lines: list[str] = []
    lines.append("/* Mutable working tile map. DestroyBlock writes TILE_BRICK_PRESSED_* here; LoadLevelTileMap")
    lines.append(" * refreshes it from the const ROM source so losing a life rolls the visuals back. */")
    lines.append("static u8 gActiveTileMap[sizeof(level1TileMap) / sizeof(level1TileMap[0])];")
    lines.append("")
    lines.append("const BlockSeed *GetLevelBlockSeeds(u16 level, u16 *count)")
    lines.append("{")
    lines.append("    switch (level) {")
    for level in range(1, 11):
        lines.append(f"    case {level}:")
        lines.append(f"        *count = sizeof(level{level}BlockSeeds) / sizeof(level{level}BlockSeeds[0]);")
        lines.append(f"        return level{level}BlockSeeds;")
    lines.append("    default:")
    lines.append("        *count = 0;")
    lines.append("        return level1BlockSeeds;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("void LoadLevelTileMap(u16 level)")
    lines.append("{")
    lines.append("    const u8 *source;")
    lines.append("    u16 i;")
    lines.append("")
    lines.append("    switch (level) {")
    for level in range(1, 11):
        lines.append(f"    case {level}: source = level{level}TileMap; break;")
    lines.append("    default: source = level1TileMap; break;")
    lines.append("    }")
    lines.append("    for (i = 0; i < sizeof(gActiveTileMap) / sizeof(gActiveTileMap[0]); ++i)")
    lines.append("        gActiveTileMap[i] = source[i];")
    lines.append("}")
    lines.append("")
    lines.append("u8 *GetLevelTileMap(u16 level)")
    lines.append("{")
    lines.append("    (void)level;")
    lines.append("    return gActiveTileMap;")
    lines.append("}")
    lines.append("")
    return lines


def generate(paths: Paths) -> str:
    chunks: list[str] = []
    chunks.append('#include "levels_data.h"')
    chunks.append('#include "levels.h"')
    chunks.append("")

    tiles = read_tiles_map(paths.source_dir / "tiles.map")

    tilemaps: list[tuple[str, list[list[tuple[str, str]]]]] = []
    seed_arrays: list[tuple[str, list[tuple[str, str, str, str, str, str, str]]]] = []

    for level in range(1, 11):
        level_path = paths.source_dir / f"level{level:02d}.level"
        seeds_path = paths.source_dir / f"level{level:02d}.seeds"
        tilemaps.append((f"level{level}TileMap", read_level(level_path, tiles)))
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

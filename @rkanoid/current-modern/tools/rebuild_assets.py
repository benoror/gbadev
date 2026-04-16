#!/usr/bin/env python3

import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROJECT_DIR = ROOT / "project"
BACKUP_DIR = PROJECT_DIR / "source_art"
BITMAP_DIR = ROOT / "bitmaps"
PALETTE_DIR = ROOT / "palettes"

ASSET_MAP = {
    "bg": BACKUP_DIR / "bg.bmp",
    "splash": BACKUP_DIR / "splash.bmp",
    "ending": BACKUP_DIR / "ending.bmp",
    "game_over": BACKUP_DIR / "game_over.bmp",
    "sprites": BACKUP_DIR / "sprites.bmp",
    "title": BACKUP_DIR / "title.bmp",
}


def bmp_to_raw_and_palette(path: Path):
    data = path.read_bytes()

    if data[:2] != b"BM":
        raise ValueError(f"{path} is not a BMP file")

    pixel_offset = struct.unpack_from("<I", data, 10)[0]
    width = struct.unpack_from("<I", data, 18)[0]
    height = struct.unpack_from("<I", data, 22)[0]
    bits_per_pixel = struct.unpack_from("<H", data, 28)[0]

    if bits_per_pixel != 8:
        raise ValueError(f"{path} must be 8bpp, found {bits_per_pixel}")

    row_size = ((width + 3) // 4) * 4
    rows = [data[pixel_offset + y * row_size : pixel_offset + y * row_size + width] for y in range(height)]
    raw = b"".join(reversed(rows))

    palette = bytearray()
    for i in range(256):
        blue, green, red, _ = struct.unpack_from("<BBBB", data, 54 + i * 4)
        gba_color = ((red >> 3) & 31) | (((green >> 3) & 31) << 5) | (((blue >> 3) & 31) << 10)
        palette.extend(struct.pack("<H", gba_color))

    return raw, bytes(palette)


def main():
    BITMAP_DIR.mkdir(exist_ok=True)
    PALETTE_DIR.mkdir(exist_ok=True)

    for asset_name, source_path in ASSET_MAP.items():
        raw, palette = bmp_to_raw_and_palette(source_path)
        (BITMAP_DIR / f"{asset_name}.raw").write_bytes(raw)
        (PALETTE_DIR / f"{asset_name}.pal").write_bytes(palette)
        print(f"rebuilt {asset_name}: raw={len(raw)} palette={len(palette)}")


if __name__ == "__main__":
    main()

# Original toolchain profile

Source of truth: `current/make.bat` and `current/make_mb.bat`

## Expected executables

- `zarmasm`
- `zarmcc`
- `zarmlink`

## Standard ROM build (from `make.bat`)

1. `zarmasm -CPU ARM7TDMI -Littleend start.asm`
2. `zarmasm -CPU ARM7TDMI -Littleend data.asm`
3. `zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst demo.c -o demo.o`
4. `zarmlink -bin -first start.o start.o data.o demo.o -map -ro-base 0x08000000 -rw-base 0x03000000 -o demo.bin`

## MultiBoot build (from `make_mb.bat`)

Same compile chain, with linker differences:

- `-ro-base 0x02000000`
- output: `demo.mb`

Script also hardcodes:

- `C:\agb\Amateur\ARM250\Bin\...`

## Host assumptions captured

- Original scripts are Windows `.bat`.
- Tool binaries likely came from old ARM250 Amateur/AGB-era distribution.
- Build flow assumes legacy assembler linker semantics (`AREA`, `INCBIN`, image symbols from startup/linker script conventions).


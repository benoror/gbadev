# Running the playable showcase

`gbajs3` cannot be run reliably from `file://` URLs because browsers block:

- service worker registration
- manifest loading
- module loading with CORS checks

## Start a local web server

Preferred (adds cross-origin isolation headers for threaded WASM):

```bash
cd /Users/benoror/code/benoror/gbadev/public
python3 serve.py
```

Then open:

- Showcase: `http://127.0.0.1:8000/`
- Emulator direct: `http://127.0.0.1:8000/gbajs3-app/`

## `@rkanoid` ROMs in showcase

Exposed in the showcase UI:

- `public/roms/@rkanoid LATEST - DEMO.gba`
- `public/roms/@rkanoid FAITHFUL - COMPARISON.gba`
- `public/roms/@rkanoid MODERN - RECOMPILED.gba`

Default embedded iframe loads:

- `public/roms/@rkanoid FAITHFUL - COMPARISON.gba`

Faithful comparison ROM source:

- `@rkanoid/build/faithful/work/rkanoid-faithful.bin`

Modern recompiled ROM source:

- `@rkanoid/current-modern/rkanoid-modern.bin`

Fallback (works for non-threaded paths only):

From repo root:

```bash
cd /Users/benoror/code/benoror/gbadev
python3 -m http.server 8000
```

Then open:

- Showcase: `http://localhost:8000/public/`
- Emulator direct: `http://localhost:8000/public/gbajs3-app/`

Reference project:

- [thenick775/gbajs3](https://github.com/thenick775/gbajs3)
- Staging demo URL mentioned by maintainer: [thenick775.github.io/gbajs3](https://thenick775.github.io/gbajs3/)


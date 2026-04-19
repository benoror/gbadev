# Playable showcase

`public/` is a fully static bundle: a tiny showcase page (`index.html`) and a minimal
vanilla-JS player (`player/`) built directly on [thenick775/mgba](https://github.com/thenick775/mgba)’s
WASM core. No build step, no React/MUI, no custom server — just files.

## Quick start

The mGBA WASM core uses threads (SharedArrayBuffer), which requires cross-origin
isolation. `player/coi-sw.js` is a drop-in service worker that injects the required
COOP/COEP headers on second load, so **any static HTTP server on localhost works**:

```bash
cd /Users/benoror/code/benoror/gbadev/public
python3 -m http.server 8000
# or: npx serve .
# or: any static host — works on GitHub Pages, Netlify, etc.
```

Then open:

- Showcase: <http://127.0.0.1:8000/>
- Player direct: <http://127.0.0.1:8000/player/>
- Player with an explicit ROM: <http://127.0.0.1:8000/player/?romURL=/roms/@rkanoid%20MODERN%20-%20RECOMPILED.gba>

> First visit registers the service worker and reloads once; after that cross-origin
> isolation is active and everything just works.

## Why not `file://`?

Threaded WASM needs a service worker (or COOP/COEP response headers). Browsers
disallow both from `file://`, so double-clicking `index.html` won’t work with this
mGBA build. Producing a **non-threaded** mGBA WASM that *would* run from `file://`
is tracked as a followup (requires an upstream emscripten rebuild without
`-pthread`).

## Features in `player/`

- Keyboard: Arrows · <kbd>Z</kbd>=B · <kbd>X</kbd>=A · <kbd>A</kbd>=L · <kbd>S</kbd>=R ·
  <kbd>Enter</kbd>=Start · <kbd>Backspace</kbd>=Select · hold <kbd>Tab</kbd> = fast-forward (5×).
- On-screen touch d-pad + A/B + L/R + Start/Select on coarse-pointer devices.
- Pause / Resume, Soft Reset (Quick Reload), Fullscreen toggle.
- 9 save-state slots (Save / Load State).
- Screenshot → downloads PNG.
- Volume slider (persisted to `localStorage`).

## Bundled ROMs

`public/roms/` ships the @rkanoid builds and a couple of demo ROMs:

- `public/roms/@rkanoid LATEST - DEMO.gba`
- `public/roms/@rkanoid FAITHFUL - COMPARISON.gba`
- `public/roms/@rkanoid MODERN - RECOMPILED.gba`
- `public/roms/cebelix.gba`
- `public/roms/dezgex.gba`

The default embedded iframe on the showcase loads the Faithful Comparison ROM.
The Modern Recompiled ROM is kept in sync by `make publish` in
`@rkanoid/current-modern/`.

## Player layout

```
public/
├── coi-sw.js    # COOP/COEP service worker at /, scope=/ so it covers both pages below
├── index.html   # showcase (links + embedded iframe)
└── player/
    ├── index.html   # minimal UI (canvas + buttons + touchpad)
    ├── app.js       # vanilla JS wrapper around mGBA (boot, input, save-state, etc.)
    ├── app.css
    ├── mgba.js      # mGBA WASM loader (from @thenick775/mgba-wasm)
    └── mgba.wasm    # mGBA core (~2 MB, threaded)
```

> `coi-sw.js` MUST sit at the repo root: the service worker's scope is the directory where
> the file lives, and iframes inherit their parent's isolation state. Placing it at `/player/`
> would only isolate the player page — the showcase iframe would still be blocked because
> its parent document (`/index.html`) wouldn't be isolated.

## Updating the mGBA core

Copy the two files from the upstream npm tarball:

```
@thenick775/mgba-wasm/dist/mgba.js    → public/player/mgba.js
@thenick775/mgba-wasm/dist/mgba.wasm  → public/player/mgba.wasm
```

## References

- [thenick775/mgba – feature/wasm branch](https://github.com/thenick775/mgba)
- [@thenick775/mgba-wasm (npm)](https://www.npmjs.com/package/@thenick775/mgba-wasm)
- [coi-serviceworker](https://github.com/gzuidhof/coi-serviceworker)

# GBA Dev

Game Boy Advance Development I did circa. 2001-2003 for the Homebrew scene, when I was a teenager.

After **~25 years** dormant, **vibecoded** the collection back to life — using agentic coding to preserve the original demos as archival objects, recompile them with a modern open-source toolchain, rebuild the **@rkanoid** gameplay loop around fixed-point physics & a real test harness, and wrap everything in a versioned interactive museum showcase served from GitHub Pages.

See [`CHANGELOG.md`](/CHANGELOG.md) for the full preservation → modernization → museum trail.

PLAY NOW: <https://benoror.github.io/gbadev>

![](/public/images/screenshot.png)

## @rkanoid

An arkanoid-clone consisting of 5 levels, 2-player mode, scoring & special items, english translated (options never implemented 🙈)

Still listed in PDRoms from back in the day: https://pdroms.de/files/nintendo-gameboyadvance-gba/arkanoid-v2-8-beta

Original release: [@rkanoid LATEST - DEMO.gba](https://github.com/benoror/gbadev/blob/master/ROMS/@rkanoid%20LATEST%20-%20DEMO.gba?raw=true)

Modern recompile (v3.0.0 — 2026): Q8.8 fixed-point physics, 10 levels, dynamic speed ramp, PSG SFX, host-side test harness — see [`@rkanoid/current-modern/`](/%40rkanoid/current-modern) and [`CHANGELOG.md`](/CHANGELOG.md).

Playable showcase (GitHub Pages): <https://benoror.github.io/gbadev> — versioned cartridge rack from v0.01 → v3.0.0 plus the other Dreamsoft demos below. Source under [`/public/`](/public/).

Run locally via any static HTTP server (required for threaded mGBA WASM; `file://` is blocked):

```bash
cd /Users/benoror/code/benoror/gbadev/public
python3 -m http.server 8000
# then open http://127.0.0.1:8000/
```


![](/public/images/%40rkanoid.gif) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-0.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-1.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-2.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-3.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-5.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-8.png) ![](/public/images/%40rkanoid%20LATEST%20-%20DEMO-4.png) 


## DexGex (formerly Mystic Quest)

A top-view inspired by Pokemon & Zelda games:

Latest release: [mystic_q.gba](https://github.com/benoror/gbadev/blob/master/ROMS/mystic_q.gba?raw=true)

\*Fun note: Was planning to make a library for developing RPGs on the GBA: [grpglib.txt](https://github.com/benoror/gbadev/blob/master/dreamsoft.es.fm/grpglib.txt)

![](/public/images/dezgex-0.png) ![](/public/images/dexgex.gif)

## Cebelix

A yet-to-be platformer mainly to test high-res(\*) images inspired by pseudo-3d from Donkey Kong series.

Latest release: [cebelix.gba](https://github.com/benoror/gbadev/blob/master/ROMS/cebelix.gba?raw=true)

![](/public/images/cebelix.gif)

---

## License

This project (source, build scripts, museum showcase, and the modernized `@rkanoid` rebuild) is released under the [MIT License](/LICENSE) — © 2001-2026 Ben Orozco. Use it, fork it, port it, learn from it.

The original Ducker / Dreamsoft demo ROMs under [`public/roms/archive/`](/public/roms/archive) are preserved here as historical freeware artifacts (they were originally distributed as free homebrew via PDRoms and the now-defunct `dreamsoft.es.fm`); they retain their original status and are not relicensed.

## Attribution

This project stands on the shoulders of excellent open-source work:

- **[mGBA WASM](https://github.com/thenick775/mgba)** — Nick Wood's WebAssembly fork of [mGBA](https://github.com/mgba-emu/mgba) (Mozilla Public License 2.0). Bundled as `public/player/mgba.{js,wasm}` and powers every emulator frame in the museum showcase.
- **[coi-serviceworker](https://github.com/gzuidhof/coi-serviceworker)** — Guido Zuidhof's COOP/COEP service worker (MIT). Bundled as `public/coi-sw.js` to enable cross-origin isolation (a hard requirement for threaded `SharedArrayBuffer`-based WASM).
- **[devkitARM](https://devkitpro.org/)** / **[Arm GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)** — bare-metal ARM toolchains used to build the modern `@rkanoid` ROM (not bundled).
- The original `@rkanoid` was built on top of **[HAM SDK](https://www.ngine.de/site.cgi?action=ham)** (Hambo's GBA dev kit) and assets from the **PDRoms / GBADev** community of the early 2000s.

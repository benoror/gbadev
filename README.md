# GBA Dev

Some Game Boy Advance Development I did circa. 2001-2003 for the Homebrew scene

## @rkanoid

An arkanoid-clone consisting of 5 levels, 2-player mode, scoring & special items, english translated (options never implemented 🙈)

Still listed in PDRoms from back in the day: https://pdroms.de/files/nintendo-gameboyadvance-gba/arkanoid-v2-8-beta

Latest release: [@rkanoid LATEST - DEMO.gba](https://github.com/benoror/gbadev/blob/master/ROMS/@rkanoid%20LATEST%20-%20DEMO.gba?raw=true)

Playable showcase: [/public/index.html](/public/index.html)

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

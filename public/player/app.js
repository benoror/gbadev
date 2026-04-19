/*
 * Minimal vanilla-JS GBA player around thenick775's mGBA WASM core.
 * API reference: https://github.com/thenick775/mgba (feature/wasm branch).
 *
 * Flow:
 *   1. coi-sw.js registers a service worker that injects COOP/COEP headers on subsequent loads
 *      so threaded mGBA can use SharedArrayBuffer on any static host.
 *   2. mgba.js is an ES module with `export default mGBA`; loaded via dynamic import so we
 *      can show a friendly status message if it fails (e.g. COI not yet applied on first load).
 *   3. `?romURL=<absolute-or-relative-url>` selects which ROM to fetch and launch.
 */

import mGBA from './mgba.js';

(function () {
  'use strict';

  /* ---------- DOM ---------- */
  const $ = (sel) => document.querySelector(sel);
  const canvas = $('#screen');
  const statusEl = $('#status');
  const pauseBtn = $('#btn-pause');
  const resetBtn = $('#btn-reset');
  const shotBtn = $('#btn-shot');
  const fullBtn = $('#btn-full');
  const saveBtn = $('#btn-save');
  const loadBtn = $('#btn-load');
  const slotSel = $('#state-slot');
  const volumeEl = $('#volume');

  /* ---------- State ---------- */
  let Module = null;
  let isPaused = false;
  let romFileName = null;
  let ffMultiplier = 1;

  function setStatus(text, kind) {
    if (!text) {
      statusEl.classList.add('hidden');
      return;
    }
    statusEl.textContent = text;
    statusEl.classList.remove('hidden');
    statusEl.classList.toggle('error', kind === 'error');
  }

  /* ---------- Boot ---------- */
  async function boot() {
    if (typeof mGBA !== 'function') {
      setStatus('mGBA core failed to load', 'error');
      return;
    }

    if (!self.crossOriginIsolated) {
      setStatus('Waiting for cross-origin isolation… (first load reloads once)');
    }

    try {
      Module = await mGBA({ canvas });
      const { projectName, projectVersion } = Module.version || {};
      setStatus(`${projectName || 'mGBA'} ${projectVersion || ''} ready`);
      Module.FSInit();
      // Ensure any persisted save files from IDBFS are available before we load.
      if (typeof Module.FSSync === 'function') {
        try { Module.FSSync(); } catch (e) { /* first run, no IDB yet */ }
      }

      wireControls();
      wireKeyboard();
      wireTouch();
      wireLifecycle();

      applyInitialVolume();

      const romURL = new URLSearchParams(location.search).get('romURL');
      if (romURL) {
        await startRomFromUrl(romURL);
      } else {
        setStatus('Open the showcase page and pick a ROM to play.');
      }
    } catch (err) {
      console.error(err);
      setStatus('Failed to initialize mGBA: ' + (err && err.message ? err.message : err), 'error');
    }
  }

  /* ---------- ROM loading ---------- */
  async function startRomFromUrl(romURL) {
    setStatus('Loading ROM…');
    const absoluteUrl = new URL(romURL, location.href);
    const res = await fetch(absoluteUrl.href);
    if (!res.ok) throw new Error(`Fetch failed (${res.status})`);

    const blob = await res.blob();
    const fileName = decodeURIComponent(absoluteUrl.pathname.split('/').pop() || 'game.gba');
    romFileName = fileName;

    const file = new File([blob], fileName, { type: 'application/octet-stream' });

    await new Promise((resolve, reject) => {
      try {
        Module.uploadRom(file, () => resolve());
      } catch (e) {
        reject(e);
      }
    });

    Module.loadGame(fileName);

    // Restore auto-save state (power-off resume) if one exists.
    try {
      if (typeof Module.loadAutoSaveState === 'function') Module.loadAutoSaveState();
    } catch (_) { /* ignore missing autosave */ }

    setStatus('');
  }

  /* ---------- Controls ---------- */
  function wireControls() {
    pauseBtn.addEventListener('click', togglePause);
    resetBtn.addEventListener('click', () => {
      safe(() => Module.quickReload && Module.quickReload());
    });
    shotBtn.addEventListener('click', takeScreenshot);
    fullBtn.addEventListener('click', toggleFullscreen);
    saveBtn.addEventListener('click', () => safe(() => Module.saveState && Module.saveState(Number(slotSel.value))));
    loadBtn.addEventListener('click', () => safe(() => Module.loadState && Module.loadState(Number(slotSel.value))));
    volumeEl.addEventListener('input', () => {
      safe(() => Module.setVolume && Module.setVolume(Number(volumeEl.value)));
      try { localStorage.setItem('player.volume', volumeEl.value); } catch (_) { /* no-op */ }
    });
  }

  function applyInitialVolume() {
    const stored = (() => { try { return localStorage.getItem('player.volume'); } catch (_) { return null; } })();
    if (stored != null) volumeEl.value = stored;
    safe(() => Module.setVolume && Module.setVolume(Number(volumeEl.value)));
  }

  function togglePause() {
    if (!Module) return;
    if (isPaused) {
      safe(() => Module.resumeGame && Module.resumeGame());
      safe(() => Module.resumeAudio && Module.resumeAudio());
      pauseBtn.textContent = 'Pause';
    } else {
      safe(() => Module.pauseGame && Module.pauseGame());
      safe(() => Module.pauseAudio && Module.pauseAudio());
      pauseBtn.textContent = 'Resume';
    }
    isPaused = !isPaused;
  }

  async function takeScreenshot() {
    if (!Module) return;
    const fileName = `screenshot-${Date.now()}.png`;
    try {
      Module.screenshot(fileName);
      if (Module.FS && Module.FS.readFile) {
        const data = Module.FS.readFile('/data/screenshots/' + fileName);
        downloadBlob(new Blob([data], { type: 'image/png' }), fileName);
        return;
      }
    } catch (e) {
      console.warn('mGBA screenshot() path not found, falling back to canvas', e);
    }
    // Canvas fallback (no audio muting, but always works).
    canvas.toBlob((blob) => { if (blob) downloadBlob(blob, fileName); }, 'image/png');
  }

  function downloadBlob(blob, fileName) {
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = fileName;
    document.body.appendChild(a);
    a.click();
    a.remove();
    setTimeout(() => URL.revokeObjectURL(url), 1000);
  }

  function toggleFullscreen() {
    const el = document.documentElement;
    if (!document.fullscreenElement) {
      (el.requestFullscreen || el.webkitRequestFullscreen || (() => {})).call(el);
    } else {
      (document.exitFullscreen || document.webkitExitFullscreen || (() => {})).call(document);
    }
  }

  /* ---------- Keyboard ---------- */
  // Gbajs3-equivalent default mapping. Names match the mGBA buttonPress/buttonUnpress contract.
  const KEY_MAP = {
    ArrowUp: 'Up',
    ArrowDown: 'Down',
    ArrowLeft: 'Left',
    ArrowRight: 'Right',
    KeyZ: 'B',
    KeyX: 'A',
    KeyA: 'L',
    KeyS: 'R',
    Enter: 'Start',
    Backspace: 'Select'
  };

  function wireKeyboard() {
    addEventListener('keydown', (e) => {
      if (e.repeat) return;
      if (e.code === 'Tab') {
        e.preventDefault();
        if (ffMultiplier === 1) {
          ffMultiplier = 5;
          safe(() => Module.setFastForwardMultiplier && Module.setFastForwardMultiplier(ffMultiplier));
        }
        return;
      }
      const btn = KEY_MAP[e.code];
      if (btn) {
        e.preventDefault();
        safe(() => Module.buttonPress && Module.buttonPress(btn));
      }
    });

    addEventListener('keyup', (e) => {
      if (e.code === 'Tab') {
        e.preventDefault();
        if (ffMultiplier !== 1) {
          ffMultiplier = 1;
          safe(() => Module.setFastForwardMultiplier && Module.setFastForwardMultiplier(ffMultiplier));
        }
        return;
      }
      const btn = KEY_MAP[e.code];
      if (btn) {
        e.preventDefault();
        safe(() => Module.buttonUnpress && Module.buttonUnpress(btn));
      }
    });

    // Prevent lingering presses on window blur.
    addEventListener('blur', () => {
      if (!Module) return;
      Object.values(KEY_MAP).forEach((b) => safe(() => Module.buttonUnpress && Module.buttonUnpress(b)));
      if (ffMultiplier !== 1) {
        ffMultiplier = 1;
        safe(() => Module.setFastForwardMultiplier && Module.setFastForwardMultiplier(1));
      }
    });
  }

  /* ---------- Touch pad ---------- */
  function wireTouch() {
    document.querySelectorAll('[data-btn]').forEach((el) => {
      const btn = el.getAttribute('data-btn');
      const down = (e) => { e.preventDefault(); safe(() => Module && Module.buttonPress && Module.buttonPress(btn)); };
      const up = (e) => { e.preventDefault(); safe(() => Module && Module.buttonUnpress && Module.buttonUnpress(btn)); };
      el.addEventListener('touchstart', down, { passive: false });
      el.addEventListener('touchend', up, { passive: false });
      el.addEventListener('touchcancel', up, { passive: false });
      el.addEventListener('mousedown', down);
      el.addEventListener('mouseup', up);
      el.addEventListener('mouseleave', up);
    });
  }

  /* ---------- Lifecycle: auto-save on unload, FS sync, visibility ---------- */
  function wireLifecycle() {
    const persist = () => {
      if (!Module) return;
      safe(() => Module.forceAutoSaveState && Module.forceAutoSaveState());
      safe(() => Module.FSSync && Module.FSSync());
    };

    addEventListener('pagehide', persist);
    addEventListener('beforeunload', persist);

    document.addEventListener('visibilitychange', () => {
      if (!Module) return;
      if (document.hidden) {
        safe(() => Module.pauseAudio && Module.pauseAudio());
        persist();
      } else if (!isPaused) {
        safe(() => Module.resumeAudio && Module.resumeAudio());
      }
    });
  }

  /* ---------- Utilities ---------- */
  function safe(fn) { try { return fn(); } catch (err) { console.warn(err); } }

  boot();
})();

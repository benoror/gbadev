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
  let romGamePath = null;
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

    // Threaded mGBA requires SharedArrayBuffer, which requires cross-origin isolation.
    // coi-sw.js normally reloads the page once after registering the service worker, but on
    // the very first visit there's a race where app.js runs before the worker is controlling.
    // In that case we wait for a controller → reload ourselves; if that also fails (e.g. a
    // private browsing session where service workers are blocked), show a helpful error.
    if (!self.crossOriginIsolated) {
      await ensureCrossOriginIsolationOrReload();
      return;
    }

    try {
      Module = await mGBA({ canvas });
      const { projectName, projectVersion } = Module.version || {};
      setStatus(`${projectName || 'mGBA'} ${projectVersion || ''} ready`);
      // FSInit returns a promise: it mounts IDBFS at /data and /autosave, pulls any
      // previously-synced files back from IndexedDB, and creates the standard sub-dirs.
      // Never race it with a manual FSSync() — you'll get "2 FS.syncfs operations in flight".
      await Module.FSInit();

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
    // `cache: 'no-store'` bypasses both the HTTP cache and the disk cache so a freshly-
    // compiled ROM (e.g. `make ship` just ran) is always fetched from the server. Without
    // this, `python3 -m http.server` responses have no `Cache-Control` header and the
    // browser applies a heuristic "fresh" window — you'd see stale frames after a rebuild
    // until a manual hard-reload.
    const res = await fetch(absoluteUrl.href, { cache: 'no-store' });
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

    // Module.uploadRom writes to /data/games/<fileName>. loadGame expects the full VFS
    // path — passing just the base name returns ENOENT and leaves the canvas blank.
    romGamePath = `${(Module.filePaths?.() || {}).gamePath || '/data/games'}/${fileName}`;
    const ok = Module.loadGame(romGamePath);
    if (!ok) {
      setStatus(`Failed to load ROM: ${fileName}`, 'error');
      return;
    }
    // Clear pause state so the just-loaded game is actually running.
    if (isPaused) { isPaused = false; pauseBtn.textContent = 'Pause'; }

    // Auto-resume on ROM open: disabled for now. Uncomment to restore the last session
    // (paired with the persist() hook below on pagehide/beforeunload).
    // try {
    //   if (typeof Module.loadAutoSaveState === 'function') Module.loadAutoSaveState();
    // } catch (_) { /* ignore missing autosave */ }

    setStatus('');
  }

  /* ---------- Controls ---------- */
  function wireControls() {
    pauseBtn.addEventListener('click', togglePause);
    resetBtn.addEventListener('click', hardReset);
    shotBtn.addEventListener('click', takeScreenshot);
    fullBtn.addEventListener('click', toggleFullscreen);
    saveBtn.addEventListener('click', saveToSelectedSlot);
    loadBtn.addEventListener('click', loadFromSelectedSlot);
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

  /*
   * Hard reset — emulates a physical power-cycle.
   *
   * mGBA keeps an auto-save state at `/autosave/<game>_auto.ss` (the /autosave mount is
   * `autoPersist: true`, so IndexedDB silently writes/reads it). `loadGame()` restores
   * from that auto-save, which is why the game appears to come back to "last saved state"
   * after a naive reload. To mimic pulling the battery out we:
   *   1. `quitGame()` — tear down the running core.
   *   2. Delete the auto-save state file (and the in-memory cache `Module.autoSaveStateName`).
   *   3. `loadGame()` — attach and boot from the cart header.
   * Battery save (`.sav` in /data/saves) is preserved on purpose — that's cartridge SRAM
   * and real hardware keeps it across power cycles.
   */
  function hardReset() {
    if (!Module || !romGamePath) return;

    if (isPaused) {
      safe(() => Module.resumeGame && Module.resumeGame());
      safe(() => Module.resumeAudio && Module.resumeAudio());
      isPaused = false;
      pauseBtn.textContent = 'Pause';
    }

    safe(() => Module.quitGame && Module.quitGame());

    // Wipe auto-save state so loadGame boots cleanly.
    const autoPath = Module.autoSaveStateName;
    if (autoPath && Module.FS && Module.FS.analyzePath) {
      safe(() => {
        if (Module.FS.analyzePath(autoPath).exists) Module.FS.unlink(autoPath);
      });
    }
    // And flush the deletion so a subsequent reload doesn't resurrect the file from IDB.
    safe(() => Module.FSSync && Module.FSSync());

    safe(() => Module.loadGame(romGamePath));
  }

  function saveToSelectedSlot() {
    if (!Module) return;
    const slot = Number(slotSel.value);
    let ok = false;
    safe(() => { ok = !!(Module.saveState && Module.saveState(slot)); });
    // Flush the FS so the state file actually hits IndexedDB (not lost if the tab is closed).
    safe(() => Module.FSSync && Module.FSSync());
    alert(ok ? `State saved to slot ${slot}` : `Failed to save state to slot ${slot}`);
  }

  function loadFromSelectedSlot() {
    if (!Module) return;
    const slot = Number(slotSel.value);
    let ok = false;
    safe(() => { ok = !!(Module.loadState && Module.loadState(slot)); });
    alert(ok ? `Loaded state from slot ${slot}` : `No state present in slot ${slot}`);
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

  /* ---------- Lifecycle: FS sync, visibility ----------
   *
   * Auto-save state on page unload is disabled for now — flipping `PERSIST_AUTOSAVE_ON_UNLOAD`
   * to true (and uncommenting the `loadAutoSaveState()` call in `startRomFromUrl`) re-enables
   * the gbajs3-style session resume. Left wired so the restore path keeps pausing audio and
   * syncing the filesystem to IDB when the tab goes to the background / is hidden.
   */
  const PERSIST_AUTOSAVE_ON_UNLOAD = false;

  function wireLifecycle() {
    const persist = () => {
      if (!Module) return;
      if (PERSIST_AUTOSAVE_ON_UNLOAD) {
        safe(() => Module.forceAutoSaveState && Module.forceAutoSaveState());
      }
      safe(() => Module.FSSync && Module.FSSync());
    };

    if (PERSIST_AUTOSAVE_ON_UNLOAD) {
      addEventListener('pagehide', persist);
      addEventListener('beforeunload', persist);
    }

    document.addEventListener('visibilitychange', () => {
      if (!Module) return;
      if (document.hidden) {
        safe(() => Module.pauseAudio && Module.pauseAudio());
        // Keep FS in sync so save states written while visible aren't lost if the tab is killed.
        safe(() => Module.FSSync && Module.FSSync());
      } else if (!isPaused) {
        safe(() => Module.resumeAudio && Module.resumeAudio());
      }
    });
  }

  /* ---------- Utilities ---------- */
  function safe(fn) { try { return fn(); } catch (err) { console.warn(err); } }

  const COI_RELOAD_KEY = 'player.coiReload';

  async function ensureCrossOriginIsolationOrReload() {
    // Safety net: if we already tried reloading once and still aren't isolated, don't loop forever.
    const attempts = Number(sessionStorage.getItem(COI_RELOAD_KEY) || '0');
    if (attempts >= 2) {
      setStatus(
        'Cross-origin isolation could not be enabled. The COI service worker might be blocked ' +
          '(private browsing, extensions, or a cross-origin server). Try a normal browser window ' +
          'or serve with COOP/COEP response headers.',
        'error'
      );
      return;
    }

    setStatus('Enabling cross-origin isolation… the page will reload once.');

    if (!('serviceWorker' in navigator)) {
      setStatus(
        'Cross-origin isolation unavailable: service workers are disabled in this browser ' +
          '(e.g. private mode). Use a normal browser window or a server sending COOP/COEP headers.',
        'error'
      );
      return;
    }

    // Wait briefly for coi-sw.js to take control and reload the page itself. If nothing
    // happens within the grace period, force a reload ourselves.
    const start = Date.now();
    while (Date.now() - start < 2500) {
      if (self.crossOriginIsolated) {
        sessionStorage.setItem(COI_RELOAD_KEY, String(attempts + 1));
        location.reload();
        return;
      }
      await new Promise((r) => setTimeout(r, 100));
    }

    try { await navigator.serviceWorker.ready; } catch (_) { /* fall through */ }
    sessionStorage.setItem(COI_RELOAD_KEY, String(attempts + 1));
    location.reload();
  }

  // Clear the reload-attempts counter once we've actually booted successfully.
  addEventListener('load', () => {
    if (self.crossOriginIsolated) sessionStorage.removeItem(COI_RELOAD_KEY);
  });

  boot();
})();

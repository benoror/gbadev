/*
 * Reusable mGBA player core, shared by:
 *   • the museum shell (`/index.html` → `museum.js` mounts a player on the GBA SVG's LCD canvas)
 *   • the standalone page (`/player/index.html` → `app.js` renders only the canvas + touchpad)
 *
 * Both hosts call `createPlayer({ canvas, status? })` to get a Promise<API>. A single mGBA
 * Module instance lives for the entire page session; switching ROMs is a hot swap via
 * `loadRomFromUrl(url)` (no full reload, no second WASM init). When the page that hosts
 * the player isn't yet cross-origin isolated, the function gates on coi-sw.js taking over
 * and reloads the page once — so callers never see a non-isolated player.
 */

import mGBA from './mgba.js';

const COI_RELOAD_KEY = 'player.coiReload';
const KEY_MAP = {
  ArrowUp: 'Up', ArrowDown: 'Down', ArrowLeft: 'Left', ArrowRight: 'Right',
  KeyZ: 'B', KeyX: 'A', KeyA: 'L', KeyS: 'R',
  Enter: 'Start', Backspace: 'Select'
};

export async function createPlayer(opts) {
  const canvas = opts.canvas;
  const statusEl = opts.status || null;

  if (!canvas) throw new Error('createPlayer({ canvas }) is required');

  /* ---------- Tiny status helper ---------- */
  function setStatus(text, kind) {
    if (!statusEl) return;
    if (!text) {
      statusEl.classList.add('hidden');
      return;
    }
    statusEl.textContent = text;
    statusEl.classList.remove('hidden');
    statusEl.classList.toggle('error', kind === 'error');
  }

  /* ---------- Cross-origin isolation (one-shot reload) ---------- */
  if (!self.crossOriginIsolated) {
    setStatus('Enabling cross-origin isolation… the page will reload once.');
    const ok = await waitOrReload();
    if (!ok) {
      setStatus(
        'Cross-origin isolation could not be enabled. The COI service worker might be blocked ' +
          '(private browsing, extensions, or a cross-origin host).',
        'error'
      );
      return null;
    }
  }

  /* ---------- Boot mGBA ---------- */
  setStatus('Booting mGBA…');
  const Module = await mGBA({ canvas });
  const { projectName, projectVersion } = Module.version || {};
  setStatus(`${projectName || 'mGBA'} ${projectVersion || ''} ready`);
  await Module.FSInit();

  /* ---------- Live state ---------- */
  let isPaused = false;
  let romGamePath = null;
  let ffMultiplier = 1;
  const pauseListeners = new Set(); /* notified with `paused: bool` whenever it changes */
  const readyCallbacks = [];

  applyInitialVolume();
  flushReady();

  /* ---------- ROM loading (hot-swap on subsequent calls) ---------- */
  async function loadRomFromUrl(romURL) {
    if (!romURL) return false;
    setStatus('Loading ROM…');
    const absolute = new URL(romURL, location.href).href;
    const res = await fetch(absolute, { cache: 'no-store' });
    if (!res.ok) {
      setStatus(`Fetch failed (${res.status})`, 'error');
      return false;
    }
    const blob = await res.blob();
    const fileName = decodeURIComponent(new URL(absolute).pathname.split('/').pop() || 'game.gba');
    const file = new File([blob], fileName, { type: 'application/octet-stream' });

    /* If a ROM was already loaded, tear it down explicitly so the new one boots from cart
     * header instead of inheriting RAM state from the previous title. Same trick as
     * `hardReset()` below — the autosave file would otherwise resurrect the previous run. */
    if (romGamePath) {
      safe(() => Module.quitGame && Module.quitGame());
    }

    await new Promise((resolve, reject) => {
      try { Module.uploadRom(file, () => resolve()); }
      catch (e) { reject(e); }
    });

    romGamePath = `${(Module.filePaths?.() || {}).gamePath || '/data/games'}/${fileName}`;
    const ok = Module.loadGame(romGamePath);
    if (!ok) { setStatus(`Failed to load ROM: ${fileName}`, 'error'); return false; }

    if (isPaused) setPaused(false);
    setStatus('');
    return true;
  }

  /* ---------- Pause / Resume ---------- */
  function setPaused(next) {
    if (next === isPaused) return;
    if (next) {
      safe(() => Module.pauseGame && Module.pauseGame());
      safe(() => Module.pauseAudio && Module.pauseAudio());
    } else {
      safe(() => Module.resumeGame && Module.resumeGame());
      safe(() => Module.resumeAudio && Module.resumeAudio());
    }
    isPaused = next;
    pauseListeners.forEach((fn) => safe(() => fn(isPaused)));
  }

  /* ---------- Hard reset (power-cycle equivalent) ---------- */
  function hardReset() {
    if (!romGamePath) return;
    if (isPaused) setPaused(false);
    safe(() => Module.quitGame && Module.quitGame());
    /* Wipe the auto-save state file so loadGame boots from the cart header. */
    const autoPath = Module.autoSaveStateName;
    if (autoPath && Module.FS?.analyzePath) {
      safe(() => { if (Module.FS.analyzePath(autoPath).exists) Module.FS.unlink(autoPath); });
    }
    safe(() => Module.FSSync && Module.FSSync());
    safe(() => Module.loadGame(romGamePath));
  }

  /* ---------- Save / Load state slots ---------- */
  function saveStateToSlot(slot) {
    let ok = false;
    safe(() => { ok = !!(Module.saveState && Module.saveState(Number(slot))); });
    safe(() => Module.FSSync && Module.FSSync());
    alert(ok ? `State saved to slot ${slot}` : `Failed to save state to slot ${slot}`);
    return ok;
  }
  function loadStateFromSlot(slot) {
    let ok = false;
    safe(() => { ok = !!(Module.loadState && Module.loadState(Number(slot))); });
    alert(ok ? `Loaded state from slot ${slot}` : `No state present in slot ${slot}`);
    return ok;
  }

  /* ---------- Screenshot ---------- */
  function takeScreenshot() {
    const fileName = `screenshot-${Date.now()}.png`;
    try {
      Module.screenshot(fileName);
      if (Module.FS?.readFile) {
        const data = Module.FS.readFile('/data/screenshots/' + fileName);
        downloadBlob(new Blob([data], { type: 'image/png' }), fileName);
        return;
      }
    } catch (e) { console.warn('mGBA screenshot path missing, falling back to canvas', e); }
    canvas.toBlob((blob) => { if (blob) downloadBlob(blob, fileName); }, 'image/png');
  }

  /* ---------- Volume (persisted in localStorage) ---------- */
  function setVolume(value, { persist = true } = {}) {
    value = Math.max(0, Math.min(100, Number(value)));
    safe(() => Module.setVolume && Module.setVolume(value));
    if (persist) {
      try { localStorage.setItem('player.volume', String(value)); } catch (_) { /* no-op */ }
    }
    return value;
  }
  function getVolume() {
    try {
      const stored = localStorage.getItem('player.volume');
      return stored != null ? Number(stored) : 70;
    } catch (_) { return 70; }
  }
  function applyInitialVolume() { setVolume(getVolume(), { persist: false }); }

  /* ---------- Keyboard binding (target = window by default) ---------- */
  function attachKeyboard(target) {
    target = target || window;
    target.addEventListener('keydown', onKeyDown);
    target.addEventListener('keyup',   onKeyUp);
    target.addEventListener('blur',    onBlur);

    function onKeyDown(e) {
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
      if (btn) { e.preventDefault(); safe(() => Module.buttonPress && Module.buttonPress(btn)); }
    }
    function onKeyUp(e) {
      if (e.code === 'Tab') {
        e.preventDefault();
        if (ffMultiplier !== 1) {
          ffMultiplier = 1;
          safe(() => Module.setFastForwardMultiplier && Module.setFastForwardMultiplier(1));
        }
        return;
      }
      const btn = KEY_MAP[e.code];
      if (btn) { e.preventDefault(); safe(() => Module.buttonUnpress && Module.buttonUnpress(btn)); }
    }
    function onBlur() {
      Object.values(KEY_MAP).forEach((b) => safe(() => Module.buttonUnpress && Module.buttonUnpress(b)));
      if (ffMultiplier !== 1) {
        ffMultiplier = 1;
        safe(() => Module.setFastForwardMultiplier && Module.setFastForwardMultiplier(1));
      }
    }

    return () => {
      target.removeEventListener('keydown', onKeyDown);
      target.removeEventListener('keyup',   onKeyUp);
      target.removeEventListener('blur',    onBlur);
    };
  }

  /* ---------- Touch pad: any element with [data-btn] inside `root` ---------- */
  function attachTouch(root) {
    root = root || document;
    root.querySelectorAll('[data-btn]').forEach((el) => {
      const btn = el.getAttribute('data-btn');
      const down = (e) => { e.preventDefault(); safe(() => Module.buttonPress?.(btn)); };
      const up   = (e) => { e.preventDefault(); safe(() => Module.buttonUnpress?.(btn)); };
      el.addEventListener('touchstart', down, { passive: false });
      el.addEventListener('touchend',   up,   { passive: false });
      el.addEventListener('touchcancel',up,   { passive: false });
      el.addEventListener('mousedown',  down);
      el.addEventListener('mouseup',    up);
      el.addEventListener('mouseleave', up);
    });
  }

  /* ---------- Lifecycle: pause audio when tab hidden ---------- */
  document.addEventListener('visibilitychange', () => {
    if (document.hidden) {
      safe(() => Module.pauseAudio && Module.pauseAudio());
      safe(() => Module.FSSync && Module.FSSync());
    } else if (!isPaused) {
      safe(() => Module.resumeAudio && Module.resumeAudio());
    }
  });

  /* ---------- Public API ---------- */
  return {
    /* Introspection */
    Module,
    isReady: () => true,
    isPaused: () => isPaused,
    onPauseChange: (cb) => { pauseListeners.add(cb); return () => pauseListeners.delete(cb); },

    /* Actions */
    pause:        () => setPaused(true),
    resume:       () => setPaused(false),
    togglePause:  () => setPaused(!isPaused),
    reset:        hardReset,
    loadRomFromUrl,
    saveState:    saveStateToSlot,
    loadState:    loadStateFromSlot,
    screenshot:   takeScreenshot,
    setVolume,
    getVolume,

    /* Input wiring (caller decides scope) */
    attachKeyboard,
    attachTouch,

    /* Boot synchronization */
    onReady(cb) { if (Module) cb(); else readyCallbacks.push(cb); },
  };

  /* ---------- internals ---------- */
  function flushReady() { while (readyCallbacks.length) safe(readyCallbacks.shift()); }

  async function waitOrReload() {
    const attempts = Number(sessionStorage.getItem(COI_RELOAD_KEY) || '0');
    if (attempts >= 2) return false;
    if (!('serviceWorker' in navigator)) return false;
    const start = Date.now();
    while (Date.now() - start < 2500) {
      if (self.crossOriginIsolated) {
        sessionStorage.setItem(COI_RELOAD_KEY, String(attempts + 1));
        location.reload();
        return false;
      }
      await new Promise((r) => setTimeout(r, 100));
    }
    try { await navigator.serviceWorker.ready; } catch (_) { /* no-op */ }
    sessionStorage.setItem(COI_RELOAD_KEY, String(attempts + 1));
    location.reload();
    return false;
  }
}

function safe(fn) { try { return fn?.(); } catch (err) { console.warn(err); } }

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

/* Reset the COI reload counter once a successful navigation lands isolated. */
addEventListener('load', () => {
  if (self.crossOriginIsolated) sessionStorage.removeItem(COI_RELOAD_KEY);
});

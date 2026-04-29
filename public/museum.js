/*
 * GBA Dev Museum — catalog-driven showcase shell.
 *
 *   1. Loads `catalog.json` (games + versions).
 *   2. Renders a cart rack (one tile per game) and a version spine (per selected game).
 *   3. Mounts the shared mGBA player core directly onto the canvas inside the GBA SVG —
 *      no iframe, no full reload on ROM change. Switching versions hot-swaps via
 *      `player.loadRomFromUrl(...)`, which calls `Module.quitGame()` → `loadGame()`.
 *   4. Deep-links: ?game=<id>&v=<versionId>.
 *
 * Shortcuts:
 *   - ← / → on focused cart/version node walks siblings; Enter activates.
 *   - F = fullscreen (whole GBA shell), C = CRT shader, I = notes drawer.
 */

'use strict';

import { createPlayer } from './player/core.js';

const qs = (sel, root = document) => root.querySelector(sel);
const qsa = (sel, root = document) => Array.from(root.querySelectorAll(sel));

const state = {
  games: [],
  gameById: {},
  selected: { gameId: null, versionId: null },
  player: null
};

async function main() {
  const catalog = await loadCatalog();
  state.games = catalog.games;
  state.games.forEach((g) => (state.gameById[g.id] = g));

  renderRack();
  wireGlobalKeys();

  /* Boot mGBA into the canvas embedded in the GBA SVG. createPlayer handles cross-origin
   * isolation (it'll reload the page once on first visit if the SW isn't controlling yet),
   * so by the time we get a non-null player back the WASM is fully booted. */
  const player = await createPlayer({
    canvas: qs('#player-canvas'),
    status: qs('#player-status')
  });
  if (!player) return; /* a COI reload is in flight */
  state.player = player;
  player.attachKeyboard();
  /* Pause label/LED follow the live player state, even if pause came from a keyboard
   * shortcut or visibility change. */
  player.onPauseChange(refreshPauseButton);

  wireStageControls();

  const params = new URLSearchParams(location.search);
  const initGameId = params.get('game') || catalog.games[0].id;
  const game = state.gameById[initGameId] || catalog.games[0];
  const initVersionId = params.get('v') || pickDefaultVersion(game).id;
  selectGame(game.id, { initial: true });
  await selectVersion(initVersionId, { initial: true });

  /* Sync the volume slider with whatever the player picked up from localStorage. */
  qs('#volume').value = String(player.getVolume());
}

async function loadCatalog() {
  const res = await fetch('./catalog.json', { cache: 'no-store' });
  if (!res.ok) throw new Error(`Catalog load failed: ${res.status}`);
  return res.json();
}

/* Prefer MODERN → LATEST → FAITHFUL → last historical → first listed. */
function pickDefaultVersion(game) {
  const order = ['modern', 'latest', 'faithful', 'historical'];
  for (const kind of order) {
    const match = [...game.versions].reverse().find((v) => v.kind === kind);
    if (match) return match;
  }
  return game.versions[0];
}

/* ---------- Rack rendering ---------- */
function renderRack() {
  const rack = qs('#rack');
  rack.innerHTML = '';
  state.games.forEach((game) => {
    const btn = document.createElement('button');
    btn.type = 'button';
    btn.className = 'cart';
    btn.dataset.gameId = game.id;
    btn.style.setProperty('--cart-color', game.cartColor || '#4a51d9');
    btn.style.setProperty('--cart-accent', game.accent || '#ffffff');
    btn.setAttribute('aria-label', `${game.title} — ${game.versions.length} version${game.versions.length === 1 ? '' : 's'}`);
    btn.innerHTML = `
      <span class="cart-badge">${game.versions.length}</span>
      <span class="cart-label">
        <span class="cart-cover" aria-hidden="true"></span>
        <span class="cart-title">${escapeHtml(game.title)}</span>
      </span>
    `;
    btn.addEventListener('click', () => selectGame(game.id));
    rack.appendChild(btn);
  });
}

/* ---------- Spine rendering ---------- */
function renderSpine(game) {
  const spine = qs('#spine');
  const spineWrap = qs('#spine-wrap');
  spineWrap.style.display = '';
  qs('#spine-title').textContent = game.title;
  qs('#spine-count').textContent = `${game.versions.length} version${game.versions.length === 1 ? '' : 's'}`;
  spine.innerHTML = '';

  game.versions.forEach((v) => {
    const node = document.createElement('button');
    node.type = 'button';
    node.className = `version-node kind-${v.kind}`;
    node.dataset.versionId = v.id;
    if (!v.rom) node.disabled = true;
    node.innerHTML = `
      <span class="dot" aria-hidden="true"></span>
      <span class="label">${escapeHtml(v.label)}</span>
      <span class="year">${v.year ?? ''}</span>
    `;
    node.addEventListener('click', () => v.rom && selectVersion(v.id));
    spine.appendChild(node);
  });
}

/* ---------- Selection / state ---------- */
function selectGame(gameId, opts = {}) {
  const game = state.gameById[gameId];
  if (!game) return;
  state.selected.gameId = gameId;

  qsa('.cart', qs('#rack')).forEach((el) => {
    el.classList.toggle('is-selected', el.dataset.gameId === gameId);
  });
  qs('#rack').dataset.hasSelection = 'true';

  renderSpine(game);

  if (!opts.initial) {
    /* When switching games, hot-swap to the default version's ROM. */
    const def = pickDefaultVersion(game);
    selectVersion(def.id);
  }
}

async function selectVersion(versionId, opts = {}) {
  const game = state.gameById[state.selected.gameId];
  if (!game) return;
  const version = game.versions.find((v) => v.id === versionId) || pickDefaultVersion(game);
  state.selected.versionId = version.id;

  qsa('.version-node', qs('#spine')).forEach((el) => {
    el.classList.toggle('is-active', el.dataset.versionId === version.id);
  });

  /* Hot-swap the ROM in the already-running player. The first call (during boot) loads
   * for the first time; subsequent calls `quitGame()` + `loadGame()` without re-init. */
  if (state.player && version.rom) {
    const url = new URL(version.rom, location.href).href;
    await state.player.loadRomFromUrl(url);
  }

  renderNotes(game, version);

  if (!opts.initial) {
    const url = new URL(location.href);
    url.searchParams.set('game', game.id);
    url.searchParams.set('v', version.id);
    history.replaceState(null, '', url);
  }
}

function renderNotes(game, version) {
  qs('#notes-eyebrow').textContent = kindLabel(version.kind);
  qs('#notes-title').textContent = `${game.title} — ${version.label}`;
  qs('#notes-meta').innerHTML = [
    version.year ? `<span><strong>${version.year}</strong></span>` : '',
    game.author ? `<span>by <strong>${escapeHtml(game.author)}</strong></span>` : '',
    version.rom ? `<span>${escapeHtml(basename(version.rom))}</span>` : '<span style="color:#ffb4b4">ROM missing</span>'
  ].filter(Boolean).join('');
  qs('#notes-summary').textContent = version.notes || game.summary || '';

  const links = [];
  if (version.rom) {
    links.push(`<a href="./player/?romURL=${encodeURIComponent(new URL(version.rom, location.href).href)}" target="_blank" rel="noopener">Open in new tab</a>`);
    links.push(`<a href="${encodeURI(version.rom)}" download>Download .gba</a>`);
  }
  if (version.kind === 'modern' && game.id === 'rkanoid') {
    links.push(`<a href="https://github.com/benoror/gbadev/tree/preserve/%40rkanoid/current-modern" target="_blank" rel="noopener">Source</a>`);
  }
  qs('#notes-links').innerHTML = links.join(' · ');
}

function kindLabel(kind) {
  switch (kind) {
    case 'historical': return 'Historical build';
    case 'latest':     return 'Canonical shipped ROM';
    case 'faithful':   return 'Reverse-engineered comparison';
    case 'modern':     return 'Modern rebuild';
    default:           return 'Version';
  }
}

/* ---------- Stage controls ---------- */
function refreshPauseButton(paused) {
  const btn = qs('#btn-pause');
  if (!btn) return;
  paused = paused ?? !!(state.player && state.player.isPaused());
  btn.classList.toggle('is-on', paused);
  const label = btn.querySelector('.label');
  if (label) label.textContent = paused ? 'Resume' : 'Pause';
}

function wireStageControls() {
  const player = state.player;

  qs('#btn-pause').addEventListener('click', () => player.togglePause());
  qs('#btn-reset').addEventListener('click', () => { player.reset(); refreshPauseButton(); });
  qs('#btn-shot') .addEventListener('click', () => player.screenshot());
  qs('#btn-save') .addEventListener('click', () => player.saveState(Number(qs('#state-slot').value)));
  qs('#btn-load') .addEventListener('click', () => player.loadState(Number(qs('#state-slot').value)));

  const volumeEl = qs('#volume');
  volumeEl.addEventListener('input', () => player.setVolume(Number(volumeEl.value)));

  qs('#btn-crt').addEventListener('click', (e) => {
    const wrap = qs('#player-wrap');
    const on = wrap.dataset.crt !== 'true';
    wrap.dataset.crt = on ? 'true' : 'false';
    e.currentTarget.classList.toggle('is-on', on);
  });

  qs('#btn-fullscreen').addEventListener('click', () => {
    /* Fullscreen the whole GBA shell (SVG + canvas inside the LCD). */
    const el = qs('#player-wrap');
    if (!document.fullscreenElement) {
      (el.requestFullscreen || el.webkitRequestFullscreen || (() => {})).call(el);
    } else {
      (document.exitFullscreen || document.webkitExitFullscreen || (() => {})).call(document);
    }
  });
  addEventListener('fullscreenchange', () => {
    qs('#btn-fullscreen').classList.toggle('is-on', !!document.fullscreenElement);
  });

  qs('#btn-info').addEventListener('click', (e) => {
    const notes = qs('#notes');
    const open = notes.style.display !== 'none';
    notes.style.display = open ? 'none' : '';
    e.currentTarget.classList.toggle('is-on', !open);
  });
}

/* ---------- Keyboard navigation ---------- */
function wireGlobalKeys() {
  addEventListener('keydown', (e) => {
    /* Don't intercept while typing in inputs (slot select / volume slider). */
    const tag = (document.activeElement && document.activeElement.tagName) || '';
    if (tag === 'INPUT' || tag === 'SELECT' || tag === 'TEXTAREA') return;

    if (e.key === 'ArrowRight' || e.key === 'ArrowLeft') {
      const dir = e.key === 'ArrowRight' ? 1 : -1;
      if (document.activeElement && document.activeElement.classList.contains('version-node')) {
        e.preventDefault();
        stepVersion(dir);
      } else if (document.activeElement && document.activeElement.classList.contains('cart')) {
        e.preventDefault();
        stepGame(dir);
      }
      return;
    }

    if (e.key.toLowerCase() === 'f') { qs('#btn-fullscreen').click(); }
    else if (e.key.toLowerCase() === 'c') { qs('#btn-crt').click(); }
    else if (e.key.toLowerCase() === 'i') { qs('#btn-info').click(); }
  });
}

function stepGame(dir) {
  const ids = state.games.map((g) => g.id);
  const idx = ids.indexOf(state.selected.gameId);
  const next = ids[(idx + dir + ids.length) % ids.length];
  selectGame(next);
  const btn = qs(`.cart[data-game-id="${next}"]`);
  if (btn) btn.focus();
}

function stepVersion(dir) {
  const game = state.gameById[state.selected.gameId];
  if (!game) return;
  const playable = game.versions.filter((v) => v.rom);
  const ids = playable.map((v) => v.id);
  const idx = ids.indexOf(state.selected.versionId);
  const next = ids[(idx + dir + ids.length) % ids.length];
  selectVersion(next);
  const btn = qs(`.version-node[data-version-id="${next}"]`);
  if (btn) btn.focus();
}

/* ---------- Utils ---------- */
function escapeHtml(s) {
  return String(s).replace(/[&<>"']/g, (c) => (
    { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]
  ));
}

function basename(path) {
  const last = path.split('/').pop() || path;
  return decodeURIComponent(last);
}

main().catch((err) => {
  console.error(err);
  const el = qs('#fatal');
  if (el) { el.textContent = `Failed to load catalog: ${err.message}`; el.style.display = ''; }
});

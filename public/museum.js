/*
 * GBA Dev Museum — catalog-driven showcase shell.
 *
 * The page is a static vanilla JS app:
 *   1. Loads `catalog.json` (games + versions).
 *   2. Renders a cart rack (one tile per game) and a version spine (per selected game).
 *   3. Points the embedded `<iframe src="./player/?romURL=...">` at the chosen ROM, so all
 *      emulator plumbing stays in /player and this file is strictly menu logic.
 *   4. Deep-links: ?game=<id>&v=<versionId> and ?rom=<url>.
 *
 * Accessibility notes:
 *   - Carts and version nodes are actual <button> elements.
 *   - ← / → navigate between sibling carts or versions while focused; Enter plays the selected.
 *   - F = fullscreen, I = info drawer (hidden on small screens), C = CRT shader toggle.
 */

'use strict';

const qs = (sel, root = document) => root.querySelector(sel);
const qsa = (sel, root = document) => Array.from(root.querySelectorAll(sel));

const state = {
  games: [],
  gameById: {},
  selected: { gameId: null, versionId: null }
};

async function main() {
  const catalog = await loadCatalog();
  state.games = catalog.games;
  state.games.forEach((g) => (state.gameById[g.id] = g));

  renderRack();
  wireGlobalKeys();
  wireStageControls();

  const params = new URLSearchParams(location.search);
  const initGameId = params.get('game') || catalog.games[0].id;
  const game = state.gameById[initGameId] || catalog.games[0];
  const initVersionId = params.get('v') || pickDefaultVersion(game).id;
  selectGame(game.id, { initial: true });
  selectVersion(initVersionId, { initial: true });
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
      <div class="cart-label">
        <div class="cart-cover" aria-hidden="true"></div>
        <div class="cart-title">${escapeHtml(game.title)}</div>
      </div>
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
    // When switching game, load the default version for that game.
    const def = pickDefaultVersion(game);
    selectVersion(def.id);
  }
}

function selectVersion(versionId, opts = {}) {
  const game = state.gameById[state.selected.gameId];
  if (!game) return;
  const version = game.versions.find((v) => v.id === versionId) || pickDefaultVersion(game);
  state.selected.versionId = version.id;

  qsa('.version-node', qs('#spine')).forEach((el) => {
    el.classList.toggle('is-active', el.dataset.versionId === version.id);
  });

  loadInPlayer(version);
  renderNotes(game, version);

  if (!opts.initial) {
    const url = new URL(location.href);
    url.searchParams.set('game', game.id);
    url.searchParams.set('v', version.id);
    history.replaceState(null, '', url);
  }
}

function loadInPlayer(version) {
  const frame = qs('#player-frame');
  if (!version.rom) {
    frame.removeAttribute('src');
    return;
  }
  const romURL = new URL(version.rom, location.href).href;
  frame.src = `./player/?romURL=${encodeURIComponent(romURL)}`;
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
function wireStageControls() {
  qs('#btn-crt').addEventListener('click', (e) => {
    const player = qs('#player-wrap');
    const on = player.dataset.crt !== 'true';
    player.dataset.crt = on ? 'true' : 'false';
    e.currentTarget.classList.toggle('is-on', on);
  });
  qs('#btn-fullscreen').addEventListener('click', () => {
    const el = qs('#player-wrap');
    if (!document.fullscreenElement) {
      (el.requestFullscreen || el.webkitRequestFullscreen || (() => {})).call(el);
    } else {
      (document.exitFullscreen || document.webkitExitFullscreen || (() => {})).call(document);
    }
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
    // Don't intercept while typing or while focused inside the iframe.
    if (document.activeElement && document.activeElement.tagName === 'IFRAME') return;

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

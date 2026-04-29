/*
 * Standalone /player/ host. Renders the canvas, mounts the shared core, and wires the
 * touchpad + "Open in new window" button. All gameplay logic — boot, ROM swap, save state,
 * keyboard handling — lives in `core.js` and is shared with the museum shell.
 */

import { createPlayer } from './core.js';

const canvas = document.querySelector('#screen');
const statusEl = document.querySelector('#status');

const player = await createPlayer({ canvas, status: statusEl });
if (player) {
  player.attachKeyboard();
  player.attachTouch(document);

  document.querySelector('#btn-pop')?.addEventListener('click', () => {
    window.open(location.pathname + location.search, '_blank', 'noopener');
  });

  const romURL = new URLSearchParams(location.search).get('romURL');
  if (romURL) {
    await player.loadRomFromUrl(romURL);
  } else if (statusEl) {
    statusEl.textContent = 'Open the showcase page and pick a ROM to play.';
    statusEl.classList.remove('hidden');
  }

  /* Expose the standalone player on window for ad-hoc dev/console access. The museum no
   * longer relies on this — it imports core.js directly — but it's useful when poking from
   * the JavaScript console while debugging the standalone URL. */
  window.player = player;
}

#ifndef RKANOID_GAME_SPRITES_H
#define RKANOID_GAME_SPRITES_H

#include "game_types.h"
#include "gba_types.h"

void ClearAllSprites(void);
/*
 * Render the ball sprite. When `trailEnabled` is TRUE, additional trail slots are placed behind the
 * head along the negative velocity direction (`velX`, `velY` in Q8.8). When FALSE, velocity is ignored.
 */
void SetBallSprite(u16 x, u16 y, boolean trailEnabled, s32 velX, s32 velY, u16 ballTileBase);
void SetPaddleSprite(u16 x, u16 y, boolean longPaddle, u16 playerIndex);
void UpdateBonusSprite(u16 x, u16 y, u16 tileBase, u16 spriteOffset);
void HideBonusSprite(u16 spriteOffset);
void UpdateLifeDisplay(u16 lives);
void UpdateScoreDisplay(u32 score);
void UpdateLevelDisplay(u16 level);
/* Right-side HUD entries below the level readout. `seconds` is capped at 999 (~16 min).
 * `speedLevel` is the current speed tier rendered as a plain integer: 1 at round start,
 * +1 per elapsed tier boundary, capped at 99. Each function only writes to the OAM slots
 * it owns; no side effects on the rest of the HUD. */
void UpdateTimeDisplay(u32 seconds);
void UpdateSpeedDisplay(u16 speedLevel);

#endif

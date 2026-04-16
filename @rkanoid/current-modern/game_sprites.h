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

#endif

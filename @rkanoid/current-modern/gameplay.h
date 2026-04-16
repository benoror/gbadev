#ifndef RKANOID_GAMEPLAY_H
#define RKANOID_GAMEPLAY_H

#include "levels.h"

extern u16 MAX_Y;
extern u16 ANCHO_BOLA;
extern u16 ALTO_BOLA;
extern u16 ANCHO_RAQUETA;
extern u16 ALTO_RAQUETA;

void ClearOam(void);
void RunGame(u16 players);
void RefreshBackground(u16 level, boolean shieldActive);
void SetBallSprite(u16 x, u16 y, u16 trailEnabled, u16 direction, u16 ballTileBase);
void UpdateBonusSprite(u16 x, u16 y, u16 tileBase, u16 spriteOffset);
void UpdateLifeDisplay(u16 num);
void UpdateScoreDisplay(u32 num);
void UpdateLevelDisplay(u16 num);
void SetPaddleSprite(u16 x, u16 y, u16 longPaddle, u16 playerIndex);
void RunServeState(u16 paddleX, u16 paddleY, u16 longPaddle, u16 playerIndex, u32 scoreCount);

#endif

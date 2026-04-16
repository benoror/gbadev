#ifndef RKANOID_BONUSES_H
#define RKANOID_BONUSES_H

#include "game_state.h"
#include "gba_types.h"

void UpdateFallingBonuses(RuntimeState *runtime, u16 paddleX, u16 paddleY, u16 *lifeCount);

#endif

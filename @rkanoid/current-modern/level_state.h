#ifndef RKANOID_LEVEL_STATE_H
#define RKANOID_LEVEL_STATE_H

#include "levels.h"
#include "levels_data.h"

void ResetBlock(Block *block);
void HideBonus(Block *block);
void InitializeBlocksForLevel(u16 level);
void ResetActiveBonusPositions(void);
u16 IsLevelCleared(void);

#endif

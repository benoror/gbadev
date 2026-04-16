#ifndef RKANOID_LEVELS_DATA_H
#define RKANOID_LEVELS_DATA_H

#include "game_types.h"

typedef struct
{
    u16 slot;
    u16 x;
    u16 y;
    u16 tileIndex;
    boolean destructible;
    u8 hitsRemaining;
    u16 bonusType;
} BlockSeed;

const BlockSeed *GetLevelBlockSeeds(u16 level, u16 *count);
u8 *GetLevelTileMap(u16 level);

#endif

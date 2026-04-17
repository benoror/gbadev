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

/*
 * Reload the working tile map from the const ROM source for `level`. Call this whenever a level
 * (re)starts so that bricks destroyed on a previous life are restored to their original tiles.
 */
void LoadLevelTileMap(u16 level);

/* Return a pointer to the active (mutable) working tile map. Requires a prior LoadLevelTileMap. */
u8 *GetLevelTileMap(u16 level);

#endif

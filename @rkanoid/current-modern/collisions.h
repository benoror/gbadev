#ifndef RKANOID_COLLISIONS_H
#define RKANOID_COLLISIONS_H

#include "game_state.h"
#include "gba_types.h"

void ApplyPaddleCollision(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY);
void ProcessBlockCollisions(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY,
    u8 *levelTileMap, u32 *score);

#endif

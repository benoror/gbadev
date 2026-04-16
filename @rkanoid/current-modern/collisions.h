#ifndef RKANOID_COLLISIONS_H
#define RKANOID_COLLISIONS_H

#include "game_state.h"
#include "gba_types.h"

void ApplyPaddleCollision(FrameState *frame, RuntimeState *runtime);
boolean ProcessBlockCollisions(FrameState *frame, RuntimeState *runtime, u8 *levelTileMap, u32 *score);

#endif

#ifndef RKANOID_PHYSICS_H
#define RKANOID_PHYSICS_H

#include "game_state.h"
#include "gba_types.h"

u16 GetPaddleSpeed(u16 buttons);
u16 ClampPaddleX(u16 paddleX, u16 paddleWidth);

void ApplyWorldBounds(FrameState *frame, RuntimeState *runtime, GameSession *session, int *lostLifeThisFrame);
void MovePaddleFromInput(u16 buttons, FrameState *frame, const RuntimeState *runtime);

#endif

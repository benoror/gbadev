#ifndef RKANOID_FRAME_PRESENT_H
#define RKANOID_FRAME_PRESENT_H

#include "game_state.h"
#include "gba_types.h"

void PresentGameplayFrame(const FrameState *frame, const RuntimeState *runtime, u16 activePlayer, u32 score,
    u16 *lives);

#endif

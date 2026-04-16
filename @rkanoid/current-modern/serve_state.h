#ifndef RKANOID_SERVE_STATE_H
#define RKANOID_SERVE_STATE_H

#include "game_state.h"
#include "gba_types.h"

ServeState RunServeState(u16 paddleX, u16 paddleY, u16 playerIndex, u32 scoreCount, RuntimeState *runtime);

#endif

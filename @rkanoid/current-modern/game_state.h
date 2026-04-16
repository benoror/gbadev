#ifndef RKANOID_GAME_STATE_H
#define RKANOID_GAME_STATE_H

#include "game_types.h"
#include "gba_types.h"

typedef struct
{
    u16 ballWidth;
    u16 ballHeight;
    u16 paddleWidth;
    u16 paddleHeight;
    u16 maxBallY;
    u16 ballTileBase;
    boolean longPaddle;
    boolean trailEnabled;
    boolean shieldActive;
} RuntimeState;

typedef struct
{
    u16 paddleX;
    u16 ballX;
} ServeState;

#endif

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

typedef struct
{
    u16 ballX;
    u16 ballY;
    u16 paddleX;
    u16 paddleY;
    int stepX;
    int stepY;
} FrameState;

typedef struct
{
    u16 playerCount;
    u16 activePlayer;
    u16 lives;
    u16 level;
    u32 score;
    u8 *levelTileMap;
} GameSession;

#endif

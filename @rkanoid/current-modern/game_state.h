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
    /* Current target Euclidean ball-speed magnitude in Q8.8 units. Drives BallNormalizeVelocity
     * call-sites so raising it mid-round speeds the ball up on the next bounce (and we also
     * renormalize immediately when it changes). Resets to BALL_SPEED_MAG per level / per life. */
    s32 ballSpeedMag;
    /* Frames elapsed since the current round (per level / per life) started. At 60 fps on GBA,
     * each second = 60 frames. Used to drive the HUD time display and the speed-tier ramp. */
    u32 levelFrame;
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
    s32 ballPosX;
    s32 ballPosY;
    s32 ballVelX;
    s32 ballVelY;
    u16 paddleX;
    u16 paddleY;
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

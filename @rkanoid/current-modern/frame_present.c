#include "frame_present.h"
#include "game_constants.h"
#include "game_sprites.h"
#include "gba_wait.h"

/*
 * Map velocity to one of four trail layouts; near-cardinal motion picks the closer diagonal quadrant.
 */
static u16 BallTrailDirectionFromVelocity(s32 vx, s32 vy)
{
    s32 ax;
    s32 ay;
    s32 sx;
    s32 sy;

    if (vx == 0 && vy == 0)
        return 1;

    ax = vx >= 0 ? vx : -vx;
    ay = vy >= 0 ? vy : -vy;
    sx = vx >= 0 ? 1 : -1;
    sy = vy >= 0 ? 1 : -1;

    if (ay * 2 > ax) {
        if (sy < 0)
            return (sx < 0) ? (u16)0 : (u16)1;
        return (sx < 0) ? (u16)2 : (u16)3;
    }
    if (ax * 2 > ay) {
        if (sx < 0)
            return (sy < 0) ? (u16)0 : (u16)2;
        return (sy < 0) ? (u16)1 : (u16)3;
    }
    if (sx < 0 && sy < 0)
        return 0;
    if (sx > 0 && sy < 0)
        return 1;
    if (sx < 0 && sy > 0)
        return 2;
    return 3;
}

void PresentGameplayFrame(const FrameState *frame, const RuntimeState *runtime, u16 activePlayer, u32 score,
    u16 *lives)
{
    u16 dir;

    wait_vbl_done();
    SetPaddleSprite(frame->paddleX, frame->paddleY, runtime->longPaddle, activePlayer);
    dir = BallTrailDirectionFromVelocity(frame->ballVelX, frame->ballVelY);
    SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, dir, runtime->ballTileBase);
    UpdateScoreDisplay(score);
    if (*lives > MAX_LIVES)
        *lives = MAX_LIVES;
    UpdateLifeDisplay(*lives);
}

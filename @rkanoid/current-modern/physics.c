#include "regs.h"
#include "game_constants.h"
#include "physics.h"
#include "ball_motion.h"
#include "collisions.h"
#include "fixed_point.h"

u16 GetPaddleSpeed(u16 buttons)
{
    return (buttons & J_B) ? 3 : 4;
}

u16 ClampPaddleX(u16 paddleX, u16 paddleWidth)
{
    if (paddleX > MAX_X - paddleWidth)
        paddleX = MAX_X - paddleWidth;
    if (paddleX < MIN_X)
        paddleX = MIN_X;
    return paddleX;
}

void ApplyWorldBounds(FrameState *frame, RuntimeState *runtime, GameSession *session, int *lostLifeThisFrame)
{
    *lostLifeThisFrame = 0;

    if (frame->ballX >= (MAX_X - runtime->ballWidth)) {
        if (frame->ballVelX > 0) {
            frame->ballVelX = -frame->ballVelX;
            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        }
        frame->ballPosX = FIX_FROM_INT((s32)MAX_X - (s32)runtime->ballWidth - 1);
        Ball_SyncPixelsFromFixedPos(frame);
    }
    if (frame->ballX <= MIN_X) {
        if (frame->ballVelX < 0) {
            frame->ballVelX = -frame->ballVelX;
            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        }
        frame->ballPosX = FIX_FROM_INT((s32)MIN_X + 1);
        Ball_SyncPixelsFromFixedPos(frame);
    }
    if (frame->ballY >= (runtime->maxBallY - runtime->ballHeight)) {
        if (runtime->shieldActive == FALSE) {
            --session->lives;
            *lostLifeThisFrame = 1;
            return;
        }
        if (frame->ballVelY > 0) {
            frame->ballVelY = -frame->ballVelY;
            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        }
        frame->ballPosY = FIX_FROM_INT((s32)runtime->maxBallY - (s32)runtime->ballHeight - 1);
        Ball_SyncPixelsFromFixedPos(frame);
        runtime->shieldActive = FALSE;
        runtime->maxBallY = 160;
    }
    if (frame->ballY <= MIN_Y) {
        if (frame->ballVelY < 0) {
            frame->ballVelY = -frame->ballVelY;
            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        }
        frame->ballPosY = FIX_FROM_INT((s32)MIN_Y + 1);
        Ball_SyncPixelsFromFixedPos(frame);
    }
}

/*
 * Steeper angles move more than one pixel per frame along an axis while keeping constant speed,
 * which skips pixel-equality brick tests. Walk the frame displacement in sub-steps (ceil of
 * max(|vx|,|vy|) / FIX_ONE); if a wall or brick changes velocity mid-walk, carry the remaining
 * fraction of that segment as displacement in the new direction (scaled by (n-i)/n).
 */
void IntegrateBallMotionThisFrame(FrameState *frame, RuntimeState *runtime, GameSession *session,
    int *lostLifeThisFrame, u8 *levelTileMap, u32 *score)
{
    s32 remX;
    s32 remY;
    int guard;
    int hitSegment;

    remX = frame->ballVelX;
    remY = frame->ballVelY;
    guard = 0;
    while (guard < 24) {
        s32 ax;
        s32 ay;
        s32 maxAbs;
        u16 n;
        u16 i;
        s32 preVx;
        s32 preVy;
        s32 ri;

        if (remX > -4 && remX < 4 && remY > -4 && remY < 4)
            break;

        ax = remX >= 0 ? remX : -remX;
        ay = remY >= 0 ? remY : -remY;
        maxAbs = ax > ay ? ax : ay;
        n = (u16)((maxAbs + FIX_ONE - 1) / FIX_ONE);
        if (n < 1)
            n = 1;
        if (n > 16)
            n = 16;

        hitSegment = 0;
        for (i = 1; i <= n; i++) {
            s32 ddx;
            s32 ddy;

            ddx = (remX * (s32)i) / (s32)n - (remX * (s32)(i - 1)) / (s32)n;
            ddy = (remY * (s32)i) / (s32)n - (remY * (s32)(i - 1)) / (s32)n;

            frame->ballPosX += ddx;
            frame->ballPosY += ddy;
            Ball_SyncPixelsFromFixedPos(frame);

            preVx = frame->ballVelX;
            preVy = frame->ballVelY;
            ApplyWorldBounds(frame, runtime, session, lostLifeThisFrame);
            if (*lostLifeThisFrame != 0)
                return;
            if (frame->ballVelX != preVx || frame->ballVelY != preVy) {
                ri = (s32)n - (s32)i;
                remX = (frame->ballVelX * ri) / (s32)n;
                remY = (frame->ballVelY * ri) / (s32)n;
                BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                hitSegment = 1;
                break;
            }

            if (ProcessBlockCollisions(frame, runtime, levelTileMap, score) == TRUE) {
                ri = (s32)n - (s32)i;
                remX = (frame->ballVelX * ri) / (s32)n;
                remY = (frame->ballVelY * ri) / (s32)n;
                BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                hitSegment = 1;
                break;
            }

            if (ApplyPaddleCollision(frame, runtime) == TRUE) {
                ri = (s32)n - (s32)i;
                remX = (frame->ballVelX * ri) / (s32)n;
                remY = (frame->ballVelY * ri) / (s32)n;
                BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                hitSegment = 1;
                break;
            }
        }

        if (hitSegment == 0) {
            remX = 0;
            remY = 0;
        }
        guard++;
    }
}

void MovePaddleFromInput(u16 buttons, FrameState *frame, const RuntimeState *runtime)
{
    if (!(buttons & J_RIGHT) && frame->paddleX < (MAX_X - runtime->paddleWidth))
        frame->paddleX += GetPaddleSpeed(buttons);
    if (!(buttons & J_LEFT) && frame->paddleX > MIN_X)
        frame->paddleX -= GetPaddleSpeed(buttons);
    frame->paddleX = ClampPaddleX(frame->paddleX, runtime->paddleWidth);
}

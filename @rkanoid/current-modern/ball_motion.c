#include "ball_motion.h"
#include "fixed_point.h"

void BallNormalizeVelocity(s32 *vx, s32 *vy, s32 speedMag)
{
    s32 vx0;
    s32 vy0;
    u32 mag2_u;
    u32 mag;
    s32 nx;
    s32 ny;

    vx0 = *vx;
    vy0 = *vy;
    mag2_u = (u32)(vx0 * vx0 + vy0 * vy0);
    if (mag2_u == 0) {
        *vx = FIX_ONE;
        *vy = -FIX_ONE;
        mag2_u = (u32)(FIX_ONE * FIX_ONE + FIX_ONE * FIX_ONE);
    }

    mag = u32_isqrt(mag2_u);
    if (mag == 0) {
        *vx = FIX_ONE;
        *vy = -FIX_ONE;
        return;
    }

    nx = (vx0 * speedMag) / (s32)mag;
    ny = (vy0 * speedMag) / (s32)mag;
    *vx = nx;
    *vy = ny;
}

void BallSyncFixedFromPixels(FrameState *frame)
{
    frame->ballPosX = FIX_FROM_INT((s32)frame->ballX);
    frame->ballPosY = FIX_FROM_INT((s32)frame->ballY);
}

void Ball_SyncPixelsFromFixedPos(FrameState *frame)
{
    frame->ballX = (u16)FIX_TO_INT_ROUND(frame->ballPosX);
    frame->ballY = (u16)FIX_TO_INT_ROUND(frame->ballPosY);
}

void BallIntegrateAndSyncPixels(FrameState *frame)
{
    frame->ballPosX += frame->ballVelX;
    frame->ballPosY += frame->ballVelY;
    Ball_SyncPixelsFromFixedPos(frame);
}

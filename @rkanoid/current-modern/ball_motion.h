#ifndef RKANOID_BALL_MOTION_H
#define RKANOID_BALL_MOTION_H

#include "game_state.h"

/*
 * Euclidean speed in the same integer space as fixed-point velocity components
 * (matches legacy diagonal step (256, -256) length).
 */
#define BALL_SPEED_MAG ((s32)362)

void BallNormalizeVelocity(s32 *vx, s32 *vy, s32 speedMag);
void BallSyncFixedFromPixels(FrameState *frame);
void Ball_SyncPixelsFromFixedPos(FrameState *frame);
void BallIntegrateAndSyncPixels(FrameState *frame);

/*
 * Compute the pixel offset of trail step `step` relative to the ball head, placed opposite to the
 * velocity direction. `stepSpacing` is the pixel distance between consecutive trail slots along
 * the motion line (e.g. 2). `speedMag` is the Euclidean length used to unit-normalize the
 * velocity (typically BALL_SPEED_MAG).
 */
void BallComputeTrailOffset(s32 vx, s32 vy, s32 speedMag, u16 step, s32 stepSpacing, s32 *outX, s32 *outY);

#endif

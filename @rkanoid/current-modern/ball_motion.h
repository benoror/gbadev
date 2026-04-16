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

#endif

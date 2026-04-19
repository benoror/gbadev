#include "game_constants.h"
#include "collisions.h"
#include "levels.h"
#include "ball_motion.h"
#include "fixed_point.h"

static s32 SnapPixelX(s32 v, s32 ballWidth)
{
    s32 lo = (s32)MIN_X + 1;
    s32 hi = (s32)MAX_X - ballWidth - 1;
    if (v < lo)
        v = lo;
    if (v > hi)
        v = hi;
    return v;
}

static s32 SnapPixelY(s32 v, s32 ballHeight, s32 maxBallY)
{
    s32 lo = (s32)MIN_Y + 1;
    s32 hi = maxBallY - ballHeight - 1;
    if (v < lo)
        v = lo;
    if (v > hi)
        v = hi;
    return v;
}

/*
 * Inclusive pixel AABB from the rounded sprite pixel (matches visuals and avoids off-by-one AABB drift that
 * appears when ballPosX/Y are not integer multiples of FIX_ONE).
 */
static void BallGetPixelBoundsFixed(const FrameState *frame, const RuntimeState *runtime, s32 *outL, s32 *outT,
    s32 *outR, s32 *outB)
{
    *outL = FIX_TO_INT_ROUND(frame->ballPosX);
    *outT = FIX_TO_INT_ROUND(frame->ballPosY);
    *outR = *outL + (s32)runtime->ballWidth - 1;
    *outB = *outT + (s32)runtime->ballHeight - 1;
}

static void DestroyBlock(Block *block, u8 *levelTileMap, u32 *score)
{
    block->x = 0;
    block->y = 0;
    block->right = 0;
    block->bottom = 0;
    levelTileMap[block->tileIndex] = TILE_BRICK_PRESSED_1;
    levelTileMap[block->tileIndex2] = TILE_BRICK_PRESSED_2;
    *score += 100;
}

/*
 * Apply scoring / bonus activation / destruction for a block hit. Does NOT mutate velocity — the caller
 * decides direction based on MTV so we don't flip wrongly on deep penetrations.
 */
static void RegisterBlockHit(Block *block, RuntimeState *runtime, boolean pierce, boolean destroyOnlyWhenCleared,
    u8 *levelTileMap, u32 *score)
{
    if (block->bonusType != NO_BONUS && block->hitsRemaining <= 1)
        block->bonusActive = TRUE;

    if (pierce == TRUE)
        block->hitsRemaining = 0;

    if (block->hitsRemaining > 0) {
        block->hitsRemaining--;
        *score += 25;
    }
    if (block->destructible == TRUE && (destroyOnlyWhenCleared == FALSE || block->hitsRemaining == 0))
        DestroyBlock(block, levelTileMap, score);
}

static void PaddleLaunchFromHitParam(FrameState *frame, RuntimeState *runtime)
{
    s32 pw = (s32)runtime->paddleWidth;
    s32 center = (s32)frame->paddleX + pw / 2;
    s32 ballCenter = FIX_TO_INT_FLOOR(frame->ballPosX) + (s32)runtime->ballWidth / 2;
    s32 half = pw / 2;
    s32 t;
    s32 maxVx;
    s32 vx;
    s32 vySq;
    s32 vy;

    if (half < 1)
        half = 1;
    t = ((ballCenter - center) * FIX_ONE) / half;
    if (t > FIX_ONE)
        t = FIX_ONE;
    else if (t < -FIX_ONE)
        t = -FIX_ONE;
    {
        const s32 speed = runtime->ballSpeedMag;
        maxVx = (speed * 85) / 100;
        vx = (t * maxVx) / FIX_ONE;
        vySq = speed * speed - vx * vx;
        if (vySq < 0) {
            vx = 0;
            vySq = speed * speed;
        }
    }
    vy = -(s32)u32_isqrt((u32)vySq);
    frame->ballVelX = vx;
    frame->ballVelY = vy;
    BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
}

boolean ApplyPaddleCollision(FrameState *frame, RuntimeState *runtime)
{
    s32 ballL;
    s32 ballT;
    s32 ballR;
    s32 ballB;
    s32 padL;
    s32 padT;
    s32 padR;
    s32 padB;

    BallGetPixelBoundsFixed(frame, runtime, &ballL, &ballT, &ballR, &ballB);
    padL = (s32)frame->paddleX;
    padT = (s32)frame->paddleY;
    padR = (s32)frame->paddleX + (s32)runtime->paddleWidth - 1;
    padB = (s32)frame->paddleY + (s32)runtime->paddleHeight - 1;

    if (ballR < padL || ballL > padR || ballB < padT || ballT > padB)
        return FALSE;

    /* Moving down and still mostly above the paddle top: treat as top hit (primary case). */
    if (frame->ballVelY > 0 && ballT <= padT) {
        /* Keep ball sitting on top of the paddle to avoid re-trigger on next sub-step. */
        frame->ballPosY =
            FIX_FROM_INT(SnapPixelY(padT - (s32)runtime->ballHeight, (s32)runtime->ballHeight, (s32)runtime->maxBallY));
        Ball_SyncPixelsFromFixedPos(frame);
        PaddleLaunchFromHitParam(frame, runtime);
        return TRUE;
    }

    /* Side hits: ball deeper than paddle top, redirect up-left / up-right. */
    if (frame->ballVelX > 0 && ballR >= padL && ballL < padL) {
        frame->ballVelX = -FIX_ONE;
        frame->ballVelY = -FIX_ONE;
        BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        frame->ballPosX = FIX_FROM_INT(SnapPixelX(padL - (s32)runtime->ballWidth - 1, (s32)runtime->ballWidth));
        Ball_SyncPixelsFromFixedPos(frame);
        return TRUE;
    }
    if (frame->ballVelX < 0 && ballL <= padR && ballR > padR) {
        frame->ballVelX = FIX_ONE;
        frame->ballVelY = -FIX_ONE;
        BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
        frame->ballPosX = FIX_FROM_INT(SnapPixelX(padR + 1, (s32)runtime->ballWidth));
        Ball_SyncPixelsFromFixedPos(frame);
        return TRUE;
    }

    /* Overlap exists but no velocity gate matched (e.g. ball dropped into paddle vertically): force up. */
    if (frame->ballVelY >= 0) {
        frame->ballPosY =
            FIX_FROM_INT(SnapPixelY(padT - (s32)runtime->ballHeight, (s32)runtime->ballHeight, (s32)runtime->maxBallY));
        Ball_SyncPixelsFromFixedPos(frame);
        PaddleLaunchFromHitParam(frame, runtime);
        return TRUE;
    }
    return FALSE;
}

boolean ProcessBlockCollisions(FrameState *frame, RuntimeState *runtime, u8 *levelTileMap, u32 *score)
{
    u16 index;
    boolean anyHit;
    const s32 incomingVx = frame->ballVelX;
    const s32 incomingVy = frame->ballVelY;
    const s32 absVx = incomingVx >= 0 ? incomingVx : -incomingVx;
    const s32 absVy = incomingVy >= 0 ? incomingVy : -incomingVy;

    anyHit = FALSE;
    for (index = 0; index < BLOCK_CAPACITY; ++index) {
        Block *block = &gBlocks[index];
        s32 ballL;
        s32 ballT;
        s32 ballR;
        s32 ballB;
        s32 penLeft;
        s32 penRight;
        s32 penTop;
        s32 penBottom;
        s32 minPenX;
        s32 minPenY;
        boolean axisVertical;
        boolean fromAbove;
        boolean fromLeft;
        boolean pierce;

        if (block->x == 0)
            continue;

        BallGetPixelBoundsFixed(frame, runtime, &ballL, &ballT, &ballR, &ballB);
        /* Block occupies [x, right-1] x [y, bottom-1]. Strict overlap avoids phantom edge hits. */
        if (ballL >= block->right || ballR < block->x || ballT >= block->bottom || ballB < block->y)
            continue;

        /* Minimum-translation-vector resolution. Each penetration is "pixels the ball must move to exit". */
        penLeft = ballR - block->x + 1;      /* push ball left by this much → ballR < block->x */
        penRight = block->right - ballL;     /* push ball right → ballL >= block->right */
        penTop = ballB - block->y + 1;       /* push ball up → ballB < block->y */
        penBottom = block->bottom - ballT;   /* push ball down → ballT >= block->bottom */

        minPenX = (penLeft < penRight) ? penLeft : penRight;
        minPenY = (penTop < penBottom) ? penTop : penBottom;

        if (minPenY < minPenX) {
            axisVertical = TRUE;
        } else if (minPenX < minPenY) {
            axisVertical = FALSE;
        } else {
            axisVertical = (absVy >= absVx);
        }

        fromAbove = (penTop <= penBottom);
        fromLeft = (penLeft <= penRight);
        pierce = (boolean)(runtime->trailEnabled == TRUE && block->destructible == TRUE);

        {
            /* Cache coords: RegisterBlockHit may destroy the brick and zero them before we snap. */
            const s32 savedX = (s32)block->x;
            const s32 savedY = (s32)block->y;
            const s32 savedRight = (s32)block->right;
            const s32 savedBottom = (s32)block->bottom;

            if (axisVertical == TRUE) {
                if (fromAbove == TRUE) {
                    RegisterBlockHit(block, runtime, pierce, TRUE, levelTileMap, score);
                    if (pierce == FALSE) {
                        if (frame->ballVelY > 0) {
                            frame->ballVelY = -frame->ballVelY;
                            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                        }
                        frame->ballPosY = FIX_FROM_INT(
                            SnapPixelY(savedY - (s32)runtime->ballHeight, (s32)runtime->ballHeight,
                                (s32)runtime->maxBallY));
                        Ball_SyncPixelsFromFixedPos(frame);
                        return TRUE;
                    }
                } else {
                    RegisterBlockHit(block, runtime, pierce, TRUE, levelTileMap, score);
                    if (pierce == FALSE) {
                        if (frame->ballVelY < 0) {
                            frame->ballVelY = -frame->ballVelY;
                            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                        }
                        frame->ballPosY = FIX_FROM_INT(
                            SnapPixelY(savedBottom, (s32)runtime->ballHeight, (s32)runtime->maxBallY));
                        Ball_SyncPixelsFromFixedPos(frame);
                        return TRUE;
                    }
                }
            } else {
                if (fromLeft == TRUE) {
                    RegisterBlockHit(block, runtime, pierce, TRUE, levelTileMap, score);
                    if (pierce == FALSE) {
                        if (frame->ballVelX > 0) {
                            frame->ballVelX = -frame->ballVelX;
                            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                        }
                        frame->ballPosX = FIX_FROM_INT(
                            SnapPixelX(savedX - (s32)runtime->ballWidth, (s32)runtime->ballWidth));
                        Ball_SyncPixelsFromFixedPos(frame);
                        return TRUE;
                    }
                } else {
                    RegisterBlockHit(block, runtime, pierce, TRUE, levelTileMap, score);
                    if (pierce == FALSE) {
                        if (frame->ballVelX < 0) {
                            frame->ballVelX = -frame->ballVelX;
                            BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, runtime->ballSpeedMag);
                        }
                        frame->ballPosX =
                            FIX_FROM_INT(SnapPixelX(savedRight, (s32)runtime->ballWidth));
                        Ball_SyncPixelsFromFixedPos(frame);
                        return TRUE;
                    }
                }
            }
        }
        anyHit = TRUE;
    }
    return anyHit;
}

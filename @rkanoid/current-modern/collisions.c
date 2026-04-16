#include "game_constants.h"
#include "collisions.h"
#include "levels.h"
#include "ball_motion.h"
#include "fixed_point.h"

/*
 * Inclusive pixel AABB from Q8.8 top-left (avoids missing hits when rounded ballX/ballY lag sub-pixel motion).
 */
static void BallGetPixelBoundsFixed(const FrameState *frame, const RuntimeState *runtime, s32 *outL, s32 *outT,
    s32 *outR, s32 *outB)
{
    s32 bw;
    s32 bh;

    bw = (s32)runtime->ballWidth;
    bh = (s32)runtime->ballHeight;
    *outL = FIX_TO_INT_FLOOR(frame->ballPosX);
    *outT = FIX_TO_INT_FLOOR(frame->ballPosY);
    *outR = FIX_TO_INT_FLOOR(frame->ballPosX + bw * FIX_ONE - 1);
    *outB = FIX_TO_INT_FLOOR(frame->ballPosY + bh * FIX_ONE - 1);
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

static void ResolveBlockHit(Block *block, RuntimeState *runtime, boolean axisIsVertical, s32 *velX, s32 *velY,
    const s32 incomingVx, const s32 incomingVy, boolean destroyOnlyWhenCleared, u8 *levelTileMap, u32 *score)
{
    if (block->bonusType != NO_BONUS && block->hitsRemaining <= 1)
        block->bonusActive = TRUE;

    if (runtime->trailEnabled == TRUE && block->destructible == TRUE) {
        block->hitsRemaining = 0;
        if (axisIsVertical == TRUE)
            *velY = incomingVy;
        else
            *velX = incomingVx;
    } else {
        if (axisIsVertical == TRUE)
            *velY = -*velY;
        else
            *velX = -*velX;
        BallNormalizeVelocity(velX, velY, BALL_SPEED_MAG);
    }

    if (block->hitsRemaining > 0) {
        block->hitsRemaining--;
        *score += 25;
    }
    if (block->destructible == TRUE && (destroyOnlyWhenCleared == FALSE || block->hitsRemaining == 0))
        DestroyBlock(block, levelTileMap, score);
}

void ApplyPaddleCollision(FrameState *frame, RuntimeState *runtime)
{
    if (frame->ballY == (frame->paddleY - runtime->ballHeight) &&
        frame->ballX >= (frame->paddleX - runtime->ballWidth) &&
        frame->ballX <= (frame->paddleX - runtime->ballWidth) + runtime->paddleWidth) {
        s32 pw = (s32)runtime->paddleWidth;
        s32 center = (s32)frame->paddleX + pw / 2;
        s32 ballCenter = (s32)frame->ballX + (s32)runtime->ballWidth / 2;
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
        maxVx = (BALL_SPEED_MAG * 85) / 100;
        vx = (t * maxVx) / FIX_ONE;
        vySq = BALL_SPEED_MAG * BALL_SPEED_MAG - vx * vx;
        if (vySq < 0) {
            vx = 0;
            vySq = BALL_SPEED_MAG * BALL_SPEED_MAG;
        }
        vy = -(s32)u32_isqrt((u32)vySq);
        frame->ballVelX = vx;
        frame->ballVelY = vy;
        BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, BALL_SPEED_MAG);
        return;
    }

    if (frame->ballX == (frame->paddleX - runtime->ballWidth) && frame->ballY >= frame->paddleY - runtime->ballHeight &&
        frame->ballY <= frame->paddleY + runtime->paddleHeight) {
        frame->ballVelX = -FIX_ONE;
        frame->ballVelY = -FIX_ONE;
        BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, BALL_SPEED_MAG);
        return;
    }

    if (frame->ballX == (frame->paddleX + runtime->paddleWidth) &&
        frame->ballY >= frame->paddleY - runtime->ballHeight &&
        frame->ballY <= frame->paddleY + runtime->paddleHeight) {
        frame->ballVelX = FIX_ONE;
        frame->ballVelY = -FIX_ONE;
        BallNormalizeVelocity(&frame->ballVelX, &frame->ballVelY, BALL_SPEED_MAG);
    }
}

boolean ProcessBlockCollisions(FrameState *frame, RuntimeState *runtime, u8 *levelTileMap, u32 *score)
{
    u16 index;
    boolean anyHit;
    /*
     * Gate each face with the velocity at the *start* of this frame (same as legacy stepX/stepY),
     * so adjacent bricks on a vertical seam cannot both flip the X component using stale signs.
     */
    const s32 incomingVx = frame->ballVelX;
    const s32 incomingVy = frame->ballVelY;
    s32 absVx;
    s32 absVy;

    absVx = incomingVx >= 0 ? incomingVx : -incomingVx;
    absVy = incomingVy >= 0 ? incomingVy : -incomingVy;

    anyHit = FALSE;
    for (index = 0; index < BLOCK_CAPACITY; ++index) {
        Block *block = &gBlocks[index];
        boolean hitTop;
        boolean hitBottom;
        boolean hitLeft;
        boolean hitRight;
        int vHits;
        int hHits;
        s32 ballL;
        s32 ballT;
        s32 ballR;
        s32 ballB;
        int overlap;

        if (block->x == 0)
            continue;

        BallGetPixelBoundsFixed(frame, runtime, &ballL, &ballT, &ballR, &ballB);
        overlap = (ballL <= block->right && ballR >= block->x && ballT <= block->bottom && ballB >= block->y);
        if (overlap == 0)
            continue;

        hitTop = (boolean)(incomingVy > 0 && ballL <= block->right && ballR >= block->x && ballB >= block->y - 1 &&
            ballT <= block->bottom);
        hitBottom = (boolean)(incomingVy < 0 && ballL <= block->right && ballR >= block->x &&
            ballT <= block->bottom + 1 && ballB >= block->y);
        hitLeft = (boolean)(incomingVx > 0 && ballT <= block->bottom && ballB >= block->y && ballR >= block->x - 1 &&
            ballL <= block->right);
        hitRight = (boolean)(incomingVx < 0 && ballT <= block->bottom && ballB >= block->y &&
            ballL <= block->right + 1 && ballR >= block->x);

        vHits = (hitTop ? 1 : 0) + (hitBottom ? 1 : 0);
        hHits = (hitLeft ? 1 : 0) + (hitRight ? 1 : 0);
        if (vHits > 0 && hHits > 0) {
            if (absVy >= absVx) {
                hitLeft = FALSE;
                hitRight = FALSE;
            } else {
                hitTop = FALSE;
                hitBottom = FALSE;
            }
        }

        if (hitTop == TRUE) {
            ResolveBlockHit(block, runtime, TRUE, &frame->ballVelX, &frame->ballVelY, incomingVx, incomingVy, TRUE,
                levelTileMap, score);
            anyHit = TRUE;
        } else if (hitBottom == TRUE) {
            ResolveBlockHit(block, runtime, TRUE, &frame->ballVelX, &frame->ballVelY, incomingVx, incomingVy, TRUE,
                levelTileMap, score);
            anyHit = TRUE;
        } else if (hitLeft == TRUE) {
            ResolveBlockHit(block, runtime, FALSE, &frame->ballVelX, &frame->ballVelY, incomingVx, incomingVy, FALSE,
                levelTileMap, score);
            anyHit = TRUE;
        } else if (hitRight == TRUE) {
            ResolveBlockHit(block, runtime, FALSE, &frame->ballVelX, &frame->ballVelY, incomingVx, incomingVy, TRUE,
                levelTileMap, score);
            anyHit = TRUE;
        }
    }
    return anyHit;
}

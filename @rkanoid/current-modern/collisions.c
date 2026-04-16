#include "game_constants.h"
#include "collisions.h"
#include "levels.h"

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

static void ResolveBlockHit(Block *block, RuntimeState *runtime, int *axisStep, int normalBounce, int trailBounce,
    boolean destroyOnlyWhenCleared, u8 *levelTileMap, u32 *score)
{
    *axisStep = normalBounce;
    if (block->bonusType != NO_BONUS && block->hitsRemaining <= 1)
        block->bonusActive = TRUE;
    if (runtime->trailEnabled == TRUE && block->destructible == TRUE) {
        block->hitsRemaining = 0;
        *axisStep = trailBounce;
    }
    if (block->hitsRemaining > 0) {
        block->hitsRemaining--;
        *score += 25;
    }
    if (block->destructible == TRUE && (destroyOnlyWhenCleared == FALSE || block->hitsRemaining == 0))
        DestroyBlock(block, levelTileMap, score);
}

void ApplyPaddleCollision(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY)
{
    u16 splitWidth = runtime->longPaddle ? 24 : 16;

    if (frame->ballY == (frame->paddleY - runtime->ballHeight) &&
        frame->ballX >= (frame->paddleX - runtime->ballWidth) &&
        frame->ballX <= (frame->paddleX - runtime->ballWidth) + splitWidth) {
        *stepX = -1;
        *stepY = -1;
    }
    if (frame->ballY == (frame->paddleY - runtime->ballHeight) &&
        frame->ballX >= (frame->paddleX - runtime->ballWidth) + splitWidth + 1 &&
        frame->ballX <= (frame->paddleX - runtime->ballWidth) + runtime->paddleWidth) {
        *stepX = 1;
        *stepY = -1;
    }
    if (frame->ballX == (frame->paddleX - runtime->ballWidth) &&
        frame->ballY >= frame->paddleY - runtime->ballHeight &&
        frame->ballY <= frame->paddleY + runtime->paddleHeight) {
        *stepX = -1;
        *stepY = -1;
    }
    if (frame->ballX == (frame->paddleX + runtime->paddleWidth) &&
        frame->ballY >= frame->paddleY - runtime->ballHeight &&
        frame->ballY <= frame->paddleY + runtime->paddleHeight) {
        *stepX = 1;
        *stepY = -1;
    }
}

void ProcessBlockCollisions(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY,
    u8 *levelTileMap, u32 *score)
{
    u16 index;
    /*
     * Only resolve a face if the ball was moving *toward* that face this frame.
     * Otherwise two adjacent blocks can both register an edge hit on the same pixel
     * (shared vertical seam: right face of left block + left face of right block),
     * and applying both flips stepX twice (wrong / "opposite" bounce).
     */
    const int incomingX = *stepX;
    const int incomingY = *stepY;

    for (index = 0; index < BLOCK_CAPACITY; ++index) {
        Block *block = &gBlocks[index];

        if (incomingY > 0 && frame->ballY == block->y - runtime->ballHeight &&
            frame->ballX >= block->x - runtime->ballWidth && frame->ballX <= block->right) {
            ResolveBlockHit(block, runtime, stepY, -1, 1, TRUE, levelTileMap, score);
        }
        if (incomingY < 0 && frame->ballY == block->bottom &&
            frame->ballX >= block->x - runtime->ballWidth && frame->ballX <= block->right) {
            ResolveBlockHit(block, runtime, stepY, 1, -1, TRUE, levelTileMap, score);
        }
        if (incomingX > 0 && frame->ballX == block->x - runtime->ballWidth &&
            frame->ballY >= block->y - runtime->ballHeight && frame->ballY <= block->bottom) {
            ResolveBlockHit(block, runtime, stepX, -1, 1, FALSE, levelTileMap, score);
        }
        if (incomingX < 0 && frame->ballX == block->right &&
            frame->ballY >= block->y - runtime->ballHeight && frame->ballY <= block->bottom) {
            ResolveBlockHit(block, runtime, stepX, 1, -1, TRUE, levelTileMap, score);
        }
    }
}

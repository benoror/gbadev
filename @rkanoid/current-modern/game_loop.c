#include "regs.h"
#include "gameplay.h"
#include "game_state.h"
#include "game_sprites.h"
#include "levels_runtime.h"
#include "levels_data.h"

#define MIN_X 16
#define MIN_Y 8
#define MAX_X 160
#define BALL_TILE_SMALL 8
#define BALL_TILE_LARGE 40
#define DEFAULT_PADDLE_X 16
#define DEFAULT_PADDLE_Y 144
#define PLAYER_ONE 1
#define PLAYER_TWO 2
#define MAX_LIVES 99
#define BLOCK_CAPACITY 162

typedef struct
{
    u16 ballX;
    u16 ballY;
    u16 paddleX;
    u16 paddleY;
    int stepX;
    int stepY;
} FrameState;

static void InitializeRuntime(RuntimeState *runtime)
{
    runtime->ballWidth = 6;
    runtime->ballHeight = 6;
    runtime->paddleWidth = 32;
    runtime->paddleHeight = 8;
    runtime->maxBallY = 160;
    runtime->ballTileBase = BALL_TILE_SMALL;
    runtime->longPaddle = FALSE;
    runtime->trailEnabled = FALSE;
    runtime->shieldActive = FALSE;
}

static u16 GetPaddleSpeed(u16 buttons)
{
    return (buttons & J_B) ? 3 : 4;
}

static void AdvancePlayerTurn(u16 playerCount, u16 *activePlayer)
{
    if (playerCount != 2)
        return;
    *activePlayer = (*activePlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
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

static void ApplyCollectedBonus(Block *block, RuntimeState *runtime, u16 *lifeCount)
{
    switch (block->bonusType) {
    case COMET_BONUS:
        runtime->trailEnabled = TRUE;
        break;
    case LONG_PADDLE_BONUS:
        runtime->paddleWidth = 48;
        runtime->longPaddle = TRUE;
        break;
    case EXTRA_LIFE_BONUS:
        ++(*lifeCount);
        break;
    case BIG_BALL_BONUS:
        runtime->trailEnabled = FALSE;
        runtime->ballTileBase = BALL_TILE_LARGE;
        runtime->ballWidth = 8;
        runtime->ballHeight = 8;
        break;
    case SHIELD_BONUS:
        runtime->shieldActive = TRUE;
        runtime->maxBallY = 148;
        break;
    default:
        break;
    }

    HideBonus(block);
    HideBonusSprite(block->bonusSlot);
}

static void UpdateFallingBonuses(RuntimeState *runtime, u16 paddleX, u16 paddleY, u16 *lifeCount)
{
    u16 index;

    for (index = 0; index < BLOCK_CAPACITY; ++index) {
        Block *block = &gBlocks[index];
        if (block->bonusActive != TRUE || block->bonusType == NO_BONUS)
            continue;

        UpdateBonusSprite(block->bonusX, block->bonusY, block->bonusType, block->bonusSlot);
        block->bonusY++;
        if (block->bonusX + 16 >= paddleX && block->bonusX <= paddleX + runtime->paddleWidth &&
            block->bonusY >= paddleY && block->bonusY <= paddleY + runtime->paddleHeight) {
            ApplyCollectedBonus(block, runtime, lifeCount);
            continue;
        }
        if (block->bonusY >= runtime->maxBallY + 16) {
            HideBonus(block);
            HideBonusSprite(block->bonusSlot);
        }
    }
}

static void ApplyPaddleCollision(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY)
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

static void ProcessBlockCollisions(const FrameState *frame, RuntimeState *runtime, int *stepX, int *stepY,
    u8 *levelTileMap, u32 *score)
{
    u16 index;

    for (index = 0; index < BLOCK_CAPACITY; ++index) {
        Block *block = &gBlocks[index];

        if (frame->ballY == block->y - runtime->ballHeight &&
            frame->ballX >= block->x - runtime->ballWidth && frame->ballX <= block->right) {
            ResolveBlockHit(block, runtime, stepY, -1, 1, TRUE, levelTileMap, score);
        }
        if (frame->ballY == block->bottom &&
            frame->ballX >= block->x - runtime->ballWidth && frame->ballX <= block->right) {
            ResolveBlockHit(block, runtime, stepY, 1, -1, TRUE, levelTileMap, score);
        }
        if (frame->ballX == block->x - runtime->ballWidth &&
            frame->ballY >= block->y - runtime->ballHeight && frame->ballY <= block->bottom) {
            ResolveBlockHit(block, runtime, stepX, -1, 1, FALSE, levelTileMap, score);
        }
        if (frame->ballX == block->right &&
            frame->ballY >= block->y - runtime->ballHeight && frame->ballY <= block->bottom) {
            ResolveBlockHit(block, runtime, stepX, 1, -1, TRUE, levelTileMap, score);
        }
    }
}

static u16 ClampPaddleX(u16 paddleX, u16 paddleWidth)
{
    if (paddleX > MAX_X - paddleWidth)
        paddleX = MAX_X - paddleWidth;
    if (paddleX < MIN_X)
        paddleX = MIN_X;
    return paddleX;
}

static ServeState RunServeStateInternal(u16 paddleX, u16 paddleY, u16 playerIndex, u32 scoreCount, RuntimeState *runtime)
{
    ServeState state;
    u16 buttons;
    u16 paddleSpeed;

    state.paddleX = paddleX;
    state.ballX = paddleX + 13;
    SetPaddleSprite(state.paddleX, paddleY, runtime->longPaddle, playerIndex);
    SetBallSprite(state.ballX, paddleY - runtime->ballHeight, FALSE, 0, BALL_TILE_SMALL);
    buttons = ReadJoypad();
    while (buttons & J_A) {
        buttons = ReadJoypad();
        paddleSpeed = GetPaddleSpeed(buttons);
        if (buttons) {
            if (!(buttons & J_RIGHT) && state.paddleX < (MAX_X - runtime->paddleWidth)) {
                state.paddleX += paddleSpeed;
                state.ballX += paddleSpeed;
            }
            if (!(buttons & J_LEFT) && state.paddleX > MIN_X) {
                state.paddleX -= paddleSpeed;
                state.ballX -= paddleSpeed;
            }
            if (!(buttons & J_L) && state.ballX > state.paddleX)
                --state.ballX;
            if (!(buttons & J_R) && state.ballX - runtime->ballWidth <= state.paddleX + 19)
                ++state.ballX;
        }
        SetPaddleSprite(state.paddleX, paddleY, runtime->longPaddle, playerIndex);
        SetBallSprite(state.ballX, paddleY - runtime->ballHeight, FALSE, 0, BALL_TILE_SMALL);
        UpdateScoreDisplay(scoreCount);
    }

    return state;
}

void RefreshBackground(u16 level, boolean shieldActive)
{
    CopyLevelTileMapToBackground(level);
    if (shieldActive == TRUE)
        ApplyShieldFloor();
    CopyBackgroundMapToVram();
}

void RunGame(u16 playerCount)
{
    FrameState frame;
    RuntimeState runtime;
    u16 buttons;
    u16 activePlayer = PLAYER_ONE;
    u16 lives = 3;
    u16 level = 1;
    u32 score = 0;
    u8 *levelTileMap = 0;

    frame.paddleX = DEFAULT_PADDLE_X;
    frame.paddleY = DEFAULT_PADDLE_Y;

    while (lives > 0 && level <= 5) {
        if (level > 1)
            FadeOut(30, TRUE, FALSE);

        InitializeBlocksForLevel(level);
        levelTileMap = GetLevelTileMap(level);
        RefreshBackground(level, FALSE);
        ClearAllSprites();
        WaitVBlanks(100);
        FadeIn(30, TRUE, FALSE);
        UpdateLevelDisplay(level);
        UpdateLifeDisplay(lives);
        ResetActiveBonusPositions();
        InitializeRuntime(&runtime);
        RefreshBackground(level, runtime.shieldActive);
        AdvancePlayerTurn(playerCount, &activePlayer);

        if (lives == 0)
            break;

        {
            ServeState serve = RunServeStateInternal(frame.paddleX, frame.paddleY, activePlayer, score, &runtime);
            frame.paddleX = serve.paddleX;
            frame.ballX = serve.ballX;
            frame.ballY = frame.paddleY - runtime.ballHeight;
        }
        frame.stepX = 1;
        frame.stepY = -1;

        while (lives > 0 && level <= 5) {
            if (frame.ballX >= (MAX_X - runtime.ballWidth))
                frame.stepX = -1;
            if (frame.ballX <= MIN_X)
                frame.stepX = 1;
            if (frame.ballY >= (runtime.maxBallY - runtime.ballHeight)) {
                frame.stepY = -1;
                if (runtime.shieldActive == FALSE) {
                    --lives;
                    break;
                }
                runtime.shieldActive = FALSE;
                runtime.maxBallY = 160;
            }
            if (frame.ballY <= MIN_Y)
                frame.stepY = 1;

            ApplyPaddleCollision(&frame, &runtime, &frame.stepX, &frame.stepY);
            ProcessBlockCollisions(&frame, &runtime, &frame.stepX, &frame.stepY, levelTileMap, &score);
            UpdateFallingBonuses(&runtime, frame.paddleX, frame.paddleY, &lives);

            buttons = ReadJoypad();
            if (!(buttons & J_RIGHT) && frame.paddleX < (MAX_X - runtime.paddleWidth))
                frame.paddleX += GetPaddleSpeed(buttons);
            if (!(buttons & J_LEFT) && frame.paddleX > MIN_X)
                frame.paddleX -= GetPaddleSpeed(buttons);
            frame.paddleX = ClampPaddleX(frame.paddleX, runtime.paddleWidth);
            if (!(buttons & J_SELECT))
                break;

            frame.ballX += frame.stepX;
            frame.ballY += frame.stepY;

            wait_vbl_done();
            SetPaddleSprite(frame.paddleX, frame.paddleY, runtime.longPaddle, activePlayer);
            if (frame.stepX < 0 && frame.stepY < 0)
                SetBallSprite(frame.ballX, frame.ballY, runtime.trailEnabled, 0, runtime.ballTileBase);
            if (frame.stepX > 0 && frame.stepY < 0)
                SetBallSprite(frame.ballX, frame.ballY, runtime.trailEnabled, 1, runtime.ballTileBase);
            if (frame.stepX < 0 && frame.stepY > 0)
                SetBallSprite(frame.ballX, frame.ballY, runtime.trailEnabled, 2, runtime.ballTileBase);
            if (frame.stepX > 0 && frame.stepY > 0)
                SetBallSprite(frame.ballX, frame.ballY, runtime.trailEnabled, 3, runtime.ballTileBase);
            UpdateScoreDisplay(score);
            if (lives > MAX_LIVES)
                lives = MAX_LIVES;
            UpdateLifeDisplay(lives);
            if (IsLevelCleared() == TRUE) {
                ++level;
                break;
            }
            RefreshBackground(level, runtime.shieldActive);
        }
    }
}

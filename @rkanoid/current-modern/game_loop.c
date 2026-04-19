#include "regs.h"
#include "gameplay.h"
#include "game_constants.h"
#include "game_state.h"
#include "game_sprites.h"
#include "level_state.h"
#include "level_render.h"
#include "levels_data.h"
#include "physics.h"
#include "collisions.h"
#include "bonuses.h"
#include "serve_state.h"
#include "frame_present.h"
#include "ball_motion.h"
#include "fixed_point.h"

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
    runtime->ballSpeedMag = BALL_SPEED_MAG;
    runtime->levelFrame = 0;
}

/* Every 30 s (1800 frames @60 fps) ramp the ball speed by +20 %. Linear ramp is enough;
 * players stop feeling the step after a couple of tiers. */
#define SPEED_TIER_FRAMES 1800u
#define SPEED_TIER_STEP_NUMERATOR 2
#define SPEED_TIER_STEP_DENOMINATOR 10

static s32 SpeedMagForTier(u32 tier)
{
    /* BALL_SPEED_MAG * (1 + tier * step_num / step_den) without floating point. */
    return (s32)((u32)BALL_SPEED_MAG * (SPEED_TIER_STEP_DENOMINATOR + tier * SPEED_TIER_STEP_NUMERATOR)
        / SPEED_TIER_STEP_DENOMINATOR);
}

static void AdvancePlayerTurn(u16 playerCount, u16 *activePlayer)
{
    if (playerCount != 2)
        return;
    *activePlayer = (*activePlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
}

void RunGame(u16 playerCount)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    u16 buttons;
    int lostLife;

    session.playerCount = playerCount;
    session.activePlayer = PLAYER_ONE;
    session.lives = 3;
    session.level = 1;
    session.score = 0;
    session.levelTileMap = 0;

    frame.paddleX = DEFAULT_PADDLE_X;
    frame.paddleY = DEFAULT_PADDLE_Y;

    while (session.lives > 0 && session.level <= 10) {
        if (session.level > 1)
            FadeOut(7, TRUE, FALSE);

        InitializeBlocksForLevel(session.level);
        session.levelTileMap = GetLevelTileMap(session.level);
        RefreshBackground(session.level, FALSE);
        ClearAllSprites();
        WaitVBlanks(20);
        FadeIn(7, TRUE, FALSE);
        UpdateLevelDisplay(session.level);
        UpdateLifeDisplay(session.lives);
        ResetActiveBonusPositions();
        InitializeRuntime(&runtime);
        RefreshBackground(session.level, runtime.shieldActive);
        AdvancePlayerTurn(session.playerCount, &session.activePlayer);

        if (session.lives == 0)
            break;

        {
            ServeState serve =
                RunServeState(frame.paddleX, frame.paddleY, session.activePlayer, session.score, &runtime);
            frame.paddleX = serve.paddleX;
            frame.ballX = serve.ballX;
            frame.ballY = frame.paddleY - runtime.ballHeight;
        }
        BallSyncFixedFromPixels(&frame);
        frame.ballVelX = FIX_ONE;
        frame.ballVelY = -FIX_ONE;
        BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, runtime.ballSpeedMag);

        while (session.lives > 0 && session.level <= 10) {
            u32 previousTier;
            u32 currentTier;

            UpdateFallingBonuses(&runtime, frame.paddleX, frame.paddleY, &session.lives);

            buttons = ReadJoypad();
            MovePaddleFromInput(buttons, &frame, &runtime);
            if (!(buttons & J_SELECT))
                break;

            /* Advance per-round timer and bump the ball speed when crossing a 30 s boundary. */
            previousTier = runtime.levelFrame / SPEED_TIER_FRAMES;
            ++runtime.levelFrame;
            currentTier = runtime.levelFrame / SPEED_TIER_FRAMES;
            if (currentTier != previousTier) {
                runtime.ballSpeedMag = SpeedMagForTier(currentTier);
                /* Snap the in-flight velocity to the new magnitude immediately so the ramp
                 * doesn't wait for the next bounce to take effect. */
                BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, runtime.ballSpeedMag);
            }

            IntegrateBallMotionThisFrame(&frame, &runtime, &session, &lostLife, session.levelTileMap,
                &session.score);
            if (lostLife)
                break;

            PresentGameplayFrame(&frame, &runtime, session.activePlayer, session.score, &session.lives);
            if (IsLevelCleared() == TRUE) {
                ++session.level;
                break;
            }
            RefreshBackground(session.level, runtime.shieldActive);
        }
    }
}

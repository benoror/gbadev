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
        BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);

        while (session.lives > 0 && session.level <= 10) {
            UpdateFallingBonuses(&runtime, frame.paddleX, frame.paddleY, &session.lives);

            buttons = ReadJoypad();
            MovePaddleFromInput(buttons, &frame, &runtime);
            if (!(buttons & J_SELECT))
                break;

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

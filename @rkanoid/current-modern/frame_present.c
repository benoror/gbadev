#include "frame_present.h"
#include "game_constants.h"
#include "game_sprites.h"
#include "gba_wait.h"

void PresentGameplayFrame(const FrameState *frame, const RuntimeState *runtime, u16 activePlayer, u32 score,
    u16 *lives)
{
    wait_vbl_done();
    SetPaddleSprite(frame->paddleX, frame->paddleY, runtime->longPaddle, activePlayer);
    SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, frame->ballVelX, frame->ballVelY,
        runtime->ballTileBase);
    UpdateScoreDisplay(score);
    if (*lives > MAX_LIVES)
        *lives = MAX_LIVES;
    UpdateLifeDisplay(*lives);
    /* Per-round time + speed level. 60 frames per second on GBA; speed level is 1 at the
     * start of every round (tier 0), then bumps by +1 every 30 s (matches the velocity
     * ramp in game_loop, which is +20 % per tier). */
    UpdateTimeDisplay(runtime->levelFrame / 60u);
    UpdateSpeedDisplay((u16)(1u + runtime->levelFrame / 1800u));
}

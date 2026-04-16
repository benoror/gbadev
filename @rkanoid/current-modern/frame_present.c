#include "frame_present.h"
#include "game_constants.h"
#include "game_sprites.h"
#include "gba_wait.h"

void PresentGameplayFrame(const FrameState *frame, const RuntimeState *runtime, u16 activePlayer, u32 score,
    u16 *lives)
{
    wait_vbl_done();
    SetPaddleSprite(frame->paddleX, frame->paddleY, runtime->longPaddle, activePlayer);
    if (frame->stepX < 0 && frame->stepY < 0)
        SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, 0, runtime->ballTileBase);
    if (frame->stepX > 0 && frame->stepY < 0)
        SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, 1, runtime->ballTileBase);
    if (frame->stepX < 0 && frame->stepY > 0)
        SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, 2, runtime->ballTileBase);
    if (frame->stepX > 0 && frame->stepY > 0)
        SetBallSprite(frame->ballX, frame->ballY, runtime->trailEnabled, 3, runtime->ballTileBase);
    UpdateScoreDisplay(score);
    if (*lives > MAX_LIVES)
        *lives = MAX_LIVES;
    UpdateLifeDisplay(*lives);
}

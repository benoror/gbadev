#include "regs.h"
#include "game_constants.h"
#include "physics.h"

u16 GetPaddleSpeed(u16 buttons)
{
    return (buttons & J_B) ? 3 : 4;
}

u16 ClampPaddleX(u16 paddleX, u16 paddleWidth)
{
    if (paddleX > MAX_X - paddleWidth)
        paddleX = MAX_X - paddleWidth;
    if (paddleX < MIN_X)
        paddleX = MIN_X;
    return paddleX;
}

void ApplyWorldBounds(FrameState *frame, RuntimeState *runtime, GameSession *session, int *lostLifeThisFrame)
{
    *lostLifeThisFrame = 0;

    if (frame->ballX >= (MAX_X - runtime->ballWidth))
        frame->stepX = -1;
    if (frame->ballX <= MIN_X)
        frame->stepX = 1;
    if (frame->ballY >= (runtime->maxBallY - runtime->ballHeight)) {
        frame->stepY = -1;
        if (runtime->shieldActive == FALSE) {
            --session->lives;
            *lostLifeThisFrame = 1;
            return;
        }
        runtime->shieldActive = FALSE;
        runtime->maxBallY = 160;
    }
    if (frame->ballY <= MIN_Y)
        frame->stepY = 1;
}

void MovePaddleFromInput(u16 buttons, FrameState *frame, const RuntimeState *runtime)
{
    if (!(buttons & J_RIGHT) && frame->paddleX < (MAX_X - runtime->paddleWidth))
        frame->paddleX += GetPaddleSpeed(buttons);
    if (!(buttons & J_LEFT) && frame->paddleX > MIN_X)
        frame->paddleX -= GetPaddleSpeed(buttons);
    frame->paddleX = ClampPaddleX(frame->paddleX, runtime->paddleWidth);
}

#include "regs.h"
#include "game_constants.h"
#include "serve_state.h"
#include "game_sprites.h"
#include "physics.h"

ServeState RunServeState(u16 paddleX, u16 paddleY, u16 playerIndex, u32 scoreCount, RuntimeState *runtime)
{
    ServeState state;
    u16 buttons;
    u16 paddleSpeed;

    state.paddleX = paddleX;
    state.ballX = paddleX + 13;
    SetPaddleSprite(state.paddleX, paddleY, runtime->longPaddle, playerIndex);
    SetBallSprite(state.ballX, paddleY - runtime->ballHeight, FALSE, 0, 0, BALL_TILE_SMALL);
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
        SetBallSprite(state.ballX, paddleY - runtime->ballHeight, FALSE, 0, 0, BALL_TILE_SMALL);
        UpdateScoreDisplay(scoreCount);
        wait_vbl_done();
    }

    return state;
}

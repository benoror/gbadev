#include "game_constants.h"
#include "bonuses.h"
#include "game_sprites.h"
#include "levels.h"
#include "level_state.h"
#include "gba_audio.h"

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

    SfxBonusPickup();
    HideBonus(block);
    HideBonusSprite(block->bonusSlot);
}

void UpdateFallingBonuses(RuntimeState *runtime, u16 paddleX, u16 paddleY, u16 *lifeCount)
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

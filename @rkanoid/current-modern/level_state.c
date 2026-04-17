#include "level_state.h"

#define OFFSCREEN_X 240
#define OFFSCREEN_Y 160

Block gBlocks[162];

void ResetBlock(Block *block)
{
    block->x = 0;
    block->y = 0;
    block->right = 0;
    block->bottom = 0;
    block->tileIndex = 0;
    block->tileIndex2 = 0;
    block->destructible = FALSE;
    block->hitsRemaining = 0;
    block->bonusType = NO_BONUS;
    block->bonusActive = FALSE;
    block->bonusX = OFFSCREEN_X + 8;
    block->bonusY = OFFSCREEN_Y + 8;
    block->bonusSlot = 0;
}

void HideBonus(Block *block)
{
    block->bonusType = NO_BONUS;
    block->bonusActive = FALSE;
    block->bonusX = OFFSCREEN_X;
    block->bonusY = OFFSCREEN_Y;
}

void InitializeBlocksForLevel(u16 level)
{
    const BlockSeed *seeds;
    u16 blockCount;
    u16 index;
    u16 bonusSlot;

    LoadLevelTileMap(level);

    for (index = 0; index < 162; ++index) {
        ResetBlock(&gBlocks[index]);
    }

    seeds = GetLevelBlockSeeds(level, &blockCount);
    for (index = 0; index < blockCount; ++index) {
        const BlockSeed *seed = &seeds[index];
        Block *block = &gBlocks[seed->slot];

        block->x = seed->x;
        block->y = seed->y;
        block->tileIndex = seed->tileIndex;
        block->destructible = seed->destructible;
        block->hitsRemaining = seed->hitsRemaining;
        block->bonusType = seed->bonusType;
    }

    for (index = 0, bonusSlot = 0; index < 162; ++index) {
        Block *block = &gBlocks[index];

        block->right = block->x + 16;
        block->bottom = block->y + 8;
        block->tileIndex2 = block->tileIndex + 1;
        if (block->bonusType != NO_BONUS) {
            block->bonusX = block->x;
            block->bonusY = block->y;
            block->bonusSlot = bonusSlot;
            bonusSlot += 2;
        }
    }
}

void ResetActiveBonusPositions(void)
{
    u16 index;

    for (index = 0; index < 162; ++index) {
        Block *block = &gBlocks[index];
        if (block->bonusType != NO_BONUS && block->bonusActive == TRUE) {
            block->bonusX = OFFSCREEN_X;
            block->bonusY = OFFSCREEN_Y;
        }
    }
}

u16 IsLevelCleared(void)
{
    u16 index;

    for (index = 0; index < 162; ++index) {
        if (gBlocks[index].x != 0 && gBlocks[index].destructible == TRUE) {
            return FALSE;
        }
    }

    return TRUE;
}

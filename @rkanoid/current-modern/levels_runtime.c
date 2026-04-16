#include "regs.h"
#include "levels_runtime.h"

#define LEVEL_WIDTH 18
#define LEVEL_HEIGHT 18
#define BACKGROUND_WIDTH 30
#define OFFSCREEN_X 240
#define OFFSCREEN_Y 160

Block gBlocks[162];

u8 gBackgroundMap[] = {
1,0,10,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	11,1,0,1,0,1,0,1,0,

1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,40,41,42,43,44,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,8,9,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,45,46,47,48,49,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,
1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,

1,0,14,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	TILE_EMPTY_1,TILE_EMPTY_2,	15,1,0,1,0,1,0,1,0,1
};

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

void ApplyShieldFloor(void)
{
    u16 index;

    for (index = 543; index <= 560; index += 2) {
        gBackgroundMap[index] = TILE_SHIELD_1;
        gBackgroundMap[index + 1] = TILE_SHIELD_2;
    }
}

void CopyLevelTileMapToBackground(u16 level)
{
    u16 x;
    u16 y;
    u8 *levelTileMap = GetLevelTileMap(level);

    for (y = 0; y < LEVEL_HEIGHT; ++y)
        for (x = 0; x < LEVEL_WIDTH; ++x)
            gBackgroundMap[x + 3 + ((y + 1) * BACKGROUND_WIDTH)] = levelTileMap[x + (y * LEVEL_WIDTH)];
}

void CopyBackgroundMapToVram(void)
{
    u16 x;
    u16 y;

    for (x = 0; x < 30; ++x)
        for (y = 0; y < 20; ++y)
            VBUFFER[x + (y * 32)] = gBackgroundMap[x + (y * 30) + 1];
}

void InitializeBlocksForLevel(u16 level)
{
    const BlockSeed *seeds;
    u16 blockCount;
    u16 index;
    u16 bonusSlot;

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

#include "regs.h"
#include "assets.h"
#include "screens.h"

#define FULLSCREEN_TILE_COUNT 19200
#define GAMEPLAY_TILE_COUNT 1600

static void CopyIdentityTileMap(void)
{
    u16 x;
    u16 y;

    for (y = 0; y < 20; ++y)
        for (x = 0; x < 30; ++x)
            VBUFFER[y * 32 + x] = y * 30 + x;
}

static void PresentFullscreenScreen(const u16 *tiles, const u16 *palette)
{
    u16 index;

    for (index = 0; index < 256; ++index)
        BG_PLTT[index] = palette[index];
    for (index = 0; index < FULLSCREEN_TILE_COUNT; ++index)
        BG0_DATA[index] = tiles[index];
    CopyIdentityTileMap();
}

void InitializeDisplay(void)
{
    BG0CNT = 0x00C7;
    BG1CNT = 0x0285;
    DISPCNT = 0x1140;
}

void LoadSpriteGraphics(void)
{
    Dma3Transfer((u32)&spritePalette, (u32)OBJ_PLTT, 0x84000080);
    Dma3Transfer((u32)&spriteTiles, (u32)OBJCHARDATA, 0x84000980);
}

void LoadGameplayBackground(void)
{
    u16 index;

    for (index = 0; index < 256; ++index)
        BG_PLTT[index] = backgroundPalette[index];
    for (index = 0; index < GAMEPLAY_TILE_COUNT; ++index)
        BG0_DATA[index] = backgroundTiles[index];
}

void ShowSplashScreen(void)
{
    PresentFullscreenScreen((u16 *)&splashScreenTiles, (u16 *)&splashScreenPalette);
}

void ShowTitleScreen(void)
{
    PresentFullscreenScreen((u16 *)&titleScreenTiles, (u16 *)&titleScreenPalette);
}

void ShowGameOverScreen(void)
{
    PresentFullscreenScreen((u16 *)&gameOverScreenTiles, (u16 *)&gameOverScreenPalette);
}

void ShowEndingScreen(void)
{
    PresentFullscreenScreen((u16 *)&endingScreenTiles, (u16 *)&endingScreenPalette);
}

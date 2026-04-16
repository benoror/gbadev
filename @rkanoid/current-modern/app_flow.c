#include "regs.h"
#include "game_sprites.h"
#include "gameplay.h"
#include "menu.h"
#include "screens.h"

void C_Entry(void)
{
    u16 buttons;
    u16 players;

    InitializeDisplay();
    ShowSplashScreen();
    FadeIn(30, FALSE, TRUE);
    WaitVBlanks(100);
    FadeOut(30, FALSE, FALSE);

    ShowTitleScreen();
    FadeIn(30, FALSE, FALSE);
    ClearAllSprites();
    LoadSpriteGraphics();

    players = RunMainMenu();

    FadeOut(30, TRUE, FALSE);
    LoadGameplayBackground();
    LoadSpriteGraphics();
    RunGame(players);

    FadeOut(30, TRUE, FALSE);
    ShowGameOverScreen();
    FadeIn(30, TRUE, FALSE);
    buttons = ReadJoypad();
    while (buttons & J_START)
        buttons = ReadJoypad();
    FadeOut(30, TRUE, FALSE);
}

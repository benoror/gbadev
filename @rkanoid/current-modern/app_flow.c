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
    InitAudio();
    ShowSplashScreen();
    /* Splash runs at half the speed of the rest of the flow so the title lingers. */
    FadeIn(28, FALSE, TRUE);
    WaitVBlanks(600);
    FadeOut(28, FALSE, FALSE);

    ShowTitleScreen();
    FadeIn(7, FALSE, FALSE);
    ClearAllSprites();
    LoadSpriteGraphics();

    players = RunMainMenu();

    FadeOut(7, TRUE, FALSE);
    LoadGameplayBackground();
    LoadSpriteGraphics();
    RunGame(players);

    FadeOut(7, TRUE, FALSE);
    ShowGameOverScreen();
    FadeIn(7, TRUE, FALSE);
    buttons = ReadJoypad();
    while (buttons & J_START)
        buttons = ReadJoypad();
    FadeOut(7, TRUE, FALSE);
}

#include "regs.h"
#include "menu.h"

#define MENU_CURSOR_LEFT_SLOT 0
#define MENU_CURSOR_RIGHT_SLOT 1
#define MENU_START_X 78
#define MENU_PLAYER_ONE 1
#define MENU_PLAYER_TWO 2

u16 RunMainMenu(void)
{
    static const u16 optionY[] = {0, 90, 103, 116};
    u16 optionIndex = 1;
    u16 buttons;

    while (1) {
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib0 = 0x2000 + optionY[optionIndex];
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib1 = MENU_START_X - 8;
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib2 = 0x0800 + 42;

        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib0 = 0x2000 + optionY[optionIndex];
        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib1 = MENU_START_X;
        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib2 = 0x0800 + 44;

        buttons = ReadJoypad();
        if (!(buttons & J_DOWN))
            ++optionIndex;
        if (!(buttons & J_UP))
            --optionIndex;
        if (optionIndex > 3)
            optionIndex = 1;
        if (optionIndex < 1)
            optionIndex = 3;

        if (!(buttons & J_START)) {
            if (optionIndex == 1)
                return MENU_PLAYER_ONE;
            if (optionIndex == 2)
                return MENU_PLAYER_TWO;
        }
    }
}

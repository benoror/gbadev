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
    u16 prevRaw;
    u16 curDown;
    u16 prevDown;
    u16 hit;

    /* KEYINPUT: bit clear while held; one move per press, not per frame while held. */
    prevRaw = ReadJoypad();
    while (1) {
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib0 = 0x2000 + optionY[optionIndex];
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib1 = MENU_START_X - 8;
        OAM[MENU_CURSOR_LEFT_SLOT].Attrib2 = 0x0800 + 42;

        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib0 = 0x2000 + optionY[optionIndex];
        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib1 = MENU_START_X;
        OAM[MENU_CURSOR_RIGHT_SLOT].Attrib2 = 0x0800 + 44;

        buttons = ReadJoypad();
        curDown = (~buttons) & 0x03FFU;
        prevDown = (~prevRaw) & 0x03FFU;
        hit = (u16)((curDown ^ prevDown) & curDown);
        prevRaw = buttons;

        if (hit & J_DOWN)
            ++optionIndex;
        if (hit & J_UP)
            --optionIndex;
        if (optionIndex > 3)
            optionIndex = 1;
        if (optionIndex < 1)
            optionIndex = 3;

        if (hit & J_START) {
            if (optionIndex == 1)
                return MENU_PLAYER_ONE;
            if (optionIndex == 2)
                return MENU_PLAYER_TWO;
        }

        wait_vbl_done();
    }
}

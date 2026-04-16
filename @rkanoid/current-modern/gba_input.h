#ifndef RKANOID_GBA_INPUT_H
#define RKANOID_GBA_INPUT_H

#include "gba_registers.h"

static inline u16 ReadJoypad(void)
{
    u16 buttons = P1;
    P1 |= 0x03FF;
    return buttons;
}

#endif

#ifndef RKANOID_TEST_REGS_H
#define RKANOID_TEST_REGS_H

#include "game_types.h"
#include "gba_types.h"

/* Host-test stubs for hardware registers and input masks. The real regs.h is replaced by this file
 * when the test binary is compiled (-Itests comes first in the include search path). */

#define J_A 0x0001
#define J_B 0x0002
#define J_SELECT 0x0004
#define J_START 0x0008
#define J_RIGHT 0x0010
#define J_LEFT 0x0020
#define J_UP 0x0040
#define J_DOWN 0x0080
#define J_R 0x0100
#define J_L 0x0200

#endif

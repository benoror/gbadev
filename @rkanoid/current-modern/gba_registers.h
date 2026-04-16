#ifndef RKANOID_GBA_REGISTERS_H
#define RKANOID_GBA_REGISTERS_H

#include "gba_types.h"

#define DISPCNT *(volatile u32 *)0x04000000
#define BG0CNT *(volatile u16 *)0x04000008
#define BG1CNT *(volatile u16 *)0x0400000A
#define BG2CNT *(volatile u16 *)0x0400000C
#define BG3CNT *(volatile u16 *)0x0400000E
#define DMA3SAD *(volatile u32 *)0x040000D4
#define DMA3DAD *(volatile u32 *)0x040000D8
#define DMA3CNT *(volatile u32 *)0x040000DC
#define P1 *(volatile u16 *)0x04000130
#define OAM ((volatile Sprite *)0x07000000)
#define OBJCHARDATA ((volatile u8 *)0x06010000)
#define OBJ_PLTT ((volatile u16 *)0x05000200)
#define BG0_DATA ((volatile u16 *)0x06004000)
#define BG_PLTT ((volatile u16 *)0x05000000)
#define VBUFFER ((volatile u16 *)0x06000000)
#define BLDMOD *(volatile u16 *)0x04000050
#define COLEV *(volatile u16 *)0x04000052
#define COLY *(volatile u16 *)0x04000054
#define J_RIGHT 0x0010
#define J_LEFT 0x0020
#define J_UP 0x0040
#define J_DOWN 0x0080
#define J_START 0x0008
#define J_SELECT 0x0004
#define J_A 0x0001
#define J_B 0x0002
#define J_L 0x0200
#define J_R 0x0100

#endif

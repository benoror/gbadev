#ifndef RKANOID_GBA_TYPES_H
#define RKANOID_GBA_TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef struct Sprite
{
    u16 Attrib0;
    u16 Attrib1;
    u16 Attrib2;
    u16 RotateScale;
} Sprite;

#endif

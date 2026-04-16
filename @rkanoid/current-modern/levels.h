#ifndef RKANOID_LEVELS_H
#define RKANOID_LEVELS_H

#include "game_types.h"

#define TILE_BRICK_PRESSED_1 2
#define TILE_BRICK_PRESSED_2 3
#define TILE_EMPTY_1 4
#define TILE_EMPTY_2 5
#define TILE_SHIELD_1 6
#define TILE_SHIELD_2 7

#define TILE_RED_1 18
#define TILE_RED_2 19
#define TILE_GREEN_1 20
#define TILE_GREEN_2 21
#define TILE_YELLOW_1 22
#define TILE_YELLOW_2 23
#define TILE_BLUE_1 24
#define TILE_BLUE_2 25
#define TILE_BROWN_1 26
#define TILE_BROWN_2 27
#define TILE_GRAY_1 28
#define TILE_GRAY_2 29
#define TILE_PURPLE_1 30
#define TILE_PURPLE_2 31
#define TILE_PINK_1 32
#define TILE_PINK_2 33
#define TILE_SMILE_1 34
#define TILE_SMILE_2 35
#define TILE_TITANIUM_1 36
#define TILE_TITANIUM_2 37
#define TILE_IRIS_1 38
#define TILE_IRIS_2 39

#define NO_BONUS 666
#define COMET_BONUS 52
#define LONG_PADDLE_BONUS 56
#define EXTRA_LIFE_BONUS 60
#define BIG_BALL_BONUS 64
#define SHIELD_BONUS 68

typedef struct
{
    int x;
    int y;
    int right;
    int bottom;
    int tileIndex;
    int tileIndex2;
    boolean destructible;
    u8 hitsRemaining;
    u16 bonusType;
    u16 bonusActive;
    u16 bonusX;
    u16 bonusY;
    u16 bonusSlot;
} Block;

extern Block gBlocks[162];

#endif

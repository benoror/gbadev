#ifndef RKANOID_LEVELS_H
#define RKANOID_LEVELS_H

#include "regs.h"

#define O1 2
#define O2 3
#define N1 4
#define N2 5
#define mes1 6
#define mes2 7

#define TILE_BRICK_PRESSED_1 O1
#define TILE_BRICK_PRESSED_2 O2
#define TILE_EMPTY_1 N1
#define TILE_EMPTY_2 N2
#define TILE_SHIELD_1 mes1
#define TILE_SHIELD_2 mes2

#define NO_BONUS 666
#define COMET_BONUS 52
#define LONG_R_BONUS 56
#define LIFE_BONUS 60
#define BIGBALL_BONUS 64
#define MESIAS_BONUS 68

#define LONG_PADDLE_BONUS LONG_R_BONUS
#define EXTRA_LIFE_BONUS LIFE_BONUS
#define BIG_BALL_BONUS BIGBALL_BONUS
#define SHIELD_BONUS MESIAS_BONUS

#define TRUE 1
#define FALSE 0

typedef int boolean;

typedef struct
{
	int x, y, xM, yM, tile1, tile2;
	boolean destruye;
	u8 golpes;
	u16 BONUS, activoBonus, bonX, bonY, bonN;
} Block;

extern Block L[162];
extern u8 bgMap[];
extern u8 blockMap_1[];
extern u8 blockMap_2[];
extern u8 blockMap_3[];
extern u8 blockMap_4[];
extern u8 blockMap_5[];

void ApplyShieldFloor(void);
void RefreshBlockMap(u16 level);
void InitializeBlockProperties(u16 level);
void ResetActiveBonusPositions(void);
u16 IsLevelCleared(void);

#endif

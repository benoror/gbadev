#include "regs.h"
#include "game_sprites.h"
#include "ball_motion.h"

#define OFFSCREEN_X 240
#define OFFSCREEN_Y 160
#define OAM_CAPACITY 128
#define SCORE_SLOT_START 1
#define LEVEL_TENS_SLOT 0
#define LEVEL_SLOT 8
#define PADDLE_SLOT_START 9
/* HUD entries on the right edge below the level readout. Slot numbers are above the bonus
 * range (23+) and well clear of paddle/life/ball/trail slots, so they never collide. */
#define TIME_SLOT_START 60     /* 3 slots: hundreds, tens, ones of seconds */
#define SPEED_TENS_SLOT 64     /* Hidden while the level is < 10 (matches UpdateLevelDisplay). */
#define SPEED_SLOT 65          /* Ones digit of the speed level. */
#define PADDLE_SLOT_COUNT 6
#define LIFE_SLOT_START 15
#define BALL_SLOT_HEAD 17
#define BALL_TRAIL_SLOT_COUNT 6
#define BONUS_SLOT_START 23
#define DIGIT_TILE_BASE 10
#define PLAYER_ONE 1
#define PLAYER_TWO 2

typedef struct
{
    signed char xOffset;
    u16 centerTile;
} PaddleSegment;

/* Tile per trail slot (head + 5 fade steps). Previously varied per direction but is the same
 * across all four legacy layouts — the direction only changed the positional offsets. */
static const u16 kTrailTiles[BALL_TRAIL_SLOT_COUNT] = {30, 30, 32, 34, 36, 38};

/* Pixel spacing between consecutive trail slots along the motion direction. */
#define TRAIL_STEP_SPACING 2

static const PaddleSegment kShortPaddle[PADDLE_SLOT_COUNT] = {
    {0, 0}, {8, 2}, {16, 4}, {24, 6}, {0, 4}, {0, 6},
};

static const PaddleSegment kLongPaddle[PADDLE_SLOT_COUNT] = {
    {0, 0}, {8, 4}, {16, 2}, {24, 4}, {32, 2}, {40, 6},
};

static void HideSpriteSlot(u16 slot)
{
    OAM[slot].Attrib0 = 0x2000 + OFFSCREEN_Y + 8;
    OAM[slot].Attrib1 = OFFSCREEN_X + 8;
}

static void FormatTwoDigits(u16 value, u16 digits[2])
{
    digits[0] = (value / 10) % 10;
    digits[1] = value % 10;
}

static void FormatSevenDigits(u32 value, u16 digits[7])
{
    u16 index;

    for (index = 0; index < 7; ++index)
        digits[index] = 20;
    for (index = 0; index < 7; ++index) {
        digits[6 - index] = value % 10;
        value /= 10;
        if (value == 0)
            break;
    }
}

void ClearAllSprites(void)
{
    u16 slot;

    for (slot = 0; slot < OAM_CAPACITY; ++slot)
        HideSpriteSlot(slot);
}

void SetBallSprite(u16 x, u16 y, boolean trailEnabled, s32 velX, s32 velY, u16 ballTileBase)
{
    u16 step;

    if (trailEnabled == TRUE) {
        for (step = 0; step < BALL_TRAIL_SLOT_COUNT; ++step) {
            s32 offX;
            s32 offY;
            BallComputeTrailOffset(velX, velY, BALL_SPEED_MAG, step, TRAIL_STEP_SPACING, &offX, &offY);
            OAM[BALL_SLOT_HEAD + step].Attrib0 = 0x2000 + (u16)((s32)y + offY);
            OAM[BALL_SLOT_HEAD + step].Attrib1 = (u16)((s32)x + offX);
            OAM[BALL_SLOT_HEAD + step].Attrib2 = kTrailTiles[step];
        }
        return;
    }

    for (step = 1; step < BALL_TRAIL_SLOT_COUNT; ++step)
        HideSpriteSlot(BALL_SLOT_HEAD + step);
    OAM[BALL_SLOT_HEAD].Attrib0 = 0x2000 + y;
    OAM[BALL_SLOT_HEAD].Attrib1 = x;
    OAM[BALL_SLOT_HEAD].Attrib2 = ballTileBase;
}

void UpdateBonusSprite(u16 x, u16 y, u16 tileBase, u16 spriteOffset)
{
    OAM[BONUS_SLOT_START + spriteOffset].Attrib0 = 0x2000 + y;
    OAM[BONUS_SLOT_START + spriteOffset].Attrib1 = x;
    OAM[BONUS_SLOT_START + spriteOffset].Attrib2 = tileBase;

    OAM[BONUS_SLOT_START + spriteOffset + 1].Attrib0 = 0x2000 + y;
    OAM[BONUS_SLOT_START + spriteOffset + 1].Attrib1 = x + 8;
    OAM[BONUS_SLOT_START + spriteOffset + 1].Attrib2 = tileBase + 2;
}

void HideBonusSprite(u16 spriteOffset)
{
    HideSpriteSlot(BONUS_SLOT_START + spriteOffset);
    HideSpriteSlot(BONUS_SLOT_START + spriteOffset + 1);
}

void UpdateLifeDisplay(u16 lives)
{
    u16 digits[2];

    FormatTwoDigits(lives, digits);
    OAM[LIFE_SLOT_START].Attrib0 = 0x2000 + 56;
    OAM[LIFE_SLOT_START].Attrib1 = 176;
    OAM[LIFE_SLOT_START].Attrib2 = DIGIT_TILE_BASE + (digits[0] * 2);

    OAM[LIFE_SLOT_START + 1].Attrib0 = 0x2000 + 56;
    OAM[LIFE_SLOT_START + 1].Attrib1 = 184;
    OAM[LIFE_SLOT_START + 1].Attrib2 = DIGIT_TILE_BASE + (digits[1] * 2);
}

void UpdateScoreDisplay(u32 score)
{
    u16 digits[7];
    u16 slot;

    FormatSevenDigits(score, digits);
    for (slot = 0; slot < 7; ++slot) {
        OAM[SCORE_SLOT_START + slot].Attrib0 = 0x2000 + 24;
        OAM[SCORE_SLOT_START + slot].Attrib1 = 176 + (slot * 8);
        OAM[SCORE_SLOT_START + slot].Attrib2 = DIGIT_TILE_BASE + (digits[slot] * 2);
    }
}

void UpdateLevelDisplay(u16 level)
{
    u16 digits[2];

    FormatTwoDigits(level, digits);
    if (digits[0] != 0) {
        OAM[LEVEL_TENS_SLOT].Attrib0 = 0x2000 + 88;
        OAM[LEVEL_TENS_SLOT].Attrib1 = 168;
        OAM[LEVEL_TENS_SLOT].Attrib2 = DIGIT_TILE_BASE + (digits[0] * 2);
    } else {
        HideSpriteSlot(LEVEL_TENS_SLOT);
    }
    OAM[LEVEL_SLOT].Attrib0 = 0x2000 + 88;
    OAM[LEVEL_SLOT].Attrib1 = 176;
    OAM[LEVEL_SLOT].Attrib2 = DIGIT_TILE_BASE + (digits[1] * 2);
}

void UpdateTimeDisplay(u32 seconds)
{
    u16 digits[3];
    u16 i;

    if (seconds > 999)
        seconds = 999;
    digits[0] = (u16)((seconds / 100) % 10);
    digits[1] = (u16)((seconds / 10) % 10);
    digits[2] = (u16)(seconds % 10);
    for (i = 0; i < 3; ++i) {
        OAM[TIME_SLOT_START + i].Attrib0 = 0x2000 + 104;
        OAM[TIME_SLOT_START + i].Attrib1 = 168 + i * 8;
        OAM[TIME_SLOT_START + i].Attrib2 = DIGIT_TILE_BASE + (digits[i] * 2);
    }
}

void UpdateSpeedDisplay(u16 speedLevel)
{
    u16 digits[2];

    /* speedLevel is a human-readable integer: 1 at round start, +1 per tier. Capped at 99. */
    if (speedLevel > 99)
        speedLevel = 99;
    FormatTwoDigits(speedLevel, digits);
    if (digits[0] != 0) {
        OAM[SPEED_TENS_SLOT].Attrib0 = 0x2000 + 120;
        OAM[SPEED_TENS_SLOT].Attrib1 = 168;
        OAM[SPEED_TENS_SLOT].Attrib2 = DIGIT_TILE_BASE + (digits[0] * 2);
    } else {
        HideSpriteSlot(SPEED_TENS_SLOT);
    }
    OAM[SPEED_SLOT].Attrib0 = 0x2000 + 120;
    OAM[SPEED_SLOT].Attrib1 = 176;
    OAM[SPEED_SLOT].Attrib2 = DIGIT_TILE_BASE + (digits[1] * 2);
}

void SetPaddleSprite(u16 x, u16 y, boolean longPaddle, u16 playerIndex)
{
    const PaddleSegment *segments = longPaddle ? kLongPaddle : kShortPaddle;
    u16 slot;
    u16 leftCapTile = (playerIndex == PLAYER_ONE) ? 0x0800 + 0 : 0x0800 + 46;
    u16 rightCapTile = (playerIndex == PLAYER_ONE) ? 0x0800 + 6 : 0x0800 + 48;

    for (slot = 0; slot < PADDLE_SLOT_COUNT; ++slot) {
        OAM[PADDLE_SLOT_START + slot].Attrib0 = 0x2000 + y;
        OAM[PADDLE_SLOT_START + slot].Attrib1 = x + segments[slot].xOffset;
        OAM[PADDLE_SLOT_START + slot].Attrib2 = 0x0800 + segments[slot].centerTile;
    }

    OAM[PADDLE_SLOT_START].Attrib2 = leftCapTile;
    OAM[PADDLE_SLOT_START + PADDLE_SLOT_COUNT - 1].Attrib2 = rightCapTile;
    if (longPaddle == FALSE) {
        HideSpriteSlot(PADDLE_SLOT_START + 4);
        HideSpriteSlot(PADDLE_SLOT_START + 5);
        OAM[PADDLE_SLOT_START + 3].Attrib0 = 0x2000 + y;
        OAM[PADDLE_SLOT_START + 3].Attrib1 = x + 24;
        OAM[PADDLE_SLOT_START + 3].Attrib2 = rightCapTile;
    }
}

#include "gba_audio.h"

/*
 * Tiny PSG SFX engine. Uses GBA Direct-Sound-free PSG channels:
 *   - Square 1 (with frequency sweep)
 *   - Square 2
 *   - Noise (channel 4)
 *
 * All effects are single-shot register writes: we set length / envelope / duty
 * and toggle the "initial" trigger bit. The hardware plays the tone until the
 * envelope decays or the length flag expires; the CPU never blocks.
 */

#define REG_SOUND1CNT_L (*(volatile u16 *)0x04000060)
#define REG_SOUND1CNT_H (*(volatile u16 *)0x04000062)
#define REG_SOUND1CNT_X (*(volatile u16 *)0x04000064)
#define REG_SOUND2CNT_L (*(volatile u16 *)0x04000068)
#define REG_SOUND2CNT_H (*(volatile u16 *)0x0400006C)
#define REG_SOUND4CNT_L (*(volatile u16 *)0x04000078)
#define REG_SOUND4CNT_H (*(volatile u16 *)0x0400007C)
#define REG_SOUNDCNT_L  (*(volatile u16 *)0x04000080)
#define REG_SOUNDCNT_H  (*(volatile u16 *)0x04000082)
#define REG_SOUNDCNT_X  (*(volatile u16 *)0x04000084)
#define REG_SOUNDBIAS   (*(volatile u16 *)0x04000088)

#define SND_TRIGGER     0x8000u
#define SND_LENGTH_FLAG 0x4000u

/* GBA frequency rate field is 11 bits: Hz = 131072 / (2048 - rate). */
static u16 PsgRate(u32 hz)
{
    if (hz == 0)
        return 0;
    return (u16)(2048u - (131072u / hz));
}

/* envInit:0..15  envStep:0..7 (decay every step/64s)  duty:0..3  length:0..63 */
static void Square2(u32 hz, u16 duty, u16 envInit, u16 envStep, u16 length, u16 useLength)
{
    REG_SOUND2CNT_L = (u16)((envInit << 12) | (envStep << 8) | (duty << 6) | (length & 0x3F));
    REG_SOUND2CNT_H = (u16)(SND_TRIGGER | (useLength ? SND_LENGTH_FLAG : 0u) | PsgRate(hz));
}

static void Square1Sweep(u32 hz, u16 duty, u16 envInit, u16 envStep, u16 length, u16 useLength,
    u16 sweepShift, u16 sweepDir, u16 sweepTime)
{
    REG_SOUND1CNT_L = (u16)(((sweepTime & 0x07) << 4) | ((sweepDir & 0x01) << 3) | (sweepShift & 0x07));
    REG_SOUND1CNT_H = (u16)((envInit << 12) | (envStep << 8) | (duty << 6) | (length & 0x3F));
    REG_SOUND1CNT_X = (u16)(SND_TRIGGER | (useLength ? SND_LENGTH_FLAG : 0u) | PsgRate(hz));
}

static void Noise(u16 envInit, u16 envStep, u16 length, u16 useLength, u16 freqDiv, u16 freqShift, u16 width)
{
    REG_SOUND4CNT_L = (u16)((envInit << 12) | (envStep << 8) | (length & 0x3F));
    REG_SOUND4CNT_H = (u16)(SND_TRIGGER | (useLength ? SND_LENGTH_FLAG : 0u)
        | ((freqShift & 0x0F) << 4) | ((width & 0x01) << 3) | (freqDiv & 0x07));
}

void InitAudio(void)
{
    REG_SOUNDCNT_X = SND_TRIGGER >> 8;            /* master enable (bit 7) */
    REG_SOUNDCNT_L = 0xFF77;                       /* full L+R volume, all PSG chans routed both sides */
    REG_SOUNDCNT_H = 0x0002;                       /* PSG @ 100%, no Direct Sound */
    REG_SOUNDBIAS  = 0x0200;                       /* default sample rate / DC bias */
}

void SfxPaddleHit(void)
{
    /* ~30 ms bright square tick. */
    Square2(880, 2, 12, 0, 56, 1);
}

void SfxBrickHit(void)
{
    /* ~16 ms crisp click. */
    Noise(12, 0, 60, 1, 0, 4, 1);
}

void SfxWallBounce(void)
{
    /* Low, soft tick — used sparingly so we don't drown out brick hits. */
    Square2(330, 1, 9, 1, 58, 1);
}

void SfxBonusPickup(void)
{
    /* Rising sweep starting at C5; sweep makes it slide up fast. */
    Square1Sweep(523, 2, 14, 3, 0, 0,
        2,    /* shift: large jumps */
        0,    /* direction up       */
        1);   /* every 7.8 ms       */
}

void SfxLifeLost(void)
{
    /* Slow descending square at low pitch. Envelope decays naturally. */
    Square1Sweep(220, 1, 14, 5, 0, 0,
        3,    /* shift: noticeable drop  */
        1,    /* direction down          */
        4);   /* every 31 ms             */
}

void SfxLevelClear(void)
{
    /* Major-fifth fanfare: C5 + G5 sustained together for ~1.1 s. */
    Square1Sweep(523, 2, 13, 5, 0, 0, 0, 0, 0);
    Square2     (784, 2, 13, 5, 0, 0);
}

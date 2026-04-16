#ifndef RKANOID_GBA_AUDIO_H
#define RKANOID_GBA_AUDIO_H

#include "gba_wait.h"

static inline void PlayTone(u16 delayFrames, int sound)
{
    u16 index;
    u16 tones[2];

    tones[0] = sound;
    tones[1] = 0;
    for (index = 0; index < 2; ++index) {
        WaitVBlanks(delayFrames);
        *(u16 *)0x400006C = tones[index];
    }
}

#endif

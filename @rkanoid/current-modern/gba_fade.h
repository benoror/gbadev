#ifndef RKANOID_GBA_FADE_H
#define RKANOID_GBA_FADE_H

#include "gba_registers.h"
#include "gba_wait.h"
#include "game_types.h"

static inline void ConfigureFade(boolean includeSprites, boolean fadeToWhite)
{
    if (includeSprites)
        BLDMOD = fadeToWhite ? 0x00BF : 0x00FF;
    else
        BLDMOD = fadeToWhite ? 0x00AF : 0x00EF;
}

static inline void FadeOut(u8 delayFrames, boolean includeSprites, boolean fadeToWhite)
{
    u8 phase;
    u8 delay;

    ConfigureFade(includeSprites, fadeToWhite);
    for (phase = 0; phase < 20; ++phase) {
        COLY = phase;
        for (delay = 0; delay != delayFrames; ++delay) {
            wait_vbl_done();
            wait_vbl_done();
            wait_vbl_done();
        }
    }
}

static inline void FadeIn(u8 delayFrames, boolean includeSprites, boolean fadeToWhite)
{
    int phase;
    u8 delay;

    ConfigureFade(includeSprites, fadeToWhite);
    for (phase = 20; phase >= 0; --phase) {
        COLY = phase;
        for (delay = 0; delay != delayFrames; ++delay) {
            wait_vbl_done();
            wait_vbl_done();
            wait_vbl_done();
        }
    }
}

#endif

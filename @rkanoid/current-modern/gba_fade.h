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

/*
 * Fade loops advance the COLY blend coefficient and wait `delayFrames` VBlanks per phase.
 * The COLY fade saturates at 16; phases 17–19 are kept as visual settle time. The inner
 * loop historically waited 3x per delayFrame which made every fade take ~3× longer than
 * what the call-site value suggested — dropped to a single wait so delayFrames means what
 * it says (number of VBlanks per phase).
 */
static inline void FadeOut(u8 delayFrames, boolean includeSprites, boolean fadeToWhite)
{
    u8 phase;
    u8 delay;

    ConfigureFade(includeSprites, fadeToWhite);
    for (phase = 0; phase < 20; ++phase) {
        COLY = phase;
        for (delay = 0; delay != delayFrames; ++delay)
            wait_vbl_done();
    }
}

static inline void FadeIn(u8 delayFrames, boolean includeSprites, boolean fadeToWhite)
{
    int phase;
    u8 delay;

    ConfigureFade(includeSprites, fadeToWhite);
    for (phase = 20; phase >= 0; --phase) {
        COLY = phase;
        for (delay = 0; delay != delayFrames; ++delay)
            wait_vbl_done();
    }
}

#endif

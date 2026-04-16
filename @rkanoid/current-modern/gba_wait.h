#ifndef RKANOID_GBA_WAIT_H
#define RKANOID_GBA_WAIT_H

#include "gba_types.h"

void wait_vbl_done(void);

static inline void WaitVBlanks(u32 count)
{
    u8 i;
    u8 sink = 0;
    u32 frame;

    for (frame = 0; frame < count; ++frame) {
        wait_vbl_done();
        for (i = 0; i < 100; ++i)
            sink = i * 32 / 3;
    }
    (void)sink;
}

#endif

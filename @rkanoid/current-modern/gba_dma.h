#ifndef RKANOID_GBA_DMA_H
#define RKANOID_GBA_DMA_H

#include "gba_registers.h"

static inline void Dma3Transfer(u32 source, u32 destination, u32 control)
{
    DMA3SAD = source;
    DMA3DAD = destination;
    DMA3CNT = control;
}

#endif

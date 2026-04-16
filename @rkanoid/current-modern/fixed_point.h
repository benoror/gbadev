#ifndef RKANOID_FIXED_POINT_H
#define RKANOID_FIXED_POINT_H

#include "gba_types.h"

#define FIX_SHIFT 8
#define FIX_ONE ((s32)256)
#define FIX_HALF ((s32)128)

#define FIX_FROM_INT(i) ((s32)(i) << FIX_SHIFT)
#define FIX_MUL(a, b) ((s32)(((long long)(a) * (long long)(b)) >> FIX_SHIFT))

/* Round half away from zero for screen pixel sync. */
#define FIX_TO_INT_ROUND(x) \
    ((s32)(((x) >= 0 ? (x) + FIX_HALF : (x)-FIX_HALF) >> FIX_SHIFT))

/* Truncate toward -infinity (matches >> for non-negative positions). */
#define FIX_TO_INT_FLOOR(x) ((s32)((x) >> FIX_SHIFT))

static inline u32 u32_isqrt(u32 y)
{
    u32 x = 0;
    u32 b = 1u << 30;

    while (b > y)
        b >>= 2;
    while (b != 0) {
        if (y >= x + b) {
            y -= x + b;
            x = (x >> 1) + b;
        } else
            x >>= 1;
        b >>= 2;
    }
    return x;
}

#endif

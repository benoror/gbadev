#ifndef RKANOID_GBA_AUDIO_H
#define RKANOID_GBA_AUDIO_H

#include "gba_types.h"

/* One-time master enable + channel routing. Call once at boot before any Sfx* call. */
void InitAudio(void);

/* Fire-and-forget retro SFX. Each one immediately triggers a PSG channel and returns;
 * the GBA hardware plays it asynchronously while the game keeps running. */
void SfxPaddleHit(void);    /* short bright tick on Square 2                  */
void SfxBrickHit(void);     /* tight noise click on Channel 4                  */
void SfxWallBounce(void);   /* low square blip for walls/ceiling/shield        */
void SfxBonusPickup(void);  /* rising sweep on Square 1 — power-up vibe         */
void SfxLifeLost(void);     /* descending square — sad-trombone sting           */
void SfxLevelClear(void);   /* major-fifth chord (Sq1+Sq2) — fanfare            */

#endif

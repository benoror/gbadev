#include "gba_audio.h"

/* Host-side stubs: the SFX engine writes to MMIO addresses that don't exist on the
 * test runner. Replace the real implementations with no-ops so collisions/physics
 * compile and link cleanly under gcc. */

void InitAudio(void) {}
void SfxPaddleHit(void) {}
void SfxBrickHit(void) {}
void SfxWallBounce(void) {}
void SfxBonusPickup(void) {}
void SfxLifeLost(void) {}
void SfxLevelClear(void) {}

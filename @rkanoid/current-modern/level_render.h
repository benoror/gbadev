#ifndef RKANOID_LEVEL_RENDER_H
#define RKANOID_LEVEL_RENDER_H

#include "game_types.h"
#include "gba_types.h"

extern u8 gBackgroundMap[];

void ApplyShieldFloor(void);
void CopyLevelTileMapToBackground(u16 level);
void CopyBackgroundMapToVram(void);
void RefreshBackground(u16 level, boolean shieldActive);

#endif

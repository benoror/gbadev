# Rename glossary

This lane translates Spanish-era naming into clearer English-oriented names while keeping gameplay behavior unchanged.

## Files

- `demo.c` -> `app_flow.c`
- `juego.c` -> `game_loop.c`
- `niveles.c` -> `levels_data.c` + `levels_runtime.c`
- `bloques.txt` -> `blocks.txt`
- `proyecto/` -> `project/`
- `proyecto/backup_t/` -> `project/source_art/`

## Front-end / flow

- `MuestraTitulo` -> `ShowTitleScreen`
- `Menu` -> `RunMainMenu`
- `IniciaBG` -> `LoadGameplayBackground`
- `MuestraDream` -> `ShowSplashScreen`
- `MuestraGameOver` -> `ShowGameOverScreen`
- `MuestraFin` -> `ShowEndingScreen`
- `Inicia` -> `InitializeDisplay`
- `Musica` -> `PlayMusicTest`
- `Main` -> `RunGame`

## Gameplay / HUD

- `ANCHO_BOLA` -> `ballWidth`
- `ALTO_BOLA` -> `ballHeight`
- `ANCHO_RAQUETA` -> `paddleWidth`
- `ALTO_RAQUETA` -> `paddleHeight`
- `MAX_Y` -> `maxBallY`
- `Inicio_Bx` -> `serveBallX`
- `Inicio_Rx` -> `servePaddleX`
- `rastro` -> `trailEnabled`
- `jugando` -> `roundActive`
- `nivelMap` -> `levelTileMap`
- `RefrescaBG` -> `RefreshBackground`
- `EstableceBola` -> `SetBallSprite`
- `RefrescaBonus` -> `UpdateBonusSprite`
- `RefrescaLife` -> `UpdateLifeDisplay`
- `RefrescaScore` -> `UpdateScoreDisplay`
- `RefrescaNivel` -> `UpdateLevelDisplay`
- `EstableceRaqueta` -> `SetPaddleSprite`
- `limpia_oam` -> `ClearAllSprites`
- `IniciaMovimientoRaqueta` -> `RunServeState`

## Levels / state

- `L` -> `gBlocks`
- `bgMap` -> `gBackgroundMap`
- `destruye` -> `destructible`
- `golpes` -> `hitsRemaining`
- `BONUS` -> `bonusType`
- `activoBonus` -> `bonusActive`
- `bonX` -> `bonusX`
- `bonY` -> `bonusY`
- `bonN` -> `bonusSlot`
- `do_mesias` -> `ApplyShieldFloor`
- `Refresca_blockMap` -> `CopyLevelTileMapToBackground`
- `IniciaPropiedadesBloques` -> `InitializeBlocksForLevel`
- `limpia_activoBonXY` -> `ResetActiveBonusPositions`
- `ChecaTerminadoNivel` -> `IsLevelCleared`

## Assets

- `titulo` -> `title`
- `gameover` -> `game_over`
- `fin` -> `ending`
- `dream` -> `splash`
- `titulo.raw` / `.pal` / `.bmp` -> `title.raw` / `.pal` / `.bmp`
- `gameover.raw` / `.pal` / `.bmp` -> `game_over.raw` / `.pal` / `.bmp`
- `fin.raw` / `.pal` / `.bmp` -> `ending.raw` / `.pal` / `.bmp`
- `dream.raw` / `.pal` / `.bmp` -> `splash.raw` / `.pal` / `.bmp`

## Bonus constants

- `LONG_R_BONUS` -> `LONG_PADDLE_BONUS`
- `LIFE_BONUS` -> `EXTRA_LIFE_BONUS`
- `BIGBALL_BONUS` -> `BIG_BALL_BONUS`
- `MESIAS_BONUS` -> `SHIELD_BONUS`

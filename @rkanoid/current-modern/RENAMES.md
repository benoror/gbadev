# Rename glossary

This lane starts translating rough Spanish-era naming into clearer English-oriented names while keeping gameplay behavior unchanged.

## Front-end / flow

- `MuestraTitulo` -> `ShowTitleScreen`
- `Menu` -> `RunMainMenu`
- `IniciaBG` -> `LoadGameBackground`
- `MuestraDream` -> `ShowSplashScreen`
- `MuestraGameOver` -> `ShowGameOverScreen`
- `MuestraFin` -> `ShowEndingScreen`
- `Inicia` -> `InitializeDisplay`
- `Musica` -> `PlayMusicTest`
- `Main` -> `RunGame`

## Gameplay / HUD

- `RefrescaBG` -> `RefreshBackground`
- `EstableceBola` -> `SetBallSprite`
- `RefrescaBonus` -> `UpdateBonusSprite`
- `RefrescaLife` -> `UpdateLifeDisplay`
- `RefrescaScore` -> `UpdateScoreDisplay`
- `RefrescaNivel` -> `UpdateLevelDisplay`
- `EstableceRaqueta` -> `SetPaddleSprite`
- `limpia_oam` -> `ClearOam`
- `IniciaMovimientoRaqueta` -> `RunServeState`

## Levels / state

- `do_mesias` -> `ApplyShieldFloor`
- `Refresca_blockMap` -> `RefreshBlockMap`
- `IniciaPropiedadesBloques` -> `InitializeBlockProperties`
- `limpia_activoBonXY` -> `ResetActiveBonusPositions`
- `ChecaTerminadoNivel` -> `IsLevelCleared`

## Constants with English aliases

- `LONG_R_BONUS` -> `LONG_PADDLE_BONUS`
- `LIFE_BONUS` -> `EXTRA_LIFE_BONUS`
- `BIGBALL_BONUS` -> `BIG_BALL_BONUS`
- `MESIAS_BONUS` -> `SHIELD_BONUS`

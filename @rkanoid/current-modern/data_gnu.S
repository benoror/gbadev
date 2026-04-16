.section .rodata, "a", %progbits

.align 2
.global spriteTiles
spriteTiles:
    .incbin "bitmaps/sprites.raw"

.align 2
.global spritePalette
spritePalette:
    .incbin "palettes/sprites.pal"

.align 2
.global backgroundTiles
backgroundTiles:
    .incbin "bitmaps/bg.raw"

.align 2
.global backgroundPalette
backgroundPalette:
    .incbin "palettes/bg.pal"

.align 2
.global titleScreenTiles
titleScreenTiles:
    .incbin "bitmaps/title.raw"

.align 2
.global titleScreenPalette
titleScreenPalette:
    .incbin "palettes/title.pal"

.align 2
.global gameOverScreenTiles
gameOverScreenTiles:
    .incbin "bitmaps/game_over.raw"

.align 2
.global gameOverScreenPalette
gameOverScreenPalette:
    .incbin "palettes/game_over.pal"

.align 2
.global endingScreenTiles
endingScreenTiles:
    .incbin "bitmaps/ending.raw"

.align 2
.global endingScreenPalette
endingScreenPalette:
    .incbin "palettes/ending.pal"

.align 2
.global splashScreenTiles
splashScreenTiles:
    .incbin "bitmaps/splash.raw"

.align 2
.global splashScreenPalette
splashScreenPalette:
    .incbin "palettes/splash.pal"

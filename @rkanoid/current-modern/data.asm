
	AREA demo_data, DATA, READONLY
	  EXPORT spriteTiles
	  EXPORT spritePalette
	  EXPORT backgroundTiles
	  EXPORT backgroundPalette
	  EXPORT titleScreenTiles
	  EXPORT titleScreenPalette
	  EXPORT gameOverScreenTiles
	  EXPORT gameOverScreenPalette
	  EXPORT endingScreenTiles
	  EXPORT endingScreenPalette
	  EXPORT splashScreenTiles
	  EXPORT splashScreenPalette
spriteTiles
	INCBIN Bitmaps/sprites.raw

spritePalette
	INCBIN Palettes/sprites.pal

backgroundTiles
	INCBIN Bitmaps/bg.raw

backgroundPalette
	INCBIN Palettes/bg.pal

titleScreenTiles
	INCBIN Bitmaps/title.raw

titleScreenPalette
	INCBIN Palettes/title.pal

gameOverScreenTiles
	INCBIN Bitmaps/game_over.raw

gameOverScreenPalette
	INCBIN Palettes/game_over.pal

endingScreenTiles
	INCBIN Bitmaps/ending.raw

endingScreenPalette
	INCBIN Palettes/ending.pal

splashScreenTiles
	INCBIN Bitmaps/splash.raw

splashScreenPalette
	INCBIN Palettes/splash.pal

	END

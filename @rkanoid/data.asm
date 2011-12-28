
	AREA demo_data, DATA, READONLY
	  EXPORT sprites
	  EXPORT sprite_palette
	  EXPORT bg
	  EXPORT bg_pal
	  EXPORT titulo
	  EXPORT titulo_pal
	  EXPORT gameOver
	  EXPORT gameOver_pal
	  EXPORT fin
	  EXPORT fin_pal
	  EXPORT dream
	  EXPORT dream_pal
sprites
	INCBIN Bitmaps/sprites.raw

sprite_palette
	INCBIN Palettes/sprites.pal

bg
	INCBIN Bitmaps/bg.raw

bg_pal
	INCBIN Palettes/bg.pal

titulo
	INCBIN Bitmaps/titulo.raw

titulo_pal
	INCBIN Palettes/titulo.pal

gameOver
	INCBIN Bitmaps/gameOver.raw

gameOver_pal
	INCBIN Palettes/gameOver.pal

fin
	INCBIN Bitmaps/fin.raw

fin_pal
	INCBIN Palettes/fin.pal

dream
	INCBIN Bitmaps/dream.raw

dream_pal
	INCBIN Palettes/dream.pal

	END
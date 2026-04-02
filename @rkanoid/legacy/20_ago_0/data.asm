
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
	INCBIN sprites.raw

sprite_palette
	INCBIN sprites.pal

bg
	INCBIN bg.raw

bg_pal
	INCBIN bg.pal

titulo
	INCBIN titulo.raw

titulo_pal
	INCBIN titulo.pal

gameOver
	INCBIN gameOver.raw

gameOver_pal
	INCBIN gameOver.pal

fin
	INCBIN fin.raw

fin_pal
	INCBIN fin.pal

dream
	INCBIN dream.raw

dream_pal
	INCBIN dream.pal

	END

	AREA demo_data, DATA, READONLY
	  EXPORT bg
	  EXPORT bg_pal
	  EXPORT sprites
	  EXPORT sprites_pal

bg
	INCBIN bg.raw

bg_pal
	INCBIN bg.pal

sprites
	INCBIN sprites.raw

sprites_pal
	INCBIN sprites.pal

	END
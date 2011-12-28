

	AREA demo_data, DATA, READONLY
	  EXPORT cebel
	  EXPORT cebel_pal
	  EXPORT bg
	  EXPORT bg_pal

cebel
	INCBIN cebel.raw
cebel_pal
	INCBIN cebel.pal
bg
	INCBIN bg.raw
bg_pal
	INCBIN bg.pal


	END


	AREA demo_data, DATA, READONLY
	  EXPORT prsj1_pal
	  EXPORT prsj1_raw
	  EXPORT bg1_1_raw
	  EXPORT bg1_1_pal
	  EXPORT bg1_2_raw
	  EXPORT bg1_2_pal

prsj1_pal
	INCBIN prsj1.pal
prsj1_raw
	INCBIN prsj1.raw
bg1_1_pal
	INCBIN bg1_1.pal
bg1_1_raw
	INCBIN bg1_1.raw
bg1_2_pal
	INCBIN bg1_2.pal
bg1_2_raw
	INCBIN bg1_2.raw

	END
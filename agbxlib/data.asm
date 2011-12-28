;
	AREA demo_data, DATA, READONLY

	EXPORT sprUP
	EXPORT sprLEFT
	EXPORT sprDOWN
	EXPORT sprRIGHT
	EXPORT sprPAL

	EXPORT frog
	EXPORT frogPAL

	EXPORT titulo
	EXPORT bg1
	EXPORT bgPAL

	EXPORT menuBattle
	EXPORT menuMsg
	EXPORT menuStart
	EXPORT menuPAL


; =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


sprUP
	INCBIN dez/spriteUP.raw
sprLEFT
	INCBIN dez/spriteLEFT.raw
sprDOWN
	INCBIN dez/spriteDOWN.raw
sprRIGHT
	INCBIN dez/spriteRIGHT.raw
sprPAL
	INCBIN dez/paleta.pal

; =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

frog	
	INCBIN frog/frog.raw
frogPAL
	INCBIN frog/paleta.pal

; =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

titulo
	INCBIN bgs/titulo.raw
bg1
	INCBIN bgs/bg1.raw
bgPAL
	INCBIN bgs/paleta.pal

; =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

menuBattle
	INCBIN menus/BGbattle.raw
menuMsg
	INCBIN menus/BGmsg.raw
menuStart
	INCBIN menus/BGstart.raw
menuPAL
	INCBIN menus/paleta.pal


	END
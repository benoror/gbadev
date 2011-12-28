// --------------------------------- //
//
// Mystic Quest
// DreamSoft
// http://www.geocities.com/gbadevesp
// ducker0@hotmail.com
//
// --------------------------------- //

#include "regs.h"

#include "main.c"

extern u16 bg1_1_raw;
extern u16 bg1_1_pal;
extern u16 bg1_2_raw;
extern u16 bg1_2_pal;

u16 *tmp_pal;
u16 *tmp_raw;

void Inicia(void)
{
	BG0CNT = 0x0087; // 0000 0000 1000 0111
	DISPCNT = 0x1140; // 0001 0001 0100 0000
}

void CargaBG(void)
{
	u16 x,y,i;

	   	tmp_raw = (u16*)(&bg1_1_raw);
	   	tmp_pal = (u16*)(&bg1_1_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<240*160;i++) BG0Data[i]=tmp_raw[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
}

void C_Entry()
{
	Inicia();
	CargaBG();
	InitSprites();
	Main();
}
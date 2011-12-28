 // --------------------------------- //
//
// @rkanoid
// por DuckeR
// http://www.geocities.com/gbadevesp
// ducker0@hotmail.com
//
// --------------------------------- //

#include "regs.h"
#include "juego.c"

extern u16 bg;
extern u16 bg_pal;
extern u16 sprites;
extern u16 sprites_pal;

u16 *tmp_pal;
u16 *tmp_img;

/****PROTOTIPOS**************************/
void IniciaBG(void);				// Muestra el fondo y lo guarda en el VBUFFER

void Inicia(void);				// Inicia registros (modos de video, etc.)
/********************************************/

void C_Entry()				// Main
{
	Inicia();				// Inicia Registros

	IniciaBG();
	DMA3Call((u32)&sprites_pal, (u32)OBJ_PLTT, 0x84000080);
	DMA3Call((u32)&sprites, (u32)OBJCHARDATA, 0x84000980);

	Main();
}

/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><><><<>><><<*/
/*####################################################################*/
/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><>*/

void IniciaBG(void)
{
	u16 x,y,i;
	   	tmp_img = (u16*)(&bg);
	   	tmp_pal = (u16*)(&bg_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<240*160;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
}

/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<*/

void Inicia(void)
{
	BG0CNT = 0x00C7;
	BG1CNT = 0x0285;
	DISPCNT = 0x1140;
}
// --------------------------------------------	//
//
// Cebelix
// DreamSoft
// http://www.geocities.com/gbadevesp
// ducker0@hotmail.com
//
// --------------------------------------------	//

#include "regs.h" 				// Incluyo libreria de registros
#include "cebel.c"				// Incluyo libreria del personaje

extern u16 bg;
extern u16 bg_pal;
u16 *tmp_pal;
u16 *tmp_img;

void Init(void)					// Funcion para inicializar registros
{
	BG0CNT = 0x0087; 				// 0000 0000 1000 0111
	DISPCNT = 0x1140; 			// 0001 0001 0100 0000
}

void Clear_OAM(void)
{
u16 i;

	for(i=0;i<128;i++)
	{
	OAM[i].Attrib0 = -8;
	OAM[i].Attrib1 = -8;
	OAM[i].Attrib2 = 0x0000;
	}
}

void BG1()
{
	u16 x,y,i;

	   	tmp_img = (u16*)(&bg);
	   	tmp_pal = (u16*)(&bg_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<64*600;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
}

void C_Entry()
{
	Init();					// Iniciamos Registros
	Clear_OAM();
	BG1();
	/*cebel.h*/ ciclo_cebel(120,120,120);	// Ciclo de movimiento del personaje
}
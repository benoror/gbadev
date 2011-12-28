/**************************************/
/*		 	         */
/* agbXlib		       	         */
/* Por Ducker		         */
/* ducker0@hotmail.com	         */
/* http://dreamsoft.4d2.net/	         */
/*			         */
/**************************************/

#include "regs.h"

/*SISTEMA*********************************************************************************************************************/
/********************************************************************************************************************************/

#define PALETA_LCD	((unsigned short *)0x5000000)
#define PANTALLA_LCD	((unsigned char *)0x6000000)

void waitVsync(void)
{
  __asm
   {
    mov 	r0, #0x4000006
    scanline_wait:
     ldrh	r1, [r0]
     cmp	r1, #160
    bne 	scanline_wait
   }
}

void delay(u8 retardo_fin)
{
	u8 retardo_ini;

	for (retardo_ini=0; retardo_ini!=retardo_fin; retardo_ini++)
	{
		waitVsync();  waitVsync();  waitVsync();
	}
}

void MuestraIMG(u8 *tmp_img, u16 *tmp_pal)		//Muestra una imagen 240x160
{
	u16 *titulo_pal;
	u8 *video;
	u16 i;

	DISPCNT = 0x0004;  // 0000 0000 0000 0100
	   	
	titulo_pal = PALETA_LCD;

	for (i=0; i<256; i++)
		titulo_pal[i] = tmp_pal[i];
	
	video = PANTALLA_LCD;	// ?

	for (i=0;i < (240*160); i++)
		video[i]=tmp_img[i];
}

/*Sobre los Modes*************************************************************************************************************/
/********************************************************************************************************************************/

void Inicia(void)
{
	BG0CNT = 0x0087;
	DISPCNT = 0x1100;
}

/*Sobre el Pad******************************************************************************************************************/
/********************************************************************************************************************************/

#define a		J_A
#define b		J_B
#define select	J_SELECT
#define start	J_START
#define derecha	J_RIGHT
#define izquierda	J_LEFT
#define arriba	J_UP
#define abajo	J_DOWN
#define r		J_R
#define l		J_L

void EsperaBoton(u8 nom)
{
	u16 btn;

	btn=joypad();		//Espera si se presiona un botón
	while ((btn & nom))		//Mientras sea diferente de ese boton
	{
		btn=joypad();	//Sigue esperando a que se presione
	}
	key_reg = 0xFFFF;
}

/*Sobre los Sprites*************************************************************************************************************/
/********************************************************************************************************************************/

#include "map.h"
#define piso 0
#define agua 1
#define limite 2

void DMA3Call(u32 Src,u32 Dst,u32 Cnt)	//para guardar la información del sprite en la VRam
{
	DMA3SAD = Src;		//donde esta los datos
	DMA3DAD = Dst;		//a donde van a ir
	DMA3CNT = Cnt;		//que tantoa datoa se van a abrir
}

#define bits32 0x84000480
#define bits16 0x80000480

void IniciaSpritePRSJ(u32 spr, u32 sprpal)
{
	DMA3Call(spr, (u32)OBJCHR, bits32);
	DMA3Call(sprpal, (u32)OBJPAL, bits32);
}

void SPAI(u16 x,u16 y, u16 superficie)		//pone los datos del sprite en la coordinadas x,y e indica
{													if (superficie == piso) {
	OAM[0].Attrib0 = /**/0x2000+/**/y;
	OAM[0].Attrib1 = x;
	OAM[0].Attrib2 = 0;

	OAM[1].Attrib0 = 0x2000+y;
	OAM[1].Attrib1 = x+8;
	OAM[1].Attrib2 = 2;
	
	OAM[2].Attrib0 = 0x2000+y;
	OAM[2].Attrib1 = x+16;
	OAM[2].Attrib2 = 4;
	
	OAM[3].Attrib0 = 0x2000+y+8;
	OAM[3].Attrib1 = x;
	OAM[3].Attrib2 = 6;
	
	OAM[4].Attrib0 = 0x2000+y+8;
	OAM[4].Attrib1 = x+8;
	OAM[4].Attrib2 = 8;
	
	OAM[5].Attrib0 = 0x2000+y+8;
	OAM[5].Attrib1 = x+16;
	OAM[5].Attrib2 = 10;	

	OAM[6].Attrib0 = 0x2000+y+16;
	OAM[6].Attrib1 = x;
	OAM[6].Attrib2 = 12;

	OAM[7].Attrib0 = 0x2000+y+16;
	OAM[7].Attrib1 = x+8;
	OAM[7].Attrib2 = 14;
	
	OAM[8].Attrib0 = 0x2000+y+16;
	OAM[8].Attrib1 = x+16;
	OAM[8].Attrib2 = 16;		}	//en total son 12 tiles (0-11) de 8x8 qe se unen

	if (superficie == agua) {
	OAM[0].Attrib0 = /**/0x2000+/**/y;
	OAM[0].Attrib1 = x;
	OAM[0].Attrib2 = 0;

	OAM[1].Attrib0 = 0x2000+y;
	OAM[1].Attrib1 = x+8;
	OAM[1].Attrib2 = 2;
	
	OAM[2].Attrib0 = 0x2000+y;
	OAM[2].Attrib1 = x+16;
	OAM[2].Attrib2 = 4;
	
	OAM[3].Attrib0 = 0x2000+y+8;
	OAM[3].Attrib1 = x;
	OAM[3].Attrib2 = 6;
	
	OAM[4].Attrib0 = 0x2000+y+8;
	OAM[4].Attrib1 = x+8;
	OAM[4].Attrib2 = 8;
	
	OAM[5].Attrib0 = 0x2000+y+8;
	OAM[5].Attrib1 = x+16;
	OAM[5].Attrib2 = 10;		}
}

u16 superficie;

u16 Superficie(s16 x, s16 y, u16 sup)
{
	u16 resultado;

	if ( 	(MyMap[(y+8)/8][(x)/8]==sup) &&
		(MyMap[(y+8)/8][1+(x-1)/8]==sup) &&
		(MyMap[1+(y+7)/8][(x)/8]==sup) &&
		(MyMap[1+(y+7)/8][1+(x-1)/8]==sup)	) { resultado = 1; }

	else { resultado = 0; }

	superficie = sup;

	return resultado;
}

void MueveSprite(u16 x, u16 y, u8 retardo1, u8 retardo2,
		u32 up, u32 upPAL,
		u32 left, u32 leftPAL,
		u32 down, u32 downPAL,
		u32 right, u32 rightPAL)
{
	u32 btn;
	u16 new_x,
	       new_y;
	u16 i;

	SPAI(x,y,piso);	

	while (1)					
	{
	btn=(joypad());			
		if (btn)

		{

		new_x = x;
		new_y = y;

		if (!(btn & izquierda))	{	
		IniciaSpritePRSJ(left, leftPAL);	
		for (i=0;i<=16;i++)
		{
			delay(retardo1);
			new_x = x-i;
			SPAI(new_x,y,superficie);
		}
		delay(retardo2);
		}
		if (!(btn & derecha))	{	
		IniciaSpritePRSJ(right, rightPAL);			
		for (i=0;i<=16;i++)
		{
			delay(retardo1);
			new_x = x+i;
			SPAI(new_x,y,superficie);
		}
		delay(retardo2);
		}
		if (!(btn & arriba))	{		
		IniciaSpritePRSJ(up, upPAL);			
		for (i=0;i<=16;i++)
		{
			delay(retardo1);
			new_y = y-i;
			SPAI(x,new_y,superficie);
		}
		delay(retardo2);
		}										
		if (!(btn & abajo))	{	
		IniciaSpritePRSJ(down, downPAL);	
		for (i=0;i<=16;i++)
		{
			delay(retardo1);
			new_y = y+i;
			SPAI(x,new_y,superficie);
		}
		delay(retardo2);
		}										

		if ((x != new_x) && ( Superficie(new_x,y,piso) || Superficie(new_x,y,agua) ))
			{
				x = new_x;	//x se actualiza al new_x
			}

		if ((y != new_y) && ( Superficie(x,new_y,piso) || Superficie(new_x,y,agua) ))
			{	
				y = new_y;
			}
		if (y>=159) {y=0;}
		if (y==0) {y=160;}
		if (x>=239) {x=0;}
		if (x==0) {x=240;}

		}
		waitVsync();

		SPAI(x,y,superficie);		

	} //while (1)
  
 }

/*Sobre los Fondos************************************************************************************************************/
/********************************************************************************************************************************/

void IniciaBGpaleta(u16 *tmp_pal)
{
	u16 i;
		 for (i=0;i<256;i++) BKGPal[i]=tmp_pal[i];
}

void CreaBGdata(u8 *tmp_img, u16 inicio, u16 final, u16 numBG)
{
	u16 x,y,i;
	u8 *BGnum;

	switch(numBG)
	{
	case 0: BGnum=BKG0Data; break;
	case 1: BGnum=BKG1Data; break;
	case 2: BGnum=BKG2Data; break;
	case 3: BGnum=BKG3Data; break;
	}

	waitVsync();

	   	for(i=inicio;i<64*final;i++) BGnum[i]=tmp_img[i];

   		for (y=0;y<20;y++)
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x; 		// 33 ~ 33  31 ~ 31
		   }
		
}

void CreaBGscreen(u8 *tmp_img, u16 inicio, u16 final, u16 numBG)
{
	u16 x,y,i;
	u16 *BGnum;

	switch(numBG)
	{
	case 0: BGnum=BKG0Screen; break;
	case 1: BGnum=BKG1Screen; break;
	case 2: BGnum=BKG2Screen; break;
	case 3: BGnum=BKG3Screen; break;
	}

	waitVsync();

	   	for(i=inicio;i<64*final;i++) BGnum[i]=tmp_img[i];

   		for (y=0;y<20;y++)
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x; 		// 33 ~ 33  31 ~ 31
		   }
		
}

void set_bkg_data(u8 *bgdat, u16 inicio, u16 fin, int fondo)
{
  int i,j;
  u8 *FondoDestino;
  
  switch(fondo)
  {
    case 0: FondoDestino=BKG0Data; break;
    case 1: FondoDestino=BKG1Data; break;
    case 2: FondoDestino=BKG2Data; break;
    case 3: FondoDestino=BKG3Data; break;            
  }      
  
  j=0; 
  for(i=inicio*64; i<64*fin; i++, j++)  FondoDestino[i]=((u8*)bgdat)[j];
}

void set_bkg_tiles(int x, int y, int tilesAlto, int tilesAncho, int tile_inicio, int fondo)
{
  int i,j;

  u16 *FondoDestino;
  
  switch(fondo)
  {
    case 0: FondoDestino=BKG0Screen; break;
    case 1: FondoDestino=BKG1Screen; break;
    case 2: FondoDestino=BKG2Screen; break;
    case 3: FondoDestino=BKG3Screen; break;            
  }      
  
  for (j=0; j<tilesAncho; j++)
    for (i=0; i<tilesAlto; i++)
    {
      FondoDestino[y*32+j*32+x+i]=tile_inicio;
      tile_inicio++;
    }
}
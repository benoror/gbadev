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

extern u32 sprites;	
extern u32 sprite_palette;
extern u16 bg;
extern u16 bg_pal;
extern u16 titulo;
extern u16 titulo_pal;
extern u16 gameOver;
extern u16 gameOver_pal;
extern u16 fin;
extern u16 fin_pal;
extern u16 dream;
extern u16 dream_pal;

u16 *tmp_pal;
u16 *tmp_img;

/****PROTOTIPOS**************************/
void MuestraTitulo(void);			// Muestra el titulo en pantalla
void Menu(void);

void IniciaBG(void);				// Muestra el fondo y lo guarda en el VBUFFER

void MuestraDream(void);
void MuestraGameOver(void);

void Inicia(void);				// Inicia registros (modos de video, etc.)
void Musica(void);

/********************************************/

void C_Entry()				// Main
{
	u16 boton;

	Inicia();				// Inicia Registros

	MuestraDream();
	fade_in(30, 0, 1);

	Wait(100);	//Espera 3 segundos
	
	fade_out(30, 0, 0);

	MuestraTitulo();			// Muestra el titulo
	fade_in(30, 0, 0);

	limpia_oam();
	DMA3Call((u32)&sprite_palette, (u32)OBJ_PLTT, 0x84000080);
	DMA3Call((u32)&sprites, (u32)OBJCHARDATA, 0x84000980);

	//Musica();
	Menu();

	fade_out(30, 1, 0);

	IniciaBG();
	DMA3Call((u32)&sprite_palette, (u32)OBJ_PLTT, 0x84000080);
	DMA3Call((u32)&sprites, (u32)OBJCHARDATA, 0x84000980);

	Main();

	fade_out(30, 1, 0);
	MuestraGameOver();
	fade_in(30, 1, 0);
	boton=joypad();			// Espera si se presiona un botón (Starr)
	while ((boton & J_START))		// Mientras sea diferente de Start
	{
		boton=joypad();		// Sigue esperando a que se presione Start
	}
	fade_out(30, 1, 0);
 }

/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><><><<>><><<*/
/*####################################################################*/
/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><>*/

void MuestraDream(void)
{
	u16 x,y,i;

	   	tmp_img = (u16*)(&dream);
	   	tmp_pal = (u16*)(&dream_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<240*160;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
} 

void MuestraTitulo(void)
{
	u16 x,y,i;

	   	tmp_img = (u16*)(&titulo);
	   	tmp_pal = (u16*)(&titulo_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<64*600;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
} 

void Menu(void)
{
	u16 i=1, boton, x=78, yc[]={0, 90, 103, 116};

	while(1)
	{
		EstableceBola(x, yc[i], FALSE, 0, BOLA2);

		boton=joypad();
		if (!(boton & J_DOWN)) i++;
		if (!(boton & J_UP)) i--;
		if(i>3) i=1;
		if(i<1) i=3;

		if (!(boton & J_START))
		{
			if(i==1) break;
		}
	}
}

void IniciaBG(void)
{
	u16 i;
	   	tmp_img = (u16*)(&bg);
	   	tmp_pal = (u16*)(&bg_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<240*160;i++) BG0_DATA[i]=tmp_img[i];
}

void MuestraGameOver(void)
{
	u16 x,y,i;

	   	tmp_img = (u16*)(&gameOver);
	   	tmp_pal = (u16*)(&gameOver_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<64*600;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
} 

void MuestraFin(void)
{
	u16 x,y,i;

	   	tmp_img = (u16*)(&fin);
	   	tmp_pal = (u16*)(&fin_pal);

	   	for (i=0;i<256;i++) BG_PLTT[i]=tmp_pal[i];

	   	for(i=0;i<64*600;i++) BG0_DATA[i]=tmp_img[i];

   		for (y=0;y<20;y++) {
		   for (x=0;x<30;x++) {
			   VBUFFER[y*32+x]=y*30+x;
		   }
		}
} 

/*<><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<>><><<*/

void Musica(void)
{
	u16 i;
	int music[10],
	    music_a[]={ 328046,32086,321046,32986,321046,321155,321253,
				321297,321379,321452,321379,32986,32768,32768,32768,32768,321452,32986, };

		*(u16*)0x4000082 = 0x0002;
		*(u16*)0x4000084 = 0x0080;
		*(u16*)0x4000080 = 0x3377;
		*(u16*)0x4000060 = 0x0008;
		*(u16*)0x4000062 = 0xf030;
		*(u16*)0x4000068 = 0xf030;

	for(i=0;i<18;i++)
	{
		Wait(20);
		//*(u16*)0x4000064 = music[i];
		*(u16*)0x400006C = music_a[i];
	}
		 
}

void Inicia(void)
{
	BG0CNT = 0x00C7;
	BG1CNT = 0x0285;
	DISPCNT = 0x1140;
}
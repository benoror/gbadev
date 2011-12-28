/**************************************/
/*			         */
/* Uso de agbXlib		         */
/* Por Ducker		         */
/* ducker0@hotmail.com	         */
/* http://gbadev-esp.8m.com	         */
/*			         */
/**************************************/

#include "agbXlib.h"

extern u32 sprUP;
extern u32 sprLEFT;
extern u32 sprDOWN;
extern u32 sprRIGHT;
	extern u32 sprPAL;

extern u8 titulo;
extern u32 bg1;
extern u32 bgPAL;

extern u32 menuBattle;
extern u32 menuMsg;
extern u32 menuStart;
extern u32 menuPAL;

u8 *tmp_img;
u16 *tmp_pal;

void IniciaBGpaleta_(void)
{
	tmp_pal = (u16*)(&bgPAL);

	IniciaBGpaleta(tmp_pal);
}

void MuestraTitulo(void)
{
  	tmp_img = (u8*)(&titulo);

	CreaBGdata(tmp_img, 0, 600, 1);
}

void MuestraBG1(void)
{
  	tmp_img = (u8*)(&bg1);

	CreaBGdata(tmp_img, 0, 600, 1);
}

void IniciaMenuPaleta(void)
{
	tmp_pal = (u16*)(&menuPAL);

	IniciaBGpaleta(tmp_pal);
}

void MuestraBGmsg(void)
{
  	tmp_img = (u8*)(&menuMsg);
	tmp_pal = (u16*)(&menuPAL);

	CreaBGdata(tmp_img, 0, 600, 1);
}


void IniciaPRSJ(void)
{
	IniciaSpritePRSJ((u32)&sprUP, (u32)&sprPAL);

	MueveSprite(108,14, 4, 1, 
	(u32)&sprUP, (u32)&sprPAL, 
	(u32)&sprLEFT, (u32)&sprPAL, 
	(u32)&sprDOWN, (u32)&sprPAL, 
	(u32)&sprRIGHT, (u32)&sprPAL	);
}

void C_Entry()
{
	Inicia();
	IniciaBGpaleta_();

	MuestraTitulo();
	EsperaBoton(start);

	MuestraBG1();

//	IniciaMenuPaleta();
//	MuestraBGmsg();

	IniciaPRSJ();
}
#include "niveles.c"

/******************************************/
#define MIN_X 16
#define MIN_Y 8
#define MAX_X 160
u16 MAX_Y=160;

u16 ANCHO_BOLA=6;
u16 ALTO_BOLA=6;
#define BOLA1 8
#define BOLA2 50
u16 ANCHO_RAQUETA=32;
u16 ALTO_RAQUETA=8;

u16 Inicio_Bx;
u16 Inicio_Rx;

/******************************************/

void RefrescaBG(u16 nivel, boolean mesias)
{
	u16 x,y;

	Refresca_blockMap(nivel);

	if(mesias==TRUE) do_mesias();

  	 	for (x = 0; x < 30; x ++)
     	 		for (y = 0; y < 20; y++)
			VBUFFER[x + (y * 32)]=bgMap[x + (y * 30) +1];
}

void EstableceBola(u16 x, u16 y, u16 rastro, u16 direccion, u16 tipoBola)
{
	u16 i;

	if(rastro==TRUE)
	{
		if(direccion==0)
		{
		OAM[7].Attrib0 = 0x2000+y;
		OAM[7].Attrib1 = x;
		OAM[7].Attrib2 = 40;

		OAM[8].Attrib0 = OAM[7].Attrib0+2;
		OAM[8].Attrib1 = OAM[7].Attrib1+2;
		OAM[8].Attrib2 = 40;

		OAM[9].Attrib0 = OAM[8].Attrib0+2;
		OAM[9].Attrib1 = OAM[8].Attrib1+2;
		OAM[9].Attrib2 = 42;

		OAM[10].Attrib0 = OAM[9].Attrib0+2;
		OAM[10].Attrib1 = OAM[9].Attrib1+2;
		OAM[10].Attrib2 = 44;

		OAM[11].Attrib0 = OAM[10].Attrib0+2;
		OAM[11].Attrib1 = OAM[10].Attrib1+2;
		OAM[11].Attrib2 = 46;

		OAM[12].Attrib0 = OAM[11].Attrib0+2;
		OAM[12].Attrib1 = OAM[11].Attrib1+2;
		OAM[12].Attrib2 = 48;
		}

		if(direccion==1)
		{
		OAM[7].Attrib0 = 0x2000+y;
		OAM[7].Attrib1 = x;
		OAM[7].Attrib2 = 40;

		OAM[8].Attrib0 = OAM[7].Attrib0+2;
		OAM[8].Attrib1 = OAM[7].Attrib1-2;
		OAM[8].Attrib2 = 40;

		OAM[9].Attrib0 = OAM[8].Attrib0+2;
		OAM[9].Attrib1 = OAM[8].Attrib1-2;
		OAM[9].Attrib2 = 42;

		OAM[10].Attrib0 = OAM[9].Attrib0+2;
		OAM[10].Attrib1 = OAM[9].Attrib1-2;
		OAM[10].Attrib2 = 44;

		OAM[11].Attrib0 = OAM[10].Attrib0+2;
		OAM[11].Attrib1 = OAM[10].Attrib1-2;
		OAM[11].Attrib2 = 46;

		OAM[12].Attrib0 = OAM[11].Attrib0+2;
		OAM[12].Attrib1 = OAM[11].Attrib1-2;
		OAM[12].Attrib2 = 48;
		}

		if(direccion==2)
		{
		OAM[7].Attrib0 = 0x2000+y;
		OAM[7].Attrib1 = x;
		OAM[7].Attrib2 = 40;

		OAM[8].Attrib0 = OAM[7].Attrib0-2;
		OAM[8].Attrib1 = OAM[7].Attrib1+2;
		OAM[8].Attrib2 = 40;

		OAM[9].Attrib0 = OAM[8].Attrib0-2;
		OAM[9].Attrib1 = OAM[8].Attrib1+2;
		OAM[9].Attrib2 = 42;

		OAM[10].Attrib0 = OAM[9].Attrib0-2;
		OAM[10].Attrib1 = OAM[9].Attrib1+2;
		OAM[10].Attrib2 = 44;

		OAM[11].Attrib0 = OAM[10].Attrib0-2;
		OAM[11].Attrib1 = OAM[10].Attrib1+2;
		OAM[11].Attrib2 = 46;

		OAM[12].Attrib0 = OAM[11].Attrib0-2;
		OAM[12].Attrib1 = OAM[11].Attrib1+2;
		OAM[12].Attrib2 = 48;
		}

		if(direccion==3)
		{
		OAM[7].Attrib0 = 0x2000+y;
		OAM[7].Attrib1 = x;
		OAM[7].Attrib2 = 40;

		OAM[8].Attrib0 = OAM[7].Attrib0-2;
		OAM[8].Attrib1 = OAM[7].Attrib1-2;
		OAM[8].Attrib2 = 40;

		OAM[9].Attrib0 = OAM[8].Attrib0-2;
		OAM[9].Attrib1 = OAM[8].Attrib1-2;
		OAM[9].Attrib2 = 42;

		OAM[10].Attrib0 = OAM[9].Attrib0-2;
		OAM[10].Attrib1 = OAM[9].Attrib1-2;
		OAM[10].Attrib2 = 44;

		OAM[11].Attrib0 = OAM[10].Attrib0-2;
		OAM[11].Attrib1 = OAM[10].Attrib1-2;
		OAM[11].Attrib2 = 46;

		OAM[12].Attrib0 = OAM[11].Attrib0-2;
		OAM[12].Attrib1 = OAM[11].Attrib1-2;
		OAM[12].Attrib2 = 48;
		}

	}
	else
	{
		for(i=8;i<=127;i++)
			{
			if(i!=13 && i!=14)
				{
				OAM[i].Attrib0 = 0x2000+160+8;
				OAM[i].Attrib1 = 240+8;
				}
			}

		OAM[7].Attrib0 = 0x2000+y;
		OAM[7].Attrib1 = x;
		OAM[7].Attrib2 = tipoBola;
	}

}

void RefrescaBonus(u16 x, u16 y, u16 tipo, u16 num)
{
	OAM[15+num].Attrib0 = 0x2000+y;
	OAM[15+num].Attrib1 = x;
	OAM[15+num].Attrib2 = 30+(tipo*2);
}

void RefrescaLife(u16 num)
{
	OAM[4].Attrib0 = 0x2000+40;
	OAM[4].Attrib1 = 192;
	OAM[4].Attrib2 = 10+(num*2);
}

void RefrescaScore(u16 num)
{
	OAM[5].Attrib0 = 0x2000+24;
	OAM[5].Attrib1 = 176;
	OAM[5].Attrib2 = 10+(num*2);
}

void RefrescaNivel(u16 num)
{
	OAM[6].Attrib0 = 0x2000+88;
	OAM[6].Attrib1 = 176;
	OAM[6].Attrib2 = 10+(num*2);
}


void EstableceRaqueta(u16 x, u16 y, u16 long_r)
{
	if(long_r==FALSE)
	{
		OAM[0].Attrib0 = 0x2000+y;
		OAM[0].Attrib1 = x;
		OAM[0].Attrib2 = 0x0800+0;

		OAM[1].Attrib0 = 0x2000+y;
		OAM[1].Attrib1 = x+8;
		OAM[1].Attrib2 = 0x0800+2;
	
		OAM[2].Attrib0 = 0x2000+y;
		OAM[2].Attrib1 = x+16;
		OAM[2].Attrib2 = 0x0800+4;
	
		OAM[3].Attrib0 = 0x2000+y;
		OAM[3].Attrib1 = x+24;
		OAM[3].Attrib2 = 0x0800+6;

		OAM[13].Attrib0 = 0x2000+160+8;
		OAM[13].Attrib1 = 240+8;
		OAM[13].Attrib2 = 0x0800+4;
	
		OAM[14].Attrib0 = 0x2000+160+8;;
		OAM[14].Attrib1 = 240+8;
		OAM[14].Attrib2 = 0x0800+6;
	}

	if(long_r==TRUE)
	{
		OAM[0].Attrib0 = 0x2000+y;
		OAM[0].Attrib1 = x;
		OAM[0].Attrib2 = 0x0800+0;

		OAM[1].Attrib0 = 0x2000+y;
		OAM[1].Attrib1 = x+8;
		OAM[1].Attrib2 = 0x0800+4;
	
		OAM[2].Attrib0 = 0x2000+y;
		OAM[2].Attrib1 = x+16;
		OAM[2].Attrib2 = 0x0800+2;
	
		OAM[3].Attrib0 = 0x2000+y;
		OAM[3].Attrib1 = x+24;
		OAM[3].Attrib2 = 0x0800+4;
	
		OAM[13].Attrib0 = 0x2000+y;
		OAM[13].Attrib1 = x+32;
		OAM[13].Attrib2 = 0x0800+2;
	
		OAM[14].Attrib0 = 0x2000+y;
		OAM[14].Attrib1 = x+40;
		OAM[14].Attrib2 = 0x0800+6;
	}
	
}

void limpia_oam(void)
{
	u16 i;

	for(i=0;i<=128;i++)
	{
		OAM[i].Attrib0 = 0x2000+160+8;
		OAM[i].Attrib1 = 240+8;
	}
}

void IniciaMovimientoRaqueta(u16 rx, u16 ry, u16 long_r)
{
	u16 boton,bx = rx+13, incRaqueta;

	EstableceRaqueta(rx, ry, long_r);
	EstableceBola(bx,ry-ALTO_BOLA,FALSE,0,BOLA1);
	boton = (joypad());
	while ((boton & J_A))
	{
	boton = (joypad());
	if ( (boton & J_B)) incRaqueta=3;
	if (!(boton & J_B)) incRaqueta=4;
	if(boton)
	{
		if (!(boton & J_RIGHT) && rx < (MAX_X - ANCHO_RAQUETA) )
			{
			rx+=incRaqueta;
			bx+=incRaqueta;
			}
		if (!(boton & J_LEFT) && rx > MIN_X )
			{
			rx-=incRaqueta;
			bx-=incRaqueta;
			}
		if (!(boton & J_L) && bx>rx )
			{
			bx--;
			}
		if (!(boton & J_R) && bx-ANCHO_BOLA<=rx+19 )
			{
			bx++;
			}
	}
	EstableceRaqueta(rx, ry, long_r);
	EstableceBola(bx,ry-ALTO_BOLA,FALSE,0,BOLA1);
	}

	Inicio_Bx = bx;
	Inicio_Rx = rx;
}

void Main(void)
{
	int col, fil, i, delay;
	u16 boton;
	u16 bx, by;
	u16 rx = 16,
	      ry = 144,
	      saltoX = 1,
	      saltoY = 1,
		incRaqueta;
	u16 LifeCount = 5,
	      ScoreCount = 0;
	boolean jugando, rastro;
	u16 direccion, tipoBola, long_r, mesias;
	u16 nivel = 1;
	u8 *nivelMap;

	GANA_NIVEL:

	if(nivel>1) fade_out(30, 1, 0);
	RefrescaBG(nivel,mesias);
	limpia_oam();
	Wait(100);	//Espera 3 segundos
	fade_in(30, 1, 0);

	IniciaPropiedadesBloques(nivel);
	RefrescaScore(ScoreCount);
	RefrescaNivel(nivel);

	if(nivel==1) { nivelMap = blockMap_1; }//N1 = N2 = 12; }
	if(nivel==2) { nivelMap = blockMap_2; }//N1 = N2 = 13; }
	if(nivel==3) { nivelMap = blockMap_3; }//N1 = N2 = 14; }
	if(nivel==4) { nivelMap = blockMap_4; }//N1 = N2 = 15; }
	if(nivel==5) { nivelMap = blockMap_5; }//N1 = N2 = 16; }

	REINICIA:

	RefrescaLife(LifeCount);
	limpia_activoBonXY();

	jugando=TRUE;
	mesias=FALSE;
	RefrescaBG(nivel,mesias);

	tipoBola=BOLA1;
	ANCHO_BOLA=6;
	ALTO_BOLA=6;
	long_r=FALSE;
	ANCHO_RAQUETA=32;

	if(LifeCount==0 || nivel==6) jugando=FALSE;

	if(jugando==TRUE) IniciaMovimientoRaqueta(rx,ry,long_r);

	rastro=FALSE;
	MAX_Y=160;

	delay=0;
	col = saltoX;
	fil = -saltoY;

	bx = Inicio_Bx;
	by = ry - ALTO_BOLA;
	rx = Inicio_Rx;

	while (LifeCount > 0 && nivel<=5)

		{

		/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*/

		if(bx>=( MAX_X - ANCHO_BOLA ) ) col=-saltoX;
		if(bx<= MIN_X ) col=saltoX;
		if(by>=( MAX_Y - ALTO_BOLA) )
		{
			fil=-saltoY;
			if(mesias==FALSE) { LifeCount--; goto REINICIA; }
			if(mesias==TRUE) { mesias=FALSE; MAX_Y=160; }
		}
		if(by<= MIN_Y ) fil=saltoY;

	if(long_r==FALSE)
	{
		if (by == (ry-ALTO_BOLA) && bx >= (rx-ANCHO_BOLA) && bx <= (rx-ANCHO_BOLA)+16 ) {col=-saltoX; fil=-saltoY;}
		if (by == (ry-ALTO_BOLA) && bx >= (rx-ANCHO_BOLA)+17 && bx <= (rx-ANCHO_BOLA)+32 ) {col=saltoX; fil=-saltoY;}
			if (bx == (rx-ANCHO_BOLA) && ( by>=ry-ALTO_BOLA && by<=ry+8) ) {col=-saltoX; fil=-saltoY;}
			if (bx == (rx+ANCHO_RAQUETA) && ( by>=ry-ALTO_BOLA && by<=ry+8) ) {col=saltoX; fil=-saltoY;}
	}
	if(long_r==TRUE)
	{
		if (by == (ry-ALTO_BOLA) && bx >= (rx-ANCHO_BOLA) && bx <= (rx-ANCHO_BOLA)+24 ) {col=-saltoX; fil=-saltoY;}
		if (by == (ry-ALTO_BOLA) && bx >= (rx-ANCHO_BOLA)+25 && bx <= (rx-ANCHO_BOLA)+48 ) {col=saltoX; fil=-saltoY;}
			if (bx == (rx-ANCHO_BOLA) && ( by>=ry-ALTO_BOLA && by<=ry+8) ) {col=-saltoX; fil=-saltoY;}
			if (bx == (rx+ANCHO_RAQUETA) && ( by>=ry-ALTO_BOLA && by<=ry+8) ) {col=saltoX; fil=-saltoY;}
	}

for (i=0 ; i <= 162 ; i++)
		{
		if ( by == L[i].y-ALTO_BOLA && (bx >= L[i].x-ANCHO_BOLA && bx <= L[i].xM) )
			{ fil = -saltoY;
			if(L[i].BONUS!=NO_BONUS && L[i].golpes <= 1) { L[i].activoBonus=TRUE; }
			if(rastro==TRUE && L[i].destruye==TRUE)
			{
				L[i].golpes = 0;
				fil = saltoY;
			}
			if (L[i].golpes > 0)
				{
				L[i].golpes--;
				}
			if (L[i].destruye == TRUE && L[i].golpes == 0)
				{ 
				L[i].x = L[i].y = L[i].xM = L[i].yM = 0;
				nivelMap[L[i].tile1] = O1; nivelMap[L[i].tile2] = O2;
				ScoreCount ++;
				}
			}

		if ( by == L[i].yM && (bx >= L[i].x-ANCHO_BOLA && bx <= L[i].xM) )
			{ fil = saltoY;
			if(L[i].BONUS!=NO_BONUS && L[i].golpes <= 1) { L[i].activoBonus=TRUE; }
			if(rastro==TRUE && L[i].destruye==TRUE)
			{
				L[i].golpes = 0;
				fil = -saltoY;
			}
			if (L[i].golpes > 0)
				{
				L[i].golpes--;
				}
			if (L[i].destruye == TRUE && L[i].golpes == 0)
				{ 
				L[i].x = L[i].y = L[i].xM = L[i].yM = 0;
				nivelMap[L[i].tile1] = O1; nivelMap[L[i].tile2] = O2;
				ScoreCount ++;
				}
			}

		if ( bx == L[i].x-ANCHO_BOLA && (by >= L[i].y-ALTO_BOLA && by <= L[i].yM) )
			{ col = -saltoX;
			if(L[i].BONUS!=NO_BONUS && L[i].golpes <= 1) { L[i].activoBonus=TRUE; }
			if(rastro==TRUE && L[i].destruye==TRUE)
			{
				L[i].golpes = 0;
				col = saltoX;
			}
			if (L[i].golpes > 0)
				{
				L[i].golpes--;
				}
			if (L[i].destruye == TRUE)
				{ 
				L[i].x = L[i].y = L[i].xM = L[i].yM = 0;
				nivelMap[L[i].tile1] = O1; nivelMap[L[i].tile2] = O2;
				ScoreCount ++;
				}
			}

		if ( bx == L[i].xM && (by >= L[i].y-ALTO_BOLA && by <= L[i].yM) )
			{ col = saltoX;
			if(L[i].BONUS!=NO_BONUS && L[i].golpes <= 1) { L[i].activoBonus=TRUE; }
			if(rastro==TRUE && L[i].destruye==TRUE)
			{
				L[i].golpes = 0;
				col = -saltoX;
			}
			if (L[i].golpes > 0)
				{
				L[i].golpes--;
				}
			if (L[i].destruye == TRUE && L[i].golpes == 0)
				{ 
				L[i].x = L[i].y = L[i].xM = L[i].yM = 0;
				nivelMap[L[i].tile1] = O1; nivelMap[L[i].tile2] = O2;
				ScoreCount ++;
				}
			}
		}

	for (i=0 ; i <= 108 ; i++)
		if(L[i].activoBonus == TRUE)	
		{
			if(L[i].BONUS!=NO_BONUS)
				{
					RefrescaBonus(L[i].bonX,L[i].bonY,L[i].BONUS,L[i].bonN);
					L[i].bonY++;
					if(L[i].bonX+8>=rx && L[i].bonX<=rx+ANCHO_RAQUETA && L[i].bonY>=ry && L[i].bonY<=ry+ALTO_RAQUETA)
					{
						if(L[i].BONUS==COMET_BONUS)
							{
							rastro=TRUE;
							L[i].BONUS=NO_BONUS;
							L[i].activoBonus=FALSE;
							RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
							}
						if(L[i].BONUS==LONG_R_BONUS)
							{
							ANCHO_RAQUETA=48;
							long_r=TRUE;
							L[i].BONUS=NO_BONUS;
							L[i].activoBonus=FALSE;
							RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
							}
						if(L[i].BONUS==LIFE_BONUS)
							{
							LifeCount++;
							RefrescaLife(LifeCount);
							L[i].BONUS=NO_BONUS;
							L[i].activoBonus=FALSE;
							RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
							}
						if(L[i].BONUS==BIGBALL_BONUS)
							{
							rastro=FALSE;
							tipoBola = BOLA2;
							ANCHO_BOLA=8;
							ALTO_BOLA=8;
							L[i].BONUS=NO_BONUS;
							L[i].activoBonus=FALSE;
							RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
							}
						if(L[i].BONUS==MESIAS_BONUS)
							{
							mesias=TRUE;
							MAX_Y=160-12;
							L[i].BONUS=NO_BONUS;
							L[i].activoBonus=FALSE;
							RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
							}
					}
					if(L[i].bonY>=MAX_Y+16)
					{
						L[i].BONUS=NO_BONUS;
						L[i].activoBonus=FALSE;
						RefrescaBonus(240+8,160+8,L[i].BONUS,L[i].bonN);
					}
				}
		}

		boton = joypad();
		if ( (boton & J_B)) incRaqueta=3;
		if (!(boton & J_B)) incRaqueta=4;

		if (!(boton & J_RIGHT) && rx < (MAX_X - ANCHO_RAQUETA) ) rx+=incRaqueta;
		if (!(boton & J_LEFT) && rx > MIN_X ) rx-=incRaqueta;
		if (rx > MAX_X - ANCHO_RAQUETA) rx=MAX_X - ANCHO_RAQUETA;

		if (!(boton & J_SELECT)) goto REINICIA;

		bx += col;
		by += fil;

		wait_vbl_done();
		//Wait(2);

		EstableceRaqueta(rx, ry, long_r);
		if(col<0 && fil<0) direccion=0;
		if(col>0 && fil<0) direccion=1;
		if(col<0 && fil>0) direccion=2;
		if(col>0 && fil>0) direccion=3;
		EstableceBola(bx, by, rastro, direccion, tipoBola);
		RefrescaScore(ScoreCount);
		if (ScoreCount>9) { ScoreCount = 0; }

		if(ChecaTerminadoNivel()==TRUE) {nivel++; goto GANA_NIVEL;}
		RefrescaBG(nivel,mesias);

		}

}

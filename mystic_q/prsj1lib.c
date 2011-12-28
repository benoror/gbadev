#define DreamSoft_Never_Die 1

#define FALSE 0
#define TRUE 1

#define CAMINABLE 0
#define NO_CAMINABLE 1
#define SALIDA_V 2
#define SALIDA_H 3

#define normal 666
#define spz_1 1
#define spz_2 2
#define spz_3 3

#define ss 0
#define sa 4
#define sb 8
#define as 12
#define bs 16

#define down 0
#define up 28
#define right 56
#define left 84

#define w_prsj1 16
#define h_prsj1 24

u16 spazo[] = { spz_1, spz_2, spz_3, normal, };
u16 frames[8] = { ss, sa, sb, sa, ss, as, bs, as };

extern u32 prsj1_pal;
extern u32 prsj1_raw;

void Muestra_prsj1_XY(u16 x, u16 y, u16 frame, u16 lado, u16 estilo)
{
	if(estilo==normal) {
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=0+lado;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=2+lado;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=4+lado;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=6+lado;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=(8+frame)+lado;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=(10+frame)+lado;
			OAM[6].Attrib0=0x2000+160;	OAM[6].Attrib1=240;	OAM[6].Attrib2=0;
			OAM[7].Attrib0=0x2000+160;	OAM[7].Attrib1=240;	OAM[7].Attrib2=0;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
	}

	if(estilo==spz_1) {
		if(lado==down) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=0+lado;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=2+lado;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=4+lado;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=6+lado;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=128;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=10;								
			OAM[6].Attrib0=0x2000+y+16;	OAM[6].Attrib1=x-8;	OAM[6].Attrib2=126;
			OAM[7].Attrib0=0x2000+y+24;	OAM[7].Attrib1=x-8;	OAM[7].Attrib2=130;
			OAM[8].Attrib0=0x2000+y+24;	OAM[8].Attrib1=x;		OAM[8].Attrib2=132;
		}
		if(lado==up) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=28;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=30;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=32;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=142;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=36;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=146;								
			OAM[6].Attrib0=0x2000+y;	OAM[6].Attrib1=x+16;	OAM[6].Attrib2=140;
			OAM[7].Attrib0=0x2000+y+8;	OAM[7].Attrib1=x+16;	OAM[7].Attrib2=144;
			OAM[8].Attrib0=0x2000+y+16;	OAM[8].Attrib1=x+16;	OAM[8].Attrib2=148;
		}
		if(lado==right) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=170;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=58;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=172;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=174;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=176;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=164;						
			OAM[6].Attrib0=0x2000+y-8;	OAM[6].Attrib1=x;		OAM[6].Attrib2=178;
			OAM[7].Attrib0=0x2000+160;	OAM[7].Attrib1=240;	OAM[7].Attrib2=0;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==left) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=84;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=180;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=182;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=184;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=186;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=188;						
			OAM[6].Attrib0=0x2000+y-8;	OAM[6].Attrib1=x+8;	OAM[6].Attrib2=190;
			OAM[7].Attrib0=0x2000+160;	OAM[7].Attrib1=240;	OAM[7].Attrib2=0;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
	}

	if(estilo==spz_2) {
		if(lado==down) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=0+lado;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=2+lado;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=4+lado;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=6+lado;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=120;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=10;						
			OAM[6].Attrib0=0x2000+y+24;	OAM[6].Attrib1=x;		OAM[6].Attrib2=122;
			OAM[7].Attrib0=0x2000+y+32;	OAM[7].Attrib1=x;		OAM[7].Attrib2=124;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==up) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=28;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=138;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=32;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=34;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=40;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=54;						
			OAM[6].Attrib0=0x2000+y-8;	OAM[6].Attrib1=x+8;	OAM[6].Attrib2=136;
			OAM[7].Attrib0=0x2000+160;	OAM[7].Attrib1=240;	OAM[7].Attrib2=0;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==right) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=56;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=160;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=60;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=162;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=152;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=164;						
			OAM[6].Attrib0=0x2000+y;	OAM[6].Attrib1=x+16;	OAM[6].Attrib2=166;
			OAM[7].Attrib0=0x2000+y+8;	OAM[7].Attrib1=x+16;	OAM[7].Attrib2=168;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==left) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=192;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=86;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=194;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=90;	
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=186;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=198;						
			OAM[6].Attrib0=0x2000+y;	OAM[6].Attrib1=x-8;	OAM[6].Attrib2=200;
			OAM[7].Attrib0=0x2000+y+8;	OAM[7].Attrib1=x-8;	OAM[7].Attrib2=202;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
	}

	if(estilo==spz_3) {	
		if(lado==down) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=0+lado;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=2+lado;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=4+lado;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=6+lado;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=112;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=114;
			OAM[6].Attrib0=0x2000+y+24;	OAM[6].Attrib1=x;		OAM[6].Attrib2=116;
			OAM[7].Attrib0=0x2000+y+24;	OAM[7].Attrib1=x+8;	OAM[7].Attrib2=118;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==up) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=134;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=30;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=32;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=34;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=40;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=54;
			OAM[6].Attrib0=0x2000+160;	OAM[6].Attrib1=240;	OAM[6].Attrib2=0;
			OAM[7].Attrib0=0x2000+160;	OAM[7].Attrib1=240;	OAM[7].Attrib2=0;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==right) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=56;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=58;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=60;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=150;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=152;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=154;
			OAM[6].Attrib0=0x2000+y+8;	OAM[6].Attrib1=x+16;	OAM[6].Attrib2=156;
			OAM[7].Attrib0=0x2000+y+8;	OAM[7].Attrib1=x+24;	OAM[7].Attrib2=158;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
		if(lado==left) {	
			OAM[0].Attrib0=0x2000+y;	OAM[0].Attrib1=x;		OAM[0].Attrib2=84;
			OAM[1].Attrib0=0x2000+y;	OAM[1].Attrib1=x+8;	OAM[1].Attrib2=86;
			OAM[2].Attrib0=0x2000+y+8;	OAM[2].Attrib1=x;		OAM[2].Attrib2=204;
			OAM[3].Attrib0=0x2000+y+8;	OAM[3].Attrib1=x+8;	OAM[3].Attrib2=90;
			OAM[4].Attrib0=0x2000+y+16;	OAM[4].Attrib1=x;		OAM[4].Attrib2=206;
			OAM[5].Attrib0=0x2000+y+16;	OAM[5].Attrib1=x+8;	OAM[5].Attrib2=208;
			OAM[6].Attrib0=0x2000+y+8;	OAM[6].Attrib1=x-8;	OAM[6].Attrib2=212;
			OAM[7].Attrib0=0x2000+y+8;	OAM[7].Attrib1=x-16;	OAM[7].Attrib2=210;
			OAM[8].Attrib0=0x2000+160;	OAM[8].Attrib1=240;	OAM[8].Attrib2=0;
		}
	}
}

void Muestra_prsj1_(u16 x, u16 y, u16 frame, u16 lado, u16 estilo)
{
	Muestra_prsj1_XY(x,y,frame, lado, estilo);
	wait_vbl_done();
}

u16 superficie(u16 _x, u16 _y, u16 opt)
{
	return (MyMap[(_y+7)/8][(_x+4)/8]==opt);
}

void InitSprites(void)
{
	DMA3Call((u32)&prsj1_raw, (u32)OBJCHARDATA, 0x84000980);
	DMA3Call((u32)&prsj1_pal, (u32)OBJ_PLTT, 0x84000080);
}

void Muestra_prsj1(int x, int y)
{

u16 new_x=x, new_y=y, spazo_i=0, i;
u16 cambioframe, longitud = 4;
u16 frame;
u16 boton;
u16 lado = ss, estilo = normal;
u16 vel, velMin = 1, velMax=2;

while(DreamSoft_Never_Die) {

	boton = joypad();

	if(boton) {

		if(frame>7) frame = 0;

		if (!(boton & J_B)) vel=velMax;
		if ((boton & J_B)) vel=velMin;

		if (!(boton & J_A)) { if(spazo_i%7==0)estilo=spazo[spazo_i/7]; spazo_i++; if(spazo_i>4*7) {spazo_i=0; estilo=normal; frame = 0;} }

		if (!(boton & J_DOWN))
		{ if(cambioframe==longitud){frame++; cambioframe=0;} new_y=y+vel; cambioframe++; lado=down;}
		if (!(boton & J_UP))
		{ if(cambioframe==longitud){frame++; cambioframe=0;}; new_y=y-vel; cambioframe++; lado=up;}
		if (!(boton & J_RIGHT))
		{ if(cambioframe==longitud){frame++; cambioframe=0;} new_x=x+vel; cambioframe++; lado=right;}
		if (!(boton & J_LEFT))
		{ if(cambioframe==longitud){frame++; cambioframe=0;} new_x=x-vel; cambioframe++; lado=left;}

	}

	if ( x != new_x && superficie(new_x,y,CAMINABLE) ) x = new_x;
	if ( y != new_y && superficie(x,new_y,CAMINABLE) ) y = new_y;

	if(x==240) for(; ;x--) { Muestra_prsj1_(x,y,frames[frame], lado, estilo); if(x<=1) break;}

	Muestra_prsj1_(x,y,frames[frame], lado, estilo);
}

}
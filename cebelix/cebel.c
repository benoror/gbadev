#define DreamSoft_Never_Die 1

#define TRUE 1
#define FALSE 0

#define _A_ 0
#define _B_ 4
#define _C_ 8
#define _D_ 12
#define _E_ 16
#define _F_ 20
#define _RIGHT_ 0
#define _LEFT_ 28

#define w_cebel 16
#define h_cebel 16

#define down_d 1
#define up_d 0

extern u32 cebel;
extern u32 cebel_pal;

void cebel_XY(u16 x, u16 y, u16 frame, u16 lado)
{
	OAM[0].Attrib0 = 0x2000+y;
	OAM[0].Attrib1 = x;
	OAM[0].Attrib2 = 0+lado;

	OAM[1].Attrib0 = 0x2000+y;
	OAM[1].Attrib1 = x+8;
	OAM[1].Attrib2 = 2+lado;

	OAM[2].Attrib0 = 0x2000+y+8;
	OAM[2].Attrib1 = x;
	OAM[2].Attrib2 = (4+frame)+lado;

	OAM[3].Attrib0 = 0x2000+y+8;
	OAM[3].Attrib1 = x+8;
	OAM[3].Attrib2 = (6+frame)+lado;
}

u16 nace[] = {
/*1*/ 58,58,60,62,
/*2*/ 58,58,64,66,
/*3*/ 68,70,72,74,
/*4*/ 76,78,80,82,
/*5*/ 84,86,88,90,
/*6*/ 92,94,88,90
};

void ciclo_nace(u16 x, u16 y)
{
	u16 i,frame=0;

	for(i=0;i>=0;i++)
	{
		OAM[0].Attrib0 = 0x2000+y;
		OAM[0].Attrib1 = x;
		OAM[0].Attrib2 = nace[frame];

		OAM[1].Attrib0 = 0x2000+y;
		OAM[1].Attrib1 = x+8;
		OAM[1].Attrib2 = nace[frame+1];

		OAM[2].Attrib0 = 0x2000+y+8;
		OAM[2].Attrib1 = x;
		OAM[2].Attrib2 = nace[frame+2];

		OAM[3].Attrib0 = 0x2000+y+8;
		OAM[3].Attrib1 = x+8;
		OAM[3].Attrib2 = nace[frame+3];

		wait_vbl_done();
		
		frame+=4;
	}
}

void InitSprites(void)
{
	DMA3Call((u32)&cebel, (u32)OBJCHARDATA, 0x84000680);
	DMA3Call((u32)&cebel_pal, (u32)OBJ_PLTT, 0x84000080);
}

void ciclo_cebel(u16 x, u16 y, u16 superficie)
{
u16 i,boton;
u16 cambio = 4, C_cambio, vel = 1;
u16 C_salto, salto = FALSE, C_retrasoB, retrasoB = 3, D_salto;
u16 lado = _RIGHT_, frame, frames[10] = { _A_, _B_, _C_, _B_, _A_, _D_, _E_, _F_, _E_, _D_};

InitSprites();

while(DreamSoft_Never_Die)
	{
	boton = joypad();

	if (!(boton & J_RIGHT))	{
					if(C_cambio==cambio){ frame++; C_cambio=0;}
					C_cambio++; x+=vel; lado=_RIGHT_;
					}
	if (!(boton & J_LEFT)) 	{if(C_cambio==cambio){ frame++; C_cambio=0;}
					C_cambio++; x-=vel; lado=_LEFT_;
					}
	if (!(boton & J_A))
					{
					C_salto++;
					if(C_salto<=55 && C_salto<0) y--;
					if(C_salto>=55) y++;
					salto=TRUE;
					//D_salto=0;
					}


	if ( (boton & J_B)) { vel=1; } else { vel=3; }

	if(frame>=10) frame = 0;
	cebel_XY(x,y,frames[frame],lado);
	wait_vbl_done();
	}
}
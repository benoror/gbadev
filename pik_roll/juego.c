#include "mapa.c"

u16 posX[6]= { 8,24,40,56,72,88, };

u16 pzY[6];
u16 a0_, a1_, a2_, a3_, a4_, a5_; 
#define pz_ancho 16
#define pz_alto 16
u16 retraso;

void pieza(u16 pzY0,u16 pzY1,u16 pzY2,u16 pzY3,u16 pzY4,u16 pzY5, u16 a0_, u16 a1_, u16 a2_, u16 a3_, u16 a4_, u16 a5_)
{
		if(a0_==1) {
		OAM[0].Attrib0=0x2000+pzY0;		OAM[0].Attrib1=posX[0];		OAM[0].Attrib2=0;
		OAM[1].Attrib0=0x2000+pzY0;		OAM[1].Attrib1=posX[0]+8;	OAM[1].Attrib2=2;
		OAM[2].Attrib0=0x2000+pzY0+8;		OAM[2].Attrib1=posX[0];		OAM[2].Attrib2=4;
		OAM[3].Attrib0=0x2000+pzY0+8;		OAM[3].Attrib1=posX[0]+8;	OAM[3].Attrib2=6;
		}

		if(a1_==1) {
		OAM[16].Attrib0=0x2000+pzY1;		OAM[16].Attrib1=posX[1];	OAM[16].Attrib2=0;
		OAM[17].Attrib0=0x2000+pzY1;		OAM[17].Attrib1=posX[1]+8;	OAM[17].Attrib2=2;
		OAM[18].Attrib0=0x2000+pzY1+8;	OAM[18].Attrib1=posX[1];	OAM[18].Attrib2=4;
		OAM[19].Attrib0=0x2000+pzY1+8;	OAM[19].Attrib1=posX[1]+8;	OAM[19].Attrib2=6;
		}

		if(a2_==1) {
		OAM[4].Attrib0=0x2000+pzY2;		OAM[4].Attrib1=posX[2];		OAM[4].Attrib2=0;
		OAM[5].Attrib0=0x2000+pzY2;		OAM[5].Attrib1=posX[2]+8;	OAM[5].Attrib2=2;
		OAM[6].Attrib0=0x2000+pzY2+8;		OAM[6].Attrib1=posX[2];		OAM[6].Attrib2=4;
		OAM[7].Attrib0=0x2000+pzY2+8;		OAM[7].Attrib1=posX[2]+8;	OAM[7].Attrib2=6;
		}

		if(a3_==1) {
		OAM[20].Attrib0=0x2000+pzY3;		OAM[20].Attrib1=posX[3];	OAM[20].Attrib2=0;
		OAM[21].Attrib0=0x2000+pzY3;		OAM[21].Attrib1=posX[3]+8;	OAM[21].Attrib2=2;
		OAM[22].Attrib0=0x2000+pzY3+8;	OAM[22].Attrib1=posX[3];	OAM[22].Attrib2=4;
		OAM[23].Attrib0=0x2000+pzY3+8;	OAM[23].Attrib1=posX[3]+8;	OAM[23].Attrib2=6;
		}

		if(a4_==1) {
		OAM[8].Attrib0=0x2000+pzY4;		OAM[8].Attrib1=posX[4];		OAM[8].Attrib2=0;
		OAM[9].Attrib0=0x2000+pzY4;		OAM[9].Attrib1=posX[4]+8;	OAM[9].Attrib2=2;
		OAM[10].Attrib0=0x2000+pzY4+8;	OAM[10].Attrib1=posX[4];	OAM[10].Attrib2=4;
		OAM[11].Attrib0=0x2000+pzY4+8;	OAM[11].Attrib1=posX[4]+8;	OAM[11].Attrib2=6;
		}

		if(a5_==1) {
		OAM[12].Attrib0=0x2000+pzY5;		OAM[12].Attrib1=posX[5];	OAM[12].Attrib2=0;
		OAM[13].Attrib0=0x2000+pzY5;		OAM[13].Attrib1=posX[5]+8;	OAM[13].Attrib2=2;
		OAM[14].Attrib0=0x2000+pzY5+8;	OAM[14].Attrib1=posX[5];	OAM[14].Attrib2=4;
		OAM[15].Attrib0=0x2000+pzY5+8;	OAM[15].Attrib1=posX[5]+8;	OAM[15].Attrib2=6;
		}
}

void init_var(void)
{
	pzY[0]=pzY[1]=pzY[2]=pzY[3]=pzY[4]=pzY[5]=8;
	retraso=50;
	a0_=1;	a1_=1;	a2_=1;	a3_=0;	a4_=1;	a5_=0;
}

void Main()
{
	u16 i,j,boton,ok=0;

	init_props_map();

	REINICIA:
	init_var();

	while(1) {
		j++;

		boton=joypad();
		if(!(boton & J_A)) retraso=10;
		if((boton & J_A)) retraso=50;

		pieza(pzY[0],pzY[1],pzY[2],pzY[3],pzY[4],pzY[5],a0_,a1_,a2_,a3_,a4_,a5_);

		if(j>=retraso) {
			if((pzY[0]<mapV[0].y-pz_alto) && a0_!=0) { pzY[0]++; j=0; }
				else if((pzY[0]>=mapV[0].y-pz_alto) && a0_!=0) { mapV[0].y-=16; ok++; }
			if((pzY[1]<mapV[1].y-pz_alto) && a1_!=0) { pzY[1]++; j=0; }
				else if((pzY[1]>mapV[1].y-pz_alto) && a1_!=0)  { mapV[1].y-=16; ok++; }
			if((pzY[2]<mapV[2].y-pz_alto) && a2_!=0) { pzY[2]++; j=0; }
				else if((pzY[2]>mapV[2].y-pz_alto) && a2_!=0)  { mapV[2].y-=16; ok++; }
			if((pzY[3]<mapV[3].y-pz_alto) && a3_!=0) { pzY[3]++; j=0; }
				else if((pzY[3]>mapV[3].y-pz_alto) && a3_!=0)  { mapV[3].y-=16; ok++; }
			if((pzY[4]<mapV[4].y-pz_alto) && a4_!=0) { pzY[4]++; j=0; }
				else if((pzY[4]>mapV[4].y-pz_alto) && a4_!=0)  { mapV[4].y-=16; ok++; }
			if((pzY[5]<mapV[5].y-pz_alto) && a5_!=0) { pzY[5]++; j=0; }
				else if((pzY[5]>mapV[5].y-pz_alto) && a5_!=0)  { mapV[5].y-=16; ok++; }

		if(ok>=6) { goto REINICIA; ok=0; }
		}

		wait_vbl_done();
	}
}
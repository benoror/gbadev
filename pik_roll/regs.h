#define DISPCNT		*(u32*)0x04000000			//Sets DISPCNT to be the register for it.  Easier than using the hex every time
#define BG0CNT		*(u16*)0x04000008			//Sets BG0CNT to be the register for it.  Easier than using the hex every time
#define BG1CNT     	*(u16*)0x0400000A
#define BG2CNT     	*(u16*)0x0400000C
#define BG3CNT     	*(u16*)0x0400000E

#define DMA3SAD		*(u32*)0x040000d4			//setups up all display variables.  read the official docs for more info
#define DMA3DAD		*(u32*)0x040000d8
#define DMA3CNT		*(u32*)0x040000dc
#define P1			*(u16*)0x04000130
#define OAM		((Sprite*)0x07000000)
#define OBJCHARDATA	((u8*)0x06010000)
#define OBJ_PLTT	((u16*)0x05000200)

#define BG0_DATA    ((u16*)0x6004000)
#define BG_PLTT    ((u16*)0x5000000)

#define VBUFFER    ((u16*)0x6000000)

#define FALSE		0
#define TRUE		1

#define J_RIGHT		0x0010
#define J_LEFT		0x0020
#define J_UP		0x0040
#define J_DOWN		0x0080
#define J_START		0x0008
#define J_SELECT	0x0004
#define J_A		0x0001
#define J_B		0x0002
#define J_L		0x0200
#define J_R		0x0100

typedef unsigned char u8;						//some type definitions
typedef unsigned short u16;
typedef unsigned long u32;

typedef struct _Sprite { u16 Attrib0, Attrib1, Attrib2, RotateScale; } Sprite;		//define a sprite as a structure

u16 joypad() 									//joypad checking routine.  
{
	u16 j=P1;
	P1 |= 0x03FF;
	return j;
}


void DMA3Call(u32 Src,u32 Dst,u32 Cnt)				//used to load our sprite info into vram			
{
	DMA3SAD = Src;									//where the data is
	DMA3DAD = Dst;									//where the data goes
	DMA3CNT = Cnt;									//how much data should be loaded
}

void wait_vbl_done(void)
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

void Wait( u32 HowManyVSyncs )
{
	u8 i,j;
	u32 Waiter;
	for( Waiter = 0; Waiter < HowManyVSyncs; Waiter++ )
	{
		wait_vbl_done();
		for( i = 0; i < 100; i++ ) j = i*32/3; // Do something stupid
	}
}

//Fades de pantalla
#define COLEV      *(u16 *)0x04000052
#define COLY       *(u16 *)0x04000054
#define BLDMOD     *(u16 *)0x04000050

void fade_out(u8 retardo_fin, u8 sprfade, u8 bn)
{
 u8 Phase;
 u8 retardo_ini;

 if (sprfade)       //Si sprfade == 1, hacemos fade de los sprites también...
      	{
	if (bn == 0) { BLDMOD = 0x00FF;  }	// Si bn == 0 (negro)
	if (bn == 1) { BLDMOD = 0x00BF;  }	// Si bn == 1 (blanco)
	}
 else               //...si no, sólo de los fondos
   	{
	if (bn == 0) { BLDMOD =0x00EF;  }	// Si bn == 0 (negro)
	if (bn == 1) { BLDMOD = 0x00AF;  }	// Si bn == 1 (blanco)
	}
   
 for( Phase = 0; Phase < 20; Phase++ )
 {
  COLY = Phase;
  
  for (retardo_ini=0; retardo_ini!=retardo_fin; retardo_ini++)
  {
    wait_vbl_done();  wait_vbl_done();  wait_vbl_done();
  }  
 }
}

void fade_in(u8 retardo_fin, u8 sprfade, u8 bn)
{
 int Phase;
 u8 retardo_ini;

 if (sprfade)       //Si sprfade == 1, hacemos fade de los sprites también...
      	{
	if (bn == 0) { BLDMOD = 0x00FF;  }	// Si bn == 0 (negro)
	if (bn == 1) { BLDMOD = 0x00BF;  }	// Si bn == 1 (blanco)
	}
 else               //...si no, sólo de los fondos
   	{
	if (bn == 0) { BLDMOD = 0x00EF;  }	// Si bn == 0 (negro)
	if (bn == 1) { BLDMOD = 0x00AF;  }	// Si bn == 1 (blanco)
	}

 for( Phase = 20; Phase >= 0; Phase-- )
 {
  COLY = Phase;
  
  for (retardo_ini=0; retardo_ini!=retardo_fin; retardo_ini++)
  {
    wait_vbl_done();  wait_vbl_done();  wait_vbl_done();
  } 
 }
}

void Sonido(u16 delay, int sound)
{

	u16 i, C_sound[2];

	C_sound[0] = sound;
	C_sound[1] = 0;

	for(i=0;i<2;i++)
	{
	Wait(delay);
	*(u16*)0x400006C = C_sound[i];
	}
}
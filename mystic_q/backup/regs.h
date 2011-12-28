#define DISPCNT	*(u32*)0x04000000

#define BG0CNT	*(u16*)0x04000008
#define BG1CNT     	*(u16*)0x0400000A
#define BG2CNT     	*(u16*)0x0400000C
#define BG3CNT     	*(u16*)0x0400000E

#define VBUFFER    ((u16*)0x6000000)
#define BG0Data    ((u16*)0x6004000)
#define BG1Data    ((u16*)0x6008000)
#define BG2Data    ((u16*)0x600C000)

#define DMA3SAD		*(u32*)0x040000d4			//setups up all display variables.  read the official docs for more info
#define DMA3DAD		*(u32*)0x040000d8
#define DMA3CNT		*(u32*)0x040000dc

#define P1		*(u16*)0x04000130

#define OAM		((Sprite*)0x07000000)

#define BG_PLTT    ((u16*)0x5000000)
#define OBJ_PLTT	((u16*)0x05000200)

#define OBJCHARDATA	((u8*)0x06010000)

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

void delay(unsigned long int fin)
{
	u8 inicio;

	for (inicio = 0;inicio != fin; inicio++)
	{
		wait_vbl_done();  // wait_vbl_done();wait_vbl_done();
	}
}

//Fades de pantalla
#define COLEV      *(u16 *)0x04000052
#define COLY       *(u16 *)0x04000054
#define BLDMOD     *(u16 *)0x04000050
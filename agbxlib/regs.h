typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef struct _Sprite { u16 Attrib0, Attrib1, Attrib2, RotateScale; } Sprite;

 // Registers
#define DISPCNT		*(u32*)0x04000000
#define DISPSTAT		*(u16*)0x04000004
#define VCOUNT		*(u16*)0x04000006

u16 *BKGPal=(u16*)0x5000000;
//BackGround Character Data Blocks (Almacena las tiles de cada fondo)
u8  *BKG0Data=(u8  *)0x6000000; 
u8  *BKG1Data=(u8  *)0x6004000; 
u8  *BKG2Data=(u8  *)0x6008000; 
u8  *BKG3Data=(u8  *)0x600C000; 

//BackGround Screen Data Block     (Acceso la memoria de cada fondo)
u16 *BKG0Screen=(u16 *)0x6003800;       
u16 *BKG1Screen=(u16 *)0x6007800;
u16 *BKG2Screen=(u16 *)0x600B800;
u16 *BKG3Screen=(u16 *)0x600F800;

#define BG0CNT		*(u16*)0x04000008
#define BG1CNT     	*(u16*)0x0400000A
#define BG2CNT     	*(u16*)0x0400000C
#define BG3CNT     	*(u16*)0x0400000E
#define BG0HOFS		*(u16*)0x04000010       //Scroll Horizontal BKG0
#define BG0VOFS		*(u16*)0x04000012       //Scroll Vertical   BKG0
#define BG1HOFS         *(u16*)0x04000014       //Scroll Horizontal BKG1
#define BG1VOFS         *(u16*)0x04000016       //Scroll Vertical   BKG1
#define BG2HOFS         *(u16*)0x04000018       //Scroll Horizontal BKG2
#define BG2VOFS         *(u16*)0x0400001A       //Scroll Vertical   BKG2
#define BG3HOFS         *(u16*)0x0400001C       //Scroll Horizontal BKG3
#define BG3VOFS         *(u16*)0x0400001E       //Scroll Vertical   BKG3

#define MOSAIC		*(u16*)0x0400004C
#define P1			*(u16*)0x04000130

#define DMA0SAD     *(u32*)0x40000B0
#define DMA0DAD     *(u32*)0x40000B4
#define DMA0CNT     *(u32*)0x40000B8
#define DMA1SAD     *(u32*)0x40000BC
#define DMA1DAD     *(u32*)0x40000C0
#define DMA1CNT     *(u32*)0x40000C4
#define DMA2SAD     *(u32*)0x40000C8
#define DMA2DAD     *(u32*)0x40000CC
#define DMA2CNT     *(u32*)0x40000D0
#define DMA3SAD		*(u32*)0x040000d4
#define DMA3DAD		*(u32*)0x040000d8
#define DMA3CNT		*(u32*)0x040000dc

	//bkg scaling starting points
#define	BG2X_L	*(u16*)0x04000028
#define	BG2X_H	*(u16*)0x0400002A
#define	BG2Y_L	*(u16*)0x0400002C
#define	BG2Y_H	*(u16*)0x0400002E

	//bkg scaling movement
#define	BG2PA		*(u16*)0x04000020
#define	BG2PB		*(u16*)0x04000022
#define	BG2PC		*(u16*)0x04000024
#define	BG2PD		*(u16*)0x04000026

#define BG_PLTT		((u16*)0x05000000)
#define OBJCHR		((u8 *)0x06010000)
#define OBJPAL		((u16 *)0x05000200)
#define VBUFFER		((u16*)0x06000000)
#define OAM		((Sprite*)0x07000000)




 // Function to wait for Vertical Sync. I took Eloist's ASM Version
void WaitForVSync()
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

#define J_A		0x0001
#define J_B		0x0002
#define J_SELECT		0x0004
#define J_START		0x0008
#define J_RIGHT		0x0010
#define J_LEFT		0x0020
#define J_UP		0x0040
#define J_DOWN		0x0080
#define J_R		0x0100
#define J_L		0x0200

#define key_reg	(*(u16*)0x04000130)
u16 joypad() 	
{
	u16 j=P1;
	P1 |= 0x03FF;
	return j;
}
#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512


#define SHAPE_SQUARE       0  //Cuadrado
#define SHAPE_H_RECT       1  //Rectángulo horizontal
#define SHAPE_V_RECT       2  //Rectángulo vertical

#define SIZE_8x8          0
#define SIZE_16x8         0
#define SIZE_8x16         0
#define SIZE_16x16        1
#define SIZE_32x8         1
#define SIZE_8x32         1
#define SIZE_32x32        2
#define SIZE_32x16        2
#define SIZE_16x32        2
#define SIZE_64x64        3
#define SIZE_64x32        3
#define SIZE_32x64        3

#define FLIP_X          1
#define FLIP_Y          1
#define NO_FLIP_X       0
#define NO_FLIP_Y       0


#define TM0D    *(u16 *)0x04000100
#define TM0CNT  *(u16 *)0x04000102
u16 *Timer0Data = &TM0D;

u16 scrollH, scrollV;
u16 scrollH1, scrollV1;
u16 scrollH2, scrollV2;
u16 scrollH3, scrollV3;

#define COLEV      *(u16 *)0x04000052
#define COLY       *(u16 *)0x04000054
#define BLDMOD     *(u16 *)0x04000050


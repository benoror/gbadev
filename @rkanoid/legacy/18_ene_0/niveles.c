#define O1 2  // Presionado 1
#define O2 3  // Presionado 2
#define N1 4  // Nada 1
#define N2 5  // Nada 2
#define mes1 6 // Mesías
#define mes2 7 // Mesías
#define j1 18  // Rojo 1
#define j2 19  // Rojo 2
#define v1 20  // Verde 1
#define v2 21  // Verde 2
#define a1 22  // Amarillo 1
#define a2 23  // Amarillo 2
#define z1 24  // Azul 1
#define z2 25  // Azul 2
#define c1 26  // Cafe 1
#define c2 27  // Cafe 2
#define g1 28  // Gris 1
#define g2 29  // Gris 2
#define m1 30  // Morado 1
#define m2 31  // Morado 2
#define r1 32  // Rosa 1
#define r2 33  // Rosa 2
#define s1 34  // Smile 1
#define s2 35  // Smile 2
#define t1 36  // Titanium 1
#define t2 37  // Titanium 2
#define i1 38  // Iris 1
#define i2 39  // Iris 2

#define NO_BONUS 666
#define COMET_BONUS 52
#define LONG_R_BONUS 56
#define LIFE_BONUS 60
#define BIGBALL_BONUS 64
#define MESIAS_BONUS 68

#define TRUE 1
#define FALSE 0

typedef int boolean;

typedef struct
	{
		int x, y, xM, yM, tile1, tile2;
		boolean destruye;
		u8 golpes;
		u16 BONUS, activoBonus, bonX, bonY, bonN;
	} Bloques;

Bloques L[162];		// Array de 162 espacios (o sea 9*18 bloques)

/******************************************/

u8 bgMap[] = {
1,0,10,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	17,17,	11,1,0,1,0,1,0,1,0,

1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,40,41,42,43,44,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,8,9,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,45,46,47,48,49,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,
1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,

1,0,14,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	15,1,0,1,0,1,0,1,0,1

};

u8 blockMap_1[] = {
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
t1,t2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	t1,t2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
};

u8 blockMap_2[] = {
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	a1,a2,	N1,N2,	v1,v2,	N1,N2,	a1,a2,	N1,N2,	v1,v2,	N1,N2,
z1,z2,	N1,N2,	z1,z2,	N1,N2,	z1,z2,	N1,N2,	z1,z2,	N1,N2,	z1,z2,
N1,N2,	N1,N2,	c1,c2,	N1,N2,	r1,r2,	N1,N2,	c1,c2,	N1,N2,	N1,N2,
t1,t2,	m1,m2,	N1,N2,	a1,a2,	N1,N2,	m1,m2,	N1,N2,	a1,a2,	t1,t2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
s1,s2,	i1,i2,	i1,i2,	i1,i2,	i1,i2,	i1,i2,	i1,i2,	i1,i2,	s1,s2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
};

u8 blockMap_3[] = {
s1,s2,	z1,z2,	m1,m2,	r1,r2,	j1,j2,	c1,c2,	g1,g2,	a1,a2,	v1,v2,
z1,z2,	m1,m2,	r1,r2,	j1,j2,	c1,c2,	g1,g2,	a1,a2,	v1,v2,	N1,N2,
m1,m2,	r1,r2,	j1,j2,	c1,c2,	g1,g2,	a1,a2,	v1,v2,	N1,N2,	N1,N2,
r1,r2,	j1,j2,	c1,c2,	g1,g2,	a1,a2,	v1,v2,	N1,N2,	N1,N2,	N1,N2,
j1,j2,	c1,c2,	g1,g2,	a1,a2,	v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
c1,c2,	g1,g2,	a1,a2,	v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
g1,g2,	a1,a2,	v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
a1,a2,	v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
i1,i2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	t1,t2,	i1,i2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
};

u8 blockMap_4[] = {
s1,s2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	s1,s2,
t1,t2,	N1,N2,	N1,N2,	g1,g2,	g1,g2,	g1,g2,	N1,N2,	N1,N2,	t1,t2,
N1,N2,	N1,N2,	g1,g2,	z1,z2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,	N1,N2,
N1,N2,	g1,g2,	z1,z2,	z1,z2,	z1,z2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	g1,g2,	z1,z2,	z1,z2,	z1,z2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	g1,g2,	z1,z2,	z1,z2,	z1,z2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	g1,g2,	z1,z2,	z1,z2,	z1,z2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	g1,g2,	m1,m2,	z1,z2,	m1,m2,	m1,m2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	g1,g2,	r1,r2,	m1,m2,	m1,m2,	r1,r2,	r1,r2,	g1,g2,	N1,N2,
N1,N2,	N1,N2,	g1,g2,	r1,r2,	r1,r2,	r1,r2,	g1,g2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	g1,g2,	g1,g2,	g1,g2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
};

u8 blockMap_5[] = {
N1,N2,	N1,N2,	m1,m2,	N1,N2,	c1,c2,	N1,N2,	m1,m2,	N1,N2,	N1,N2,
v1,v2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	v1,v2,
N1,N2,	v1,v2,	N1,N2,	N1,N2,	t1,t2,	N1,N2,	N1,N2,	v1,v2,	N1,N2,
N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,
N1,N2,	z1,z2,	N1,N2,	t1,t2,	j1,j2,	t1,t2,	N1,N2,	z1,z2,	N1,N2,
N1,N2,	N1,N2,	z1,z2,	N1,N2,	s1,s2,	N1,N2,	z1,z2,	N1,N2,	N1,N2,
r1,r2,	N1,N2,	N1,N2,	t1,t2,	j1,j2,	t1,t2,	N1,N2,	N1,N2,	r1,r2,
r1,r2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	r1,r2,
N1,N2,	N1,N2,	m1,m2,	N1,N2,	t1,t2,	N1,N2,	m1,m2,	N1,N2,	N1,N2,
N1,N2,	g1,g2,	N1,N2,	g1,g2,	N1,N2,	g1,g2,	N1,N2,	g1,g2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,
};

void PropNivel1(void)
{
	u16 i;

	i = 0;
	L[i].x = 80;
	L[i].y = 16;
	L[i].tile1 = 26;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 1;
	L[i].x = 64;
	L[i].y = 24;
	L[i].tile1 = 42;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = MESIAS_BONUS;

	i = 2;
	L[i].x = 96;
	L[i].y = 24;
	L[i].tile1 = 46;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 3;
	L[i].x = 48;
	L[i].y = 32;
	L[i].tile1 = 58;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 4;
	L[i].x = 112;
	L[i].y = 32;
	L[i].tile1 = 66;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LONG_R_BONUS;

	i = 5;
	L[i].x = 64;
	L[i].y = 40;
	L[i].tile1 = 78;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = BIGBALL_BONUS;

	i = 6;
	L[i].x = 96;
	L[i].y = 40;
	L[i].tile1 = 82;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = COMET_BONUS;

	i = 7;
	L[i].x = 80;
	L[i].y = 48;
	L[i].tile1 = 98;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LIFE_BONUS;

	i = 8;
	L[i].x = 16;
	L[i].y = 56;
	L[i].tile1 = 108;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 9;
	L[i].x = 144;
	L[i].y = 56;
	L[i].tile1 = 124;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 10;
	L[i].x = 32;
	L[i].y = 80;
	L[i].tile1 = 164;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 11;
	L[i].x = 48;
	L[i].y = 80;
	L[i].tile1 = 166;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 12;
	L[i].x = 64;
	L[i].y = 80;
	L[i].tile1 = 168;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 13;
	L[i].x = 80;
	L[i].y = 80;
	L[i].tile1 = 170;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 14;
	L[i].x = 96;
	L[i].y = 80;
	L[i].tile1 = 172;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 15;
	L[i].x = 112;
	L[i].y = 80;
	L[i].tile1 = 174;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 16;
	L[i].x = 128;
	L[i].y = 80;
	L[i].tile1 = 176;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;
}

void PropNivel2(void)
{
	u16 i;

	i = 0;
	L[i].x = 32;
	L[i].y = 24;
	L[i].tile1 = 38;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = MESIAS_BONUS;

	i = 1;
	L[i].x = 64;
	L[i].y = 24;
	L[i].tile1 = 42;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 2;
	L[i].x = 96;
	L[i].y = 24;
	L[i].tile1 = 46;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = COMET_BONUS;

	i = 3;
	L[i].x = 128;
	L[i].y = 24;
	L[i].tile1 = 50;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 4;
	L[i].x = 16;
	L[i].y = 32;
	L[i].tile1 = 54;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LIFE_BONUS;

	i = 5;
	L[i].x = 48;
	L[i].y = 32;
	L[i].tile1 = 58;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 6;
	L[i].x = 80;
	L[i].y = 32;
	L[i].tile1 = 62;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 7;
	L[i].x = 112;
	L[i].y = 32;
	L[i].tile1 = 66;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 8;
	L[i].x = 144;
	L[i].y = 32;
	L[i].tile1 = 70;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 9;
	L[i].x = 48;
	L[i].y = 40;
	L[i].tile1 = 76;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = MESIAS_BONUS;

	i = 10;
	L[i].x = 80;
	L[i].y = 40;
	L[i].tile1 = 80;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 11;
	L[i].x = 112;
	L[i].y = 40;
	L[i].tile1 = 84;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = BIGBALL_BONUS;

	i = 12;
	L[i].x = 16;
	L[i].y = 48;
	L[i].tile1 = 90;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 13;
	L[i].x = 32;
	L[i].y = 48;
	L[i].tile1 = 92;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 14;
	L[i].x = 64;
	L[i].y = 48;
	L[i].tile1 = 96;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 15;
	L[i].x = 96;
	L[i].y = 48;
	L[i].tile1 = 100;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 16;
	L[i].x = 128;
	L[i].y = 48;
	L[i].tile1 = 104;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 17;
	L[i].x = 144;
	L[i].y = 48;
	L[i].tile1 = 106;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 18;
	L[i].x = 16;
	L[i].y = 80;
	L[i].tile1 = 162;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 19;
	L[i].x = 32;
	L[i].y = 80;
	L[i].tile1 = 164;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 20;
	L[i].x = 48;
	L[i].y = 80;
	L[i].tile1 = 166;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = COMET_BONUS;

	i = 21;
	L[i].x = 64;
	L[i].y = 80;
	L[i].tile1 = 168;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 22;
	L[i].x = 80;
	L[i].y = 80;
	L[i].tile1 = 170;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 23;
	L[i].x = 96;
	L[i].y = 80;
	L[i].tile1 = 172;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 24;
	L[i].x = 112;
	L[i].y = 80;
	L[i].tile1 = 174;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 25;
	L[i].x = 128;
	L[i].y = 80;
	L[i].tile1 = 176;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = NO_BONUS;

	i = 26;
	L[i].x = 144;
	L[i].y = 80;
	L[i].tile1 = 178;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LONG_R_BONUS;
}

void PropNivel3(void)
{
	u16 i;

	i = 0;
	L[i].x = 16;
	L[i].y = 8;
	L[i].tile1 = 0;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LIFE_BONUS;

	i = 1;
	L[i].x = 32;
	L[i].y = 8;
	L[i].tile1 = 2;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 2;
	L[i].x = 48;
	L[i].y = 8;
	L[i].tile1 = 4;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 3;
	L[i].x = 64;
	L[i].y = 8;
	L[i].tile1 = 6;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 4;
	L[i].x = 80;
	L[i].y = 8;
	L[i].tile1 = 8;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 5;
	L[i].x = 96;
	L[i].y = 8;
	L[i].tile1 = 10;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = MESIAS_BONUS;

	i = 6;
	L[i].x = 112;
	L[i].y = 8;
	L[i].tile1 = 12;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 7;
	L[i].x = 128;
	L[i].y = 8;
	L[i].tile1 = 14;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 8;
	L[i].x = 144;
	L[i].y = 8;
	L[i].tile1 = 16;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 9;
	L[i].x = 16;
	L[i].y = 16;
	L[i].tile1 = 18;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 10;
	L[i].x = 32;
	L[i].y = 16;
	L[i].tile1 = 20;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 11;
	L[i].x = 48;
	L[i].y = 16;
	L[i].tile1 = 22;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 12;
	L[i].x = 64;
	L[i].y = 16;
	L[i].tile1 = 24;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 13;
	L[i].x = 80;
	L[i].y = 16;
	L[i].tile1 = 26;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 14;
	L[i].x = 96;
	L[i].y = 16;
	L[i].tile1 = 28;
	L[i].xM = L[i].x + 16;
	L[i].yM = L[i].y + 8;
	L[i].tile2 = L[i].tile1 + 1;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LONG_R_BONUS;

	i = 15;
	L[i].x = 112;
	L[i].y = 16;
	L[i].tile1 = 30;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 16;
	L[i].x = 128;
	L[i].y = 16;
	L[i].tile1 = 32;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 18;
	L[i].x = 16;
	L[i].y = 24;
	L[i].tile1 = 36;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 19;
	L[i].x = 32;
	L[i].y = 24;
	L[i].tile1 = 38;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 20;
	L[i].x = 48;
	L[i].y = 24;
	L[i].tile1 = 40;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 21;
	L[i].x = 64;
	L[i].y = 24;
	L[i].tile1 = 42;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 22;
	L[i].x = 80;
	L[i].y = 24;
	L[i].tile1 = 44;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 23;
	L[i].x = 96;
	L[i].y = 24;
	L[i].tile1 = 46;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = MESIAS_BONUS;

	i = 24;
	L[i].x = 112;
	L[i].y = 24;
	L[i].tile1 = 48;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 27;
	L[i].x = 16;
	L[i].y = 32;
	L[i].tile1 = 54;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = LIFE_BONUS;

	i = 28;
	L[i].x = 32;
	L[i].y = 32;
	L[i].tile1 = 56;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 29;
	L[i].x = 48;
	L[i].y = 32;
	L[i].tile1 = 58;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 30;
	L[i].x = 64;
	L[i].y = 32;
	L[i].tile1 = 60;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 31;
	L[i].x = 80;
	L[i].y = 32;
	L[i].tile1 = 62;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 32;
	L[i].x = 96;
	L[i].y = 32;
	L[i].tile1 = 64;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 36;
	L[i].x = 16;
	L[i].y = 40;
	L[i].tile1 = 72;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 37;
	L[i].x = 32;
	L[i].y = 40;
	L[i].tile1 = 74;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 38;
	L[i].x = 48;
	L[i].y = 40;
	L[i].tile1 = 76;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 39;
	L[i].x = 64;
	L[i].y = 40;
	L[i].tile1 = 78;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 40;
	L[i].x = 80;
	L[i].y = 40;
	L[i].tile1 = 80;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = BIGBALL_BONUS;

	i = 45;
	L[i].x = 16;
	L[i].y = 48;
	L[i].tile1 = 90;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 46;
	L[i].x = 32;
	L[i].y = 48;
	L[i].tile1 = 92;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 47;
	L[i].x = 48;
	L[i].y = 48;
	L[i].tile1 = 94;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 48;
	L[i].x = 64;
	L[i].y = 48;
	L[i].tile1 = 96;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 54;
	L[i].x = 16;
	L[i].y = 56;
	L[i].tile1 = 108;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 55;
	L[i].x = 32;
	L[i].y = 56;
	L[i].tile1 = 110;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 56;
	L[i].x = 48;
	L[i].y = 56;
	L[i].tile1 = 112;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 63;
	L[i].x = 16;
	L[i].y = 64;
	L[i].tile1 = 126;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 64;
	L[i].x = 32;
	L[i].y = 64;
	L[i].tile1 = 128;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 72;
	L[i].x = 16;
	L[i].y = 72;
	L[i].tile1 = 144;
	L[i].destruye = TRUE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 90;
	L[i].x = 16;
	L[i].y = 88;
	L[i].tile1 = 180;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = COMET_BONUS;

	i = 91;
	L[i].x = 32;
	L[i].y = 88;
	L[i].tile1 = 182;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 92;
	L[i].x = 48;
	L[i].y = 88;
	L[i].tile1 = 184;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 93;
	L[i].x = 64;
	L[i].y = 88;
	L[i].tile1 = 186;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 94;
	L[i].x = 80;
	L[i].y = 88;
	L[i].tile1 = 188;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 95;
	L[i].x = 96;
	L[i].y = 88;
	L[i].tile1 = 190;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 96;
	L[i].x = 112;
	L[i].y = 88;
	L[i].tile1 = 192;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 97;
	L[i].x = 128;
	L[i].y = 88;
	L[i].tile1 = 194;
	L[i].destruye = FALSE;
	L[i].golpes = 0;
	L[i].BONUS = NO_BONUS;

	i = 98;
	L[i].x = 144;
	L[i].y = 88;
	L[i].tile1 = 196;
	L[i].destruye = TRUE;
	L[i].golpes = 5;
	L[i].BONUS = COMET_BONUS;
}

void PropNivel4(void)
{
	u16 i;

      i = 0;
      L[i].x = 16;
      L[i].y = 16;
      L[i].tile1 = 18;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 1;
      L[i].x = 144;
      L[i].y = 16;
      L[i].tile1 = 34;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 2;
      L[i].x = 16;
      L[i].y = 8;
      L[i].tile1 = 0;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = COMET_BONUS;

      i = 3;
      L[i].x = 144;
      L[i].y = 8;
      L[i].tile1 = 16;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 4;
      L[i].x = 64;
      L[i].y = 16;
      L[i].tile1 = 24;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 5;
      L[i].x = 80;
      L[i].y = 16;
      L[i].tile1 = 26;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = LONG_R_BONUS;

      i = 6;
      L[i].x = 96;
      L[i].y = 16;
      L[i].tile1 = 28;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 7;
      L[i].x = 48;
      L[i].y = 24;
      L[i].tile1 = 40;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 8;
      L[i].x = 112;
      L[i].y = 24;
      L[i].tile1 = 48;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 9;
      L[i].x = 32;
      L[i].y = 32;
      L[i].tile1 = 56;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 10;
      L[i].x = 32;
      L[i].y = 40;
      L[i].tile1 = 74;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = LIFE_BONUS;

      i = 11;
      L[i].x = 32;
      L[i].y = 48;
      L[i].tile1 = 92;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 12;
      L[i].x = 32;
      L[i].y = 56;
      L[i].tile1 = 110;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 13;
      L[i].x = 32;
      L[i].y = 64;
      L[i].tile1 = 128;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 14;
      L[i].x = 32;
      L[i].y = 72;
      L[i].tile1 = 146;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 15;
      L[i].x = 48;
      L[i].y = 80;
      L[i].tile1 = 166;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = BIGBALL_BONUS;

      i = 16;
      L[i].x = 64;
      L[i].y = 88;
      L[i].tile1 = 186;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 17;
      L[i].x = 80;
      L[i].y = 88;
      L[i].tile1 = 188;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 18;
      L[i].x = 96;
      L[i].y = 88;
      L[i].tile1 = 190;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 19;
      L[i].x = 112;
      L[i].y = 80;
      L[i].tile1 = 174;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 20;
      L[i].x = 128;
      L[i].y = 72;
      L[i].tile1 = 158;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = MESIAS_BONUS;

      i = 21;
      L[i].x = 128;
      L[i].y = 64;
      L[i].tile1 = 140;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 22;
      L[i].x = 128;
      L[i].y = 56;
      L[i].tile1 = 122;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 23;
      L[i].x = 128;
      L[i].y = 48;
      L[i].tile1 = 104;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 24;
      L[i].x = 128;
      L[i].y = 40;
      L[i].tile1 = 86;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 25;
      L[i].x = 128;
      L[i].y = 32;
      L[i].tile1 = 68;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = COMET_BONUS;

      i = 26;
      L[i].x = 64;
      L[i].y = 24;
      L[i].tile1 = 42;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 27;
      L[i].x = 80;
      L[i].y = 24;
      L[i].tile1 = 44;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 28;
      L[i].x = 96;
      L[i].y = 24;
      L[i].tile1 = 46;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 29;
      L[i].x = 48;
      L[i].y = 32;
      L[i].tile1 = 58;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 30;
      L[i].x = 48;
      L[i].y = 40;
      L[i].tile1 = 76;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = LONG_R_BONUS;

      i = 31;
      L[i].x = 48;
      L[i].y = 48;
      L[i].tile1 = 94;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 32;
      L[i].x = 48;
      L[i].y = 56;
      L[i].tile1 = 112;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 33;
      L[i].x = 48;
      L[i].y = 64;
      L[i].tile1 = 130;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 34;
      L[i].x = 48;
      L[i].y = 72;
      L[i].tile1 = 148;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 35;
      L[i].x = 64;
      L[i].y = 72;
      L[i].tile1 = 150;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = LIFE_BONUS;

      i = 36;
      L[i].x = 64;
      L[i].y = 64;
      L[i].tile1 = 132;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 37;
      L[i].x = 64;
      L[i].y = 56;
      L[i].tile1 = 114;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 38;
      L[i].x = 64;
      L[i].y = 48;
      L[i].tile1 = 96;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 39;
      L[i].x = 64;
      L[i].y = 40;
      L[i].tile1 = 78;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 40;
      L[i].x = 64;
      L[i].y = 32;
      L[i].tile1 = 60;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = BIGBALL_BONUS;

      i = 41;
      L[i].x = 80;
      L[i].y = 32;
      L[i].tile1 = 62;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 42;
      L[i].x = 80;
      L[i].y = 40;
      L[i].tile1 = 80;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 43;
      L[i].x = 80;
      L[i].y = 48;
      L[i].tile1 = 98;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 44;
      L[i].x = 80;
      L[i].y = 56;
      L[i].tile1 = 116;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 45;
      L[i].x = 80;
      L[i].y = 64;
      L[i].tile1 = 134;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 46;
      L[i].x = 80;
      L[i].y = 72;
      L[i].tile1 = 152;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = MESIAS_BONUS;

      i = 47;
      L[i].x = 96;
      L[i].y = 72;
      L[i].tile1 = 154;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 48;
      L[i].x = 96;
      L[i].y = 64;
      L[i].tile1 = 136;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 49;
      L[i].x = 96;
      L[i].y = 56;
      L[i].tile1 = 118;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 50;
      L[i].x = 96;
      L[i].y = 48;
      L[i].tile1 = 100;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 51;
      L[i].x = 96;
      L[i].y = 40;
      L[i].tile1 = 82;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = COMET_BONUS;

      i = 52;
      L[i].x = 96;
      L[i].y = 32;
      L[i].tile1 = 64;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 53;
      L[i].x = 112;
      L[i].y = 32;
      L[i].tile1 = 66;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 54;
      L[i].x = 112;
      L[i].y = 40;
      L[i].tile1 = 84;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 55;
      L[i].x = 112;
      L[i].y = 48;
      L[i].tile1 = 102;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 56;
      L[i].x = 112;
      L[i].y = 56;
      L[i].tile1 = 120;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = LIFE_BONUS;

      i = 57;
      L[i].x = 112;
      L[i].y = 64;
      L[i].tile1 = 138;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 58;
      L[i].x = 112;
      L[i].y = 72;
      L[i].tile1 = 156;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 59;
      L[i].x = 96;
      L[i].y = 80;
      L[i].tile1 = 172;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 60;
      L[i].x = 80;
      L[i].y = 80;
      L[i].tile1 = 170;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 61;
      L[i].x = 64;
      L[i].y = 80;
      L[i].tile1 = 168;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = LONG_R_BONUS;
}

void PropNivel5(void)
{
	u16 i;

      i = 0;
      L[i].x = 48;
      L[i].y = 8;
      L[i].tile1 = 4;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 1;
      L[i].x = 80;
      L[i].y = 8;
      L[i].tile1 = 8;
      L[i].destruye = TRUE;
      L[i].golpes = 3;
      L[i].BONUS = LIFE_BONUS;

      i = 2;
      L[i].x = 112;
      L[i].y = 8;
      L[i].tile1 = 12;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 3;
      L[i].x = 16;
      L[i].y = 16;
      L[i].tile1 = 18;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = COMET_BONUS;

      i = 4;
      L[i].x = 144;
      L[i].y = 16;
      L[i].tile1 = 34;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = BIGBALL_BONUS;

      i = 5;
      L[i].x = 32;
      L[i].y = 24;
      L[i].tile1 = 38;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 6;
      L[i].x = 80;
      L[i].y = 24;
      L[i].tile1 = 44;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 7;
      L[i].x = 128;
      L[i].y = 24;
      L[i].tile1 = 50;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 8;
      L[i].x = 48;
      L[i].y = 32;
      L[i].tile1 = 58;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 9;
      L[i].x = 112;
      L[i].y = 32;
      L[i].tile1 = 66;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 10;
      L[i].x = 32;
      L[i].y = 40;
      L[i].tile1 = 74;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = LONG_R_BONUS;

      i = 11;
      L[i].x = 64;
      L[i].y = 40;
      L[i].tile1 = 78;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 12;
      L[i].x = 80;
      L[i].y = 40;
      L[i].tile1 = 80;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 13;
      L[i].x = 96;
      L[i].y = 40;
      L[i].tile1 = 82;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 14;
      L[i].x = 128;
      L[i].y = 40;
      L[i].tile1 = 86;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = MESIAS_BONUS;

      i = 15;
      L[i].x = 48;
      L[i].y = 48;
      L[i].tile1 = 94;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 16;
      L[i].x = 80;
      L[i].y = 48;
      L[i].tile1 = 98;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 17;
      L[i].x = 112;
      L[i].y = 48;
      L[i].tile1 = 102;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 18;
      L[i].x = 16;
      L[i].y = 56;
      L[i].tile1 = 108;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 19;
      L[i].x = 64;
      L[i].y = 56;
      L[i].tile1 = 114;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 20;
      L[i].x = 80;
      L[i].y = 56;
      L[i].tile1 = 116;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 21;
      L[i].x = 96;
      L[i].y = 56;
      L[i].tile1 = 118;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 22;
      L[i].x = 80;
      L[i].y = 72;
      L[i].tile1 = 118;
      L[i].destruye = FALSE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 23;
      L[i].x = 144;
      L[i].y = 56;
      L[i].tile1 = 124;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 24;
      L[i].x = 16;
      L[i].y = 64;
      L[i].tile1 = 126;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 25;
      L[i].x = 144;
      L[i].y = 64;
      L[i].tile1 = 142;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = NO_BONUS;

      i = 26;
      L[i].x = 48;
      L[i].y = 72;
      L[i].tile1 = 148;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = COMET_BONUS;

      i = 27;
      L[i].x = 112;
      L[i].y = 72;
      L[i].tile1 = 156;
      L[i].destruye = TRUE;
      L[i].golpes = 0;
      L[i].BONUS = COMET_BONUS;

      i = 28;
      L[i].x = 32;
      L[i].y = 80;
      L[i].tile1 = 164;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 29;
      L[i].x = 64;
      L[i].y = 80;
      L[i].tile1 = 168;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 30;
      L[i].x = 96;
      L[i].y = 80;
      L[i].tile1 = 172;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

      i = 31;
      L[i].x = 128;
      L[i].y = 80;
      L[i].tile1 = 176;
      L[i].destruye = TRUE;
      L[i].golpes = 5;
      L[i].BONUS = NO_BONUS;

}

void do_mesias(void)
{
	u16 i;

	for(i=543; i<=560; i+=2) { bgMap[i]=mes1; bgMap[i+1]=mes2; }
}

void Refresca_blockMap(u16 nivel)
{
	u16 x,y;
	u8 *nivelMap;

	if(nivel==1) nivelMap = blockMap_1;
	if(nivel==2) nivelMap = blockMap_2;
	if(nivel==3) nivelMap = blockMap_3;
	if(nivel==4) nivelMap = blockMap_4;
	if(nivel==5) nivelMap = blockMap_5;

	for(y=0;y<18;y++)
	for(x=0;x<18;x++)
	bgMap[x+3+((y+1)*30)] = nivelMap[x+(y*18)];
}

void IniciaPropiedadesBloques(u16 nivel)
{
	u16 i,bonN;

	for(i=0 ; i<=162 ; i++)
		{
		L[i].x = 0;
		L[i].y = 0;
		L[i].xM = 0;
		L[i].yM = 0;
		L[i].tile1 = 0;
		L[i].tile2 = 0;
		L[i].destruye = FALSE;
		L[i].golpes = 0;
		L[i].BONUS = NO_BONUS;
		L[i].activoBonus = FALSE;
			L[i].bonX = 240+8;
			L[i].bonY = 160+8;
			L[i].bonN = 0;
		}

	if(nivel==1) PropNivel1();
	if(nivel==2) PropNivel2();
	if(nivel==3) PropNivel3();
	if(nivel==4) PropNivel4();
	if(nivel==5) PropNivel5();

	for(i=0, bonN=0 ; i<=162 ; i++)
		{
		L[i].xM = L[i].x + 16;
		L[i].yM = L[i].y + 8;
		L[i].tile2 = L[i].tile1 + 1;
		if(L[i].BONUS!=NO_BONUS)
			{
			L[i].bonX = L[i].x;
			L[i].bonY = L[i].y;
			L[i].bonN = bonN;
			bonN+=2;
			}
		}
}

void limpia_activoBonXY(void)
{
	u16 i;

	for(i=0 ; i<=162 ; i++)
		{
		if(L[i].BONUS!=NO_BONUS && L[i].activoBonus==TRUE)
			{
			L[i].bonX = 240;
			L[i].bonY = 160;
			}
		}
}

u16 ChecaTerminadoNivel(void)
{
	u16 i,j=0,r;

	for(i=0;i<=162;i++)
		{
		if(L[i].x != 0 && L[i].destruye == TRUE) { j++; }
		}

	if(j==0) { r = TRUE; }
	else if(j>0) { r = FALSE; }

	return r;
}

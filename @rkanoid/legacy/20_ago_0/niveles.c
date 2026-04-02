#define i1 45  // Iris 1
#define i2 46  // Iris 2
#define j1 47  // Rojo 1
#define j2 48  // Rojo 2
#define v1 49  // Verde 1
#define v2 50  // Verde 2
#define a1 51  // Amarillo 1
#define a2 52  // Amarillo 2
#define z1 53  // Azul 1
#define z2 54  // Azul 2
#define c1 55  // Cafe 1
#define c2 56  // Cafe 2
#define g1 57  // Gris 1
#define g2 58  // Gris 2
#define m1 59  // Morado 1
#define m2 60  // Morado 2
#define r1 61  // Rosa 1
#define r2 62  // Rosa 2
#define s1 63  // Smile 1
#define s2 64  // Smile 2
#define t1 65  // Titanium 1
#define t2 66  // Titanium 2
#define N1 14  // Nada 1
#define N2 15  // Nada 2
#define O1 12  // Presionado 1
#define O2 13  // Presionado 2
#define mes 16 // Mesías
#define mes1 16 // Mesías
#define mes2 17 // Mesías

#define NO_BONUS 666
#define COMET_BONUS 0
#define LONG_R_BONUS 1
#define LIFE_BONUS 2
#define BIGBALL_BONUS 3
#define MESIAS_BONUS 4

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
1,0,22,	29,29,	29,29,	29,29,	29,29,	29,29,	29,29,	29,29,	29,29,	29,29,	23,1,0,1,0,1,0,1,0,

1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,2,3,4,5,6,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,20,21,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,7,8,9,10,11,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,
1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,

1,0,26,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	N1,N2,	27,1,0,1,0,1,0,1,0,1

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
	u16 i,bonN, v, h;
	u8 *nivelMap;

	for(i=0 ; i<=162 ; i++) {
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

	if(nivel==1) { nivelMap = blockMap_1; }
	if(nivel==2) { nivelMap = blockMap_2; }
	if(nivel==3) { nivelMap = blockMap_3; }
	if(nivel==4) { nivelMap = blockMap_4; }
	if(nivel==5) { nivelMap = blockMap_5; }

	i=0;
	for(v=0;v<18*18;v+=18) {
		for(h=0;h<18;h+=2) {
			if(nivelMap[v+h]!=N1) { L[i].tile1=v+h; L[i].tile2=v+h+1; }
			i++;
		}
	}

	i=0;
	for (v=1;v<19;v++) {
		   for (h=1;h<10;h++) {
			if(nivelMap[L[i].tile1]!=N1 && nivelMap[L[i].tile1]!=N2) { L[i].x=h*16; L[i].y=v*8; L[i].xM = L[i].x + 16; L[i].yM = L[i].y + 8; }
			i++;
		   }
		}

	for(i=0, bonN=0 ; i<=162 ; i++) {
		L[i].golpes = 0;
						L[i].destruye = TRUE;
		if(L[i].BONUS!=NO_BONUS) {
			L[i].bonX = L[i].x+4;
			L[i].bonY = L[i].y;
			bonN++;
			L[i].bonN = bonN;
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

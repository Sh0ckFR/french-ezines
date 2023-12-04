#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/*
 * 0x804821a -> chaine sousmise
 * 0x804816f -> chaine output 
 * 0x80480af -> instructions cryptees		(zone)
 * 0x8048288 -> buffer TRASH de 260 octets	(data1)
 * 
 */


unsigned char data1[256];
unsigned char zone[0x20];

unsigned int eax = 0;
unsigned int ebx = 0;
unsigned int ecx = 0;
unsigned int edx = 0;
unsigned int edi = 0;
unsigned int esi = 0;

unsigned char *p_al = (unsigned char*)&eax;
unsigned char *p_ah = (unsigned char*)(&eax + 1);
unsigned char *p_bl = (unsigned char*)&ebx;
unsigned char *p_cl = (unsigned char*)&ecx;
unsigned char *p_dl = (unsigned char*)&edx;
unsigned char *p_dh = (unsigned char*)(&edx + 1);

static __inline__
void dummy2(char *chaine)
{

	unsigned char *p = data1;
	unsigned char temp;
	
	eax = 0xfffefdfc;
	ecx = 0x40;

	// boucle1
	
	while(1)
	{
		*((int *)(p-4+ecx*4)) = eax;
		eax -= 0x4040404;
		
		--ecx;
		if(!ecx)
			break;
	}

	eax = 0;

init:

	ebx = 0;
	esi = 4;

	goto debut;
	
	// boucle 2

	for(ecx=1; ecx < 256; ++ecx)
	{
		++ebx;
		--esi;
	
		if(!esi)
		{
			ebx = 0;
			esi = 4;
		}	
debut:	
		// 256 octets parcourus !
		
		*p_dl = *(p+ecx);
		*p_al += *(chaine + ebx) + *p_dl;
		*(p+ecx) = *(p+eax);
		*(p+eax) = *p_dl;
	} 

	return;
}

static __inline__
void dummy1(char *zone)
{
	// vars
	
	unsigned char *p = data1;
	int i = 0;
	unsigned char temp;
	
	// init
	
	eax = 0;
	ebx = 0;
	ecx = 0;
	edx = 0;

	edi = 0x11;
	
	while(edi)
	{
		++(*p_bl);
		temp = *(p+ebx);
		*p_al += temp;
		*p_cl = *(p+eax);
		*(p+ebx) = *p_cl;
		*(p+eax) = temp;
		*p_cl += temp;
		
		zone[i] ^= *(p+ecx);
		++i;
		--edi;
	}
	
	return;
}


int is_it_ok(char *chaine)
{
	unsigned int *ptr = NULL;//(int *)saisie;
	
	/* init du tableau */
	
	bzero(data1,sizeof(data1));
	ptr = (unsigned int *)data1+4;
	*ptr = 0xa8eb9aeb;
	
	/* Init de zone */

	ptr = (unsigned int *)zone;
	*ptr++ = 0x30f36254;
	*ptr++ = 0x5a3b9164;
	*ptr++ = 0xe1a9471f;
	*ptr++ = 0x541d0209;
	*ptr++ = 0x40c03191; 
	*ptr++ = 0x80cddb31;
  	*ptr++ = 0xfefdfcb8;
  	*ptr++ = 0x0040b9ff;
	*ptr++ = 0x04890000;
  	*ptr++ = 0x0482888d;
  	*ptr++ = 0x04042d08;
  	*ptr++ = 0x75490404;
	*ptr++ = 0x31c031f1;
  	*ptr++ = 0x0004bedb;
  	*ptr++ = 0x05e90000;
  	*ptr++ = 0xfe000000;
	*ptr++ = 0xef744ec3;
  	*ptr++ = 0x828c918a;
  	*ptr++ = 0x04020804;
  	*ptr++ = 0x8ad0001f;
	*ptr++ = 0x04828cb0;
  	*ptr++ = 0x8cb18808;
  	*ptr++ = 0x88080482;
  	*ptr++ = 0x04828c90;
	*ptr++ = 0x75c1fe08;
  	*ptr++ = 0xff85c3da;
  	*ptr++ = 0xc0314174;
  	*ptr++ = 0xc931db31;
	*ptr++ = 0xc3fed231;
  	*ptr++ = 0x828c938a;
  	*ptr++ = 0xd0000804;
  	*ptr++ = 0x828c888a;
	*ptr++ = 0x8b880804;
  	*ptr++ = 0x0804828c;
  	*ptr++ = 0x828c9088;
  	*ptr++ = 0xd1000804;
	*ptr++ = 0x828c898a;
  	*ptr++ = 0x0e300804;
  	*ptr++ = 0xd6754f46;
  	*ptr++ = 0x8cbfc031;
	*ptr++ = 0xb9080482;
  	*ptr++ = 0x00000040;
  	*ptr++ = 0xc3abf3fc;
  	*ptr++ = 0x04b0c031;
	*ptr++ = 0xcd43db31;
  	*ptr++ = 0xc031c380;
  	*ptr++ = 0xdb3103b0;
  	*ptr++ = 0xc380cd43;
	*ptr++ = 0x2d09090a;
  	*ptr++ = 0x3d3d2d2d;
  	*ptr++ = 0x706d6520;
  	*ptr++ = 0x72632027;
	*ptr++ = 0x6d6b6361;
  	*ptr++ = 0x20332065;
  	*ptr++ = 0x2d2d3d3d;
  	*ptr++ = 0x09090a2d;
	*ptr++ = 0x2d2d2d2d;
  	*ptr++ = 0x2d2d2d2d;
  	*ptr++ = 0x2d2d2d2d;
  	*ptr++ = 0x2d2d2d2d;
	*ptr++ = 0x2d2d2d2d;
  	*ptr++ = 0x2d2d2d2d;
  	*ptr++ = 0x0a0a2d2d;
  	*ptr   = 0x70747468;
	
	/* Ok c tipar */

	eax = 7;
	esi = 7;
	dummy2(chaine);
	
	edi = 0x11;
	dummy1(zone);
	
	ptr = (unsigned int *)zone;
	if(*ptr == 0xbb99090)
		return 1;
	else
		return 0;
	
}

static __inline__
int gimme_next_val(char *string)
{
	int i=0;
	unsigned int *val = (unsigned int *)string;
	
#define IS_NOT_NUM(x)			((x >= 48 && x <= 57) ? 0 : 1)
#define IS_NOT_CHAR_MAJ(x)		((x >= 65 && x <= 90) ? 0 : 1)
#define IS_NOT_CHAR_MIN(x)		((x >= 97 && x <= 122) ? 0 : 1)
	
	for(i=0;i<4;i++)
	{	
		if( IS_NOT_NUM(string[i]) && IS_NOT_CHAR_MIN(string[i])
		&& IS_NOT_CHAR_MAJ(string[i]))
			return 0;
	}
	
	//printf("NEXT : %.8x\n",*val);
	return 1;
}

int main(int argc, char **argv)
{
	
	
	unsigned int val = 0x30303031;	// 0x0x804821a
	unsigned char *p_chaine = (unsigned char*)&val;
	
	while(!is_it_ok(p_chaine))
	{
		
		++val;

		while(!(gimme_next_val(p_chaine)))
			++val;

		if(val == 0x30303030)
		{
			printf("Erreur : impossible de trouver une soluce T_T\n");
			exit(EXIT_FAILURE);
		}
		
		
	}
	
	printf("soluce : %c%c%c%c (0x%.8x)\n",p_chaine[0],p_chaine[1],
			p_chaine[2],p_chaine[3],val);
	
	return 0;
}

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

#ifdef SHOW_PERF
unsigned int compteur_test = 0;
#endif

unsigned char data1[256];
unsigned char zone[256];
unsigned char key[5];

int is_it_ok()
{
	unsigned int *ptr = NULL;
		
	/* init du tableau */
	
	bzero(data1,sizeof(data1));
	
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
	
	__asm__("movl $%0,%%edi\n"
		"call sub_dummy2\n"
		:"=m" (key));
	
	/* 4 -> edi suffit */
	
	__asm__("movl $0x4, %%edi\n"	
		"movl $%0, %%esi\n"
		"call sub_dummy1\n"
		:"=m" (zone));

#ifdef SHOW_PERF
	compteur_test++;
#endif
	
	ptr = (unsigned int *)zone;
	
	if(*ptr == 0xbb99090)
		return 1;
	else
		return 0;
	
}

static __inline__ int gimme_next_val(void)
{
	int i=0;
	unsigned int *val = (unsigned int *)key;
	
#define IS_NOT_NUM(x)			((x >= 48 && x <= 57) ? 0 : 1)
#define IS_NOT_CHAR_MAJ(x)		((x >= 65 && x <= 90) ? 0 : 1)
#define IS_NOT_CHAR_MIN(x)		((x >= 97 && x <= 122) ? 0 : 1)
	
	for(i=0;i<4;i++)
	{	
		if( IS_NOT_NUM(key[i]) && IS_NOT_CHAR_MIN(key[i])
		&& IS_NOT_CHAR_MAJ(key[i]))
			return 0;
	}

#ifdef want_to_see
	printf("NEXT : %s\n ",val);//%.8x\n",*val);
#endif
	return 1;
}


int main(int argc, char **argv)
{
	unsigned int *val = (unsigned int *)key;
	*val = 0x30303031;

	while(!is_it_ok())
	{
		
		++(*val);

		while(!(gimme_next_val()))
			++(*val);

		if(*val == 0x30303030)
		{
			printf("Erreur : impossible de trouver une soluce T_T\n");
			exit(EXIT_FAILURE);
		}
		
		
	}
	
	key[4] = 0;
	printf("Soluce : %s (%.8x)\n",key,*val);

#ifdef SHOW_PERF
	printf("%lu solutions testees ...\n",compteur_test);
#endif
	return 0;
}

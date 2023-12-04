/*
Programme de Lecture de carte à puce ISO by obscurer
Requiert le super montage d'obscurer pour fonctionner ! hé ouais :)
Pour plus de simplicité je defini des valeur pour RST R/W etc...
Comme ça pour savoir si RST est à 1 on a qu'a faire un "AND" :
			a=getvalue();
			if(a&RST==RST) { RST est à 1 kewl :) }
			else { RST est po à 1 :( }
Allez, voila le prog
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define	VCC		1	// = 00000001 en binaire
#define CLK		4	// = 00000100 en binaire
#define RST		8	// = 00001000 en binaire
#define IO		16	// = 00010000 en binaire

unsigned short getvalue()
{
	unsigned short ret;	

	_asm {
		mov dx,379h
		in ax,dx
		mov ret,ax
	}

	return ret;		// retourne betement la valeur de 0x379
}

void setvalue(unsigned short value)
{
	_asm {
		   mov dx,378h
           mov ax,value
           out dx,ax
	}
}

int main()
{
	unsigned int index;		// un petit compteur
	unsigned short getval;
	unsigned char buffer[256];
	FILE *output;

	printf("Presser une touche pour commencer à lire la carte\n");
	getch();
	
	setvalue(0);	// met toute les broches sortant du PC à 0
	
	setvalue(VCC+CLK);	// initialise
	setvalue(VCC+0);	// le dialogue

	setvalue(VCC+RST);  // on met reset à 1 pour toute la durée du programme

	for(index=0;index<256;index++)
	{
	setvalue(VCC+RST+CLK);	// RST toujours à 1, CLK à 1
	setvalue(VCC+RST);		// RST toujours à 1, CLK à 0 
						// = incrémente le compteur de la carte
						// donc elle va retourner une valeur sur I/O
	getval=getvalue();  // et heureusement on est la pour la chopper
	getval&=IO;			// on AND getval et IO pour voir si I/O est à 1
	
	if(getval==IO) { printf("1"); buffer[index]='1'; }
	else { printf("0"); buffer[index]='0'; }	// no comment

	if(index%8==0) { printf(" "); }  // ajoute un espace tout les 8
	if(index%32==0) { printf("\n");} // va à la ligne tout les 32

	}


	printf("Presser une touche pour quitter\n");
	getch();

	output=fopen("carte.txt","w");	// Balance le buffer dans le fichier
	fputs(buffer,output);
	fclose(output);

	exit(0);
}
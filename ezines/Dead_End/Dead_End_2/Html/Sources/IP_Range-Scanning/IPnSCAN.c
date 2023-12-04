/******************************************************************************
 *                             IPnSCAN.c v1.0                                 *
 * ---------------IP v4-6 Range Scanner & Reverse DNS Tool--------------------*
 ******************************************************************************
 *----------------------------------------------------------------------------*
 *                        By Andras & Skyrunner                               *
 *----------------------------------------------------------------------------*
 ******************************************************************************
 *    This program is distributed in the hope that it will be useful, but     *
 *    WITHOUT ANY WARRANTY; the authors could not be held in charge for the   *
 *    use which will be made by it. Have Fun!                                 *
 ******************************************************************************/

#include <stdio.h>   /*printf(), scanf()...*/
#include <string.h>  /*strlen(), strcmp()...*/
#include <stdlib.h>  /*malloc(), exit()...*/

/*----- Définitions des constantes pour le réglage de la vitesse du scan -----*/
#define HIGH_SPEED 100
#define MEDIUM_SPEED 500
#define LOW_SPEED 1000
/*----------------------------------------------------------------------------*/

/*------ Définitions des constantes relatives à la syntaxe des requêtes ------*/
#define V4_REQUEST "ping -n 1 -w"
#define V6_REQUEST "ping6 -n 1 -w"
#define V4_LOOKUP "ping -a"
#define V6_LOOKUP "ping6 -a"
/*----------------------------------------------------------------------------*/


/*------------ Taille définit pour les allocations mémoires ------------------*/
#define BUFFER 80
/*----------------------------------------------------------------------------*/


/*--------------------- Constantes pour appel système ------------------------*/
#define SCREEN_CL "cls"    /*effacer les résultats du scan précédent à l'écran*/
#define ERASE "del log.txt"/*effacer les logs du scan*/
/*----------------------------------------------------------------------------*/

/*----------------------------- Autres ---------------------------------------*/
#define SUCCESS 0
#define ERROR 1
/*----------------------------------------------------------------------------*/


/*------Définitions des structures relatives à la version des adresses IP-----*/

/*------------------------*/
/* Structure IP version 4 */
/*------------------------*/

typedef struct
{
	unsigned int d1;
	unsigned int d2;
	unsigned int d3;
	unsigned int d4;
	unsigned int f1;
	unsigned int f2;
	unsigned int f3;
	unsigned int f4;
}adress_v4;
/*------------------------*/


/*------------------------*/
/* Structure IP version 6 */
/*------------------------*/
typedef struct
{
	unsigned int g1;
	unsigned int g2;
	unsigned int g3;
	unsigned int g4;
	unsigned int g5;
	unsigned int g6;
	unsigned int g7;
	unsigned int g8;
	unsigned int h1;
	unsigned int h2;
	unsigned int h3;
	unsigned int h4;
	unsigned int h5;
	unsigned int h6;
	unsigned int h7;
	unsigned int h8;
}adress_v6;
/*------------------------*/
/*----------------------------------------------------------------------------*/



/*---------------- Structure relative à la liste chainnée --------------------*/
typedef struct eltstruct
{
	char *target;
	struct eltstruct *next;
}maillon, *list;
/*----------------------------------------------------------------------------*/



/*------------- Variables globales nécessaires au programme ------------------*/
unsigned long compteurip=0;   /*comptabiliser le nombre d'IP actives détectées*/
list l=NULL;      /*liste chainnée pour stockée le(s) IP active(s) détectée(s)*/
maillon *m;       /*maillon de la liste*/
/*----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
  PROTOTYPES DES FONCTIONS
  ----------------------------------------------------------------------------*/

void demande(void);

void saisiev4 (void);

void saisiev6 (void);

int decouper(char adresse1[], char adresse2[]);

int ping4 (char adreschoix[], int timeout);

int ping6 (char adreschoix[], int timeout);

int verif_syntaxv4(adress_v4 adresse1, adress_v4 adresse2);

int verif_syntaxv6(adress_v6 adresse1, adress_v6 adresse2);

void calcul_adressv4(adress_v4 adresse, int timeout);

void calcul_adressv6(adress_v6 adresse, int timeout);

void incrementev4(adress_v4 adresse1, adress_v4 adresse2, int timeout);

void incrementev6(adress_v6 adresse, adress_v6 adresse2, int timeout);

list insert_tailing(char *adresse, list l);

void lookupv4(void);

void lookupv6(void);

void namehotechoix(void);

void namehotev4(char *adresse);

void namehotev6(char *adresse);

void credit(void);

void usage(void);
/*----------------------------------------------------------------------------*/


void demande(void)
{
	int choix;
	list aux;

	compteurip=0;  /* on initialise compteurip à 0*/
	system(SCREEN_CL);/*on efface les opérations précédente affichées à l'écran*/

	/* libération de l'espace mémoire occupée par la liste chainnée contenant les
	   ip actives */
	while(l != NULL)
	{
		aux=l;
		l=l->next;
		free(aux);
	}

	/*---------------------------------Menu--------------------------------------*/
	printf("-------------------------------[IPnSCAN v1.0]\n\n");
	printf("[IPnSCAN -h pour la version en lignes de commandes]\n\n\n"); 
	printf("----[En attente instructions...\n\n");

	printf("--------[1 Scanner une plage d'adresse IP version 4\n");
	printf("--------[2 Scanner une plage d'adresse IP version 6\n");
	printf("--------[3 Effectuer un Reverse DNS\n");
	printf("--------[4 Quitter le programme\n");
	printf("\nChoix : ");
	scanf("%d%*c",&choix);
	printf("\n°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
	/*---------------------------------------------------------------------------*/
	switch(choix)
	{
		case 1 : saisiev4(); break;  /*si choix=2 on appelle saisiev4*/
		case 2 : saisiev6(); break;  /*si choix=3 on appelle saisiev6*/
		case 3 : namehotechoix(); break;  /*si choix=4 on apelle namehote*/
		default: exit(SUCCESS);  /*si choix autre que 1,2,3 ou 4 alors exit*/
	}
}



void saisiev4 (void)
{
	adress_v4 adresse1, adresse2;
	int choixmd;
	unsigned int speed;
	char choix, ip[20];
	
	printf("\nEntrez l'adresse ip correspondant au debut de la plage a scanner :" 
	       "\n\n------------------------------["); /* 1ere adresse */
	scanf("%d.%d.%d.%d%*c",&adresse1.d1,&adresse1.d2,&adresse1.d3,&adresse1.d4);

	printf("\nEntrez l'adresse ip correspondant a la fin de la plage a scanner :" 
	       "\n\n------------------------------["); /* 2eme adresse */
	scanf("%d.%d.%d.%d%*c",&adresse2.f1,&adresse2.f2,&adresse2.f3,&adresse2.f4);

	switch(verif_syntaxv4(adresse1,adresse2))
	{
		case -1 : printf("\n\n----[L'adresse de debut a une syntaxe incorrect !"
				         "\n     Chaque partie de l'adresse doit etre comprise entre 0 et 255."
				         "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
			      scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
				    saisiev4();
			      else
				    exit(SUCCESS);
	
		case -2 : printf("\n\n----[L'adresse de fin a une syntaxe incorrect !"
			             "\n     Chaque partie de l'adresse doit etre comprise entre 0 et 255."
			             "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
			      scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
				    saisiev4();
			      else
				    exit(SUCCESS);
	
		case -3 : printf("\n\n----[L'adresse de debut est superieure a l'adresse de fin !"
				         "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
                  scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
                    saisiev4();
			      else
			        exit(SUCCESS);
			  
        default : break;
	}

	/*------------------------------Modes------------------------------------*/
	printf("\n\n----[Quel mode voulez-vous utiliser pour le scan ?");
	printf("\n\n--------[1 Mode Sherlock (vitesse lente - haute precision)");
	printf("\n--------[2 Mode Normal (vitesse normale - precision normale)");
	printf("\n--------[3 Mode Burn (vitesse rapide - basse precision)\n");
	printf("\nChoix : ");
	scanf("%d%*c", &choixmd);
	/*-----------------------------------------------------------------------*/
	switch(choixmd)
	{
		case 1 : speed=LOW_SPEED;
			     printf("\n\n----[Mode Sherlock active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
			     break;
		
		case 2 : speed=MEDIUM_SPEED;
			     printf("\n\n----[Mode Normal active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
			     break;
		
		case 3 : speed=HIGH_SPEED;
			     printf("\n\n----[Mode Burn active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
			     break;
		
		default : printf("\n\n----[Erreur, aucun mode associe a ce choix !"
					     "\n     Appuyez sur ENTREE pour retourner au menu...");
			      getchar();
			      return;
	}

  /*on transmet à calcul_adress() les 2 adresses éclatées*/
	calcul_adressv4(adresse1, speed);

  /*on incremente l'adresse pour scanner tte la plage*/
	incrementev4(adresse1, adresse2, speed);

  /*si le nbre d'ip actives est nulle (une fois que toutes les ft st terminées)*/
	if (!compteurip)
		printf("\n\nScan termine !"
				"\nAucune IP active dans la tranche scannee !\n");
	else  /*sinon*/
        printf("\n\nEffectuer un reverse DNS sur une adresse ? (O/N) : ");
	    scanf("%c%*c", &choix);
	    while (choix=='o' || choix=='O')
	    {
           printf("\n\nEntrez l'adresse : ");
           scanf("%s%*c",ip);
		   namehotev4(ip);
		   printf("\n\nEffectuer un reverse DNS sur une autre adresse ? (O/N) : ");
		   scanf("%c%*c", &choix);
	    }
		printf("\n\n----[Scan termine !");

	printf("\n\nAppuyez sur ENTREE pour retourner au menu...");
	getchar();
	demande();  /*on retourne au menu*/
}




void saisiev6 (void)
{
	adress_v6 adresse1, adresse2;
	int choixmd;
	unsigned int speed;
	char *install="ipv6 install", choix, ip[45];

	printf("\nEntrez l'adresse ip correspondant au debut de la plage a scanner : \n\n--------------------[");
	scanf("%x:%x:%x:%x:%x:%x:%x:%x%*c",&adresse1.g1, &adresse1.g2, &adresse1.g3, &adresse1.g4, &adresse1.g5, &adresse1.g6, &adresse1.g7, &adresse1.g8);

	printf("\nEntrez l'adresse ip correspondant au debut de la plage a scanner : \n\n--------------------[");
	scanf("%x:%x:%x:%x:%x:%x:%x:%x%*c",&adresse2.h1, &adresse2.h2, &adresse2.h3, &adresse2.h4, &adresse2.h5, &adresse2.h6, &adresse2.h7, &adresse2.h8);

	switch(verif_syntaxv6(adresse1,adresse2))
	{
		case -1 : printf("\n\n----[L'adresse de debut a une syntaxe incorrect !"
				         "\n     Chaque partie de l'adresse doit etre comprise entre 0 et FFFF."
				         "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
			      scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
				    saisiev6();
			      else
				    exit(SUCCESS);
	
		case -2 : printf("\n\n----[L'adresse de fin a une syntaxe incorrect !"
			             "\n     Chaque partie de l'adresse doit etre comprise entre 0 et FFFF."
			             "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
                  scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
				    saisiev6();
			      else
				    exit(SUCCESS);
	
		case -3 : printf("\n\n----[L'adresse de debut est superieure a l'adresse de fin !"
				         "\n     Voulez-vous entrer une nouvelle plage d'adresse ? (O/N) : ");
			      scanf("%c%*c", &choix);
			      if (choix == 'o' || choix == 'O')
				    saisiev6();
			      else
				    exit(SUCCESS);
			  
		default : break;
	}

	printf("\n\n----[Quel mode voulez-vous utiliser pour le scan ?");
	printf("\n\n--------[1 Mode Sherlock (vitesse lente - haute precision)");
	printf("\n--------[2 Mode Normal (vitesse normale - precision normale)");
	printf("\n--------[3 Mode Burn (vitesse rapide - basse precision)\n");
	printf("\nChoix : ");
	scanf("%d%*c", &choixmd);

	switch(choixmd)
	{
		case 1 : speed=LOW_SPEED;
			     printf("\n\n----[Mode Sherlock active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
		         break;
		
		case 2 : speed=MEDIUM_SPEED;
			     printf("\n\n----[Mode Normal active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
		         break;
		
		case 3 : speed=HIGH_SPEED;
			     printf("\n\n----[Mode Burn active !\n"
					    "\nDEMARRAGE DU SCAN...\n");
			     break;
		
		default : printf("\n\n----[Erreur, aucun mode associe a ce choix !"
					     "\n----[Appuyez sur ENTREE pour retourner au menu...");
			      getchar();
			      return;
	}

	/*-On fait un test pour déterminer si le pilote IPv6 est installé sur le système-*/
	if(system("ipv6 if>nul")!=0)
	{
		fprintf(stderr,"\n\n----[Le pilote IPv6 n'est pas installe sur votre systeme !"
				       "\n     L'installation va debuter dans quelques secondes...\n");
		system(install);  /*on installe les pilotes*/
		printf("\n\n----[Reprise du scan...\n");
	}
	/*-------------------------------------------------------------------------------*/

	calcul_adressv6(adresse1, speed);

	incrementev6(adresse1, adresse2, speed);

	if (!compteurip)
		printf("\n\nScan termine !"
				"\nAucune IP active dans la tranche scannee !\n");
	else
        printf("\n\nEffectuer un reverse DNS sur une adresse ? (O/N) : ");
	    scanf("%c%*c", &choix);
	    while (choix=='o' || choix=='O')
	    {
           printf("\n\nEntrez l'adresse : ");
           scanf("%s%*c",ip);
		   namehotev6(ip);
		   printf("\n\nEffectuer un reverse DNS sur une autre adresse ? (O/N) : ");
		   scanf("%c%*c", &choix);
	    }
		printf("\n\n----[Scan termine !");

	printf("\n\nAppuyez sur ENTREE pour retourner au menu...");
	getchar();
	demande();
}





int ping4 (char *adreschoix, int timeout)
{
	char *req_aux, *req, *mute=">nul";
	int res;

	req_aux=V4_REQUEST; /*req="ping -n 1 -w "*/

  /*réservation de l'espace mémoire nécessaire pour la requête*/
	if((req=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du scan."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		exit(SUCCESS);
	}

  /*concaténation de la requête*/
	sprintf(req,"%s %d %s%s", req_aux, timeout, adreschoix, mute);/*req="ping -n 1 -w [TIMEOUT] [ADRESSE]"*/

	res=system(req);  /* res recoit le code de retour de l'appel système avec la requete :
			             0 si l'adresse ip est active, 1 sinon */
	free(req);
	return res;   /* on retourne le code de retour de l'appel système*/
}





int ping6 (char *adreschoix, int timeout)/* idem à ping4() à l'exception de la requête */
{
	char *req_aux, *req, *mute=">nul";
	int res;

	req_aux=V6_REQUEST;

	if((req=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du scan."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}

	sprintf(req,"%s %d %s%s", req_aux, timeout, adreschoix, mute);

	res=system(req);

	free(req);
	return res;
}




void calcul_adressv4(adress_v4 adresse, int timeout)
{
	char *adress, classe;
	int res;

  /*réservation de l'espace mémoire pour stocker l'adresse*/
	if((adress=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du scan."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}

  /*concaténation de l'adresse IP*/
	sprintf(adress,"%d.%d.%d.%d",adresse.d1,adresse.d2,adresse.d3,adresse.d4);

  /*on appelle la ft ping a laquelle on envoi l'adresse ip reconsituée avec les 2 fragements*/
	res=ping4(adress, timeout) ;
	if (!res)   /*si res=0 i.e si l'ip est active*/
	{
        if(adresse.d1 <= 127)
            classe='A';
        else if(adresse.d1 >= 128 && adresse.d1 <= 191)
                  classe='B';
        else if(adresse.d1 >=192 && adresse.d1<= 223)
                  classe='C';
        else classe='N';

        if(classe != 'N')
		    printf("\n\a[ %s ] est active et appartient a un reseau de classe [ %c ]", adress, classe);
        else
             printf("\n\a[ %s ] est active et appartient a un reseau de classe [ inconnue ]", adress);
        /*réservation de l'espace mémoire pour un maillon afin de stocker l'adresse */
		if((m=(maillon *)malloc(sizeof(maillon)))==NULL)
		{
			fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
					       "\n   Abandon du scan."
					       "\n\n   Appuyez sur ENTREE pour quitter");
			getchar();
			exit(SUCCESS);
		}
		m->target=adress;            /*on stocke l'adresse détéctée dans le maillon précédemment créé*/
		l=insert_tailing(adress, l); /*on insère le maillon dans la liste chainnée*/
		compteurip++;                 /*1 IP de plus détectée*/
	}
	else                                        /*si res different de 0...*/
		printf("\n[ %s ] est inactive", adress);
}




void calcul_adressv6(adress_v6 adresse, int timeout)
{
	char *adress;
	int res;

	if((adress=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du scan."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}
	sprintf(adress,"%x:%x:%x:%x:%x:%x:%x:%x",adresse.g1, adresse.g2, adresse.g3, adresse.g4, adresse.g5, adresse.g6, adresse.g7, adresse.g8);

	res=ping6(adress, timeout);

	if (!res)
	{
		printf("\n\a[ %s ] est active", adress);
		if((m=(maillon *)malloc(sizeof(maillon)))==NULL)
		{
			fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
					       "\n   Abandon du scan."
					       "\n\n   Appuyez sur ENTREE pour quitter");
			getchar();
			exit(SUCCESS);
		}
		m->target=adress;
		l=insert_tailing(adress, l);
		compteurip++;
	}
	else
		printf("\n[ %s ] est inactive", adress);
}





void incrementev4(adress_v4 adresse1, adress_v4 adresse2, int timeout)
{
  /*tant que l'ip de début et l'ip de fin ne st pas egalles*/
	while (adresse1.d1!=adresse2.f1 || adresse1.d2!=adresse2.f2 ||
           adresse1.d3!=adresse2.f3 || adresse1.d4!=adresse2.f4)
	{
      /*ip du type 1.1.1.2 qui devient 1.1.1.3*/
		if (adresse1.d4!=255)
		{
			adresse1.d4++;
			calcul_adressv4(adresse1, timeout);
		}

		if (adresse2.f4 == adresse1.d4 && adresse1.d1 == adresse2.f1 &&
            adresse1.d2 == adresse2.f2 && adresse1.d3 == adresse2.f3)
		{
            lookupv4();
            return;
        }


        /*ip du type 1.1.1.255 qui devient 1.1.2.0*/
		if (adresse1.d4==255 && adresse1.d3!=255)
		{
			adresse1.d3++;
			adresse1.d4=0;
			calcul_adressv4(adresse1, timeout);
		}



		if (adresse2.f4 == adresse1.d4 && adresse1.d1 == adresse2.f1 &&
            adresse1.d2 == adresse2.f2 && adresse1.d3 == adresse2.f3)
		{
            lookupv4();
            return;
        }


      /*ip du type 1.1.255.255 qui devient 1.2.0.0*/
		if (adresse1.d3==255 && adresse1.d4==255 && adresse1.d2!=255)
		{
			adresse1.d2++;
			adresse1.d3=0;
			adresse1.d4=0;
			calcul_adressv4(adresse1, timeout);
		}

        if (adresse2.f4 == adresse1.d4 && adresse1.d1 == adresse2.f1 &&
            adresse1.d2 == adresse2.f2 && adresse1.d3 == adresse2.f3)
		{
            lookupv4();
            return;
        }


      /*ip du type 1.255.255.255 qui devient 2.0.0.0*/
		if (adresse1.d2==255 && adresse1.d3==255 && adresse1.d4==255 && adresse1.d1!=255)
		{
			adresse1.d1++;
			adresse1.d2=0;
			adresse1.d3=0;
			adresse1.d4=0;
			calcul_adressv4(adresse1, timeout);
		}
	}
	lookupv4(); /*on propose le reverse DNS*/
}




void incrementev6( adress_v6 adresse1, adress_v6 adresse2, int timeout)
{
 /*tant que l'ip de début et l'ip de fin ne st pas egalles */
	while (adresse1.g1!=adresse2.h1 || adresse1.g2!=adresse2.h2 ||
           adresse1.g3!=adresse2.h3 || adresse1.g4!=adresse2.h4 ||
           adresse1.g5!=adresse2.h5 || adresse1.g6!=adresse2.h6 ||
           adresse1.g7!=adresse2.h7 || adresse1.g8!=adresse2.h8)
	{
      /*ip du type  1:1:1:1:1:1:1:1    1:1:1:1:1:1:1:2*/
		if (adresse1.g8!=65535)
		{
			adresse1.g8++;
			calcul_adressv6(adresse1, timeout);
		}


      /*si la huitieme partie de l'adresse de fin egale FFFFF alors on appelle lookupv6*/
		if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }



        /*ip du type  1:1:1:1:1:1:1:FFFFF*/
		if (adresse1.g8==65535 && adresse1.g7!=65535)
		{
			adresse1.g7++;
			adresse1.g8=0;
			calcul_adressv6(adresse1, timeout);
		}


        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }


		 /*ip du type  1:1:1:1:1:1:FFFFF:FFFFF*/
		if (adresse1.g7==65535 && adresse1.g8==65535 && adresse1.g6!=65535)
  		{
			adresse1.g2++;
			adresse1.g3=0;
			adresse1.g4=0;
			calcul_adressv6(adresse1, timeout);
		}



        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }



        /*ip du type  1:1:1:1:1:FFFFF:FFFFF:FFFFF*/
		if (adresse1.g6==65535 && adresse1.g7==65535 && adresse1.g8==65535 &&
            adresse1.g5!=65535)
		{
			adresse1.g1++;
			adresse1.g2=0;
			adresse1.g3=0;
			adresse1.g4=0;
			calcul_adressv6(adresse1, timeout);
		}

        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }


      /*ip du type  1:1:1:1:FFFFFF:FFFFFF:FFFFF:FFFFF*/
		if (adresse1.g5==65535 && adresse1.g6==65535 && adresse1.g7==65535 &&
            adresse1.g8==65535 && adresse1.g4!=65535)
		{
			adresse1.g4++;
			adresse1.g5=0;
			adresse1.g6=0;
			adresse1.g7=0;
			adresse1.g8=0;
			calcul_adressv6(adresse1, timeout);
		}



        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }

      /*ip du type  1:1:1:FFFFFF:FFFFFF:FFFFFF:FFFFF:FFFFF*/
		if (adresse1.g5==65535 && adresse1.g6==65535 && adresse1.g7==65535 &&
            adresse1.g8==65535 && adresse1.g4==65535 && adresse1.g3!=65535)
		{
			adresse1.g3++;
			adresse1.g4=0;
			adresse1.g5=0;
			adresse1.g6=0;
			adresse1.g7=0;
			adresse1.g8=0;
			calcul_adressv6(adresse1, timeout);
		}


        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }

      /*ip du type  1:1:FFFFFF:FFFFFF:FFFFFF:FFFFFF:FFFFF:FFFFF*/
		if (adresse1.g5==65535 && adresse1.g6==65535 && adresse1.g7==65535 &&
            adresse1.g8==65535 && adresse1.g4==65535 && adresse1.g3==65535 &&
            adresse1.g2!=65535)
		{
			adresse1.g2++;
			adresse1.g3=0;
			adresse1.g4=0;
			adresse1.g5=0;
			adresse1.g6=0;
			adresse1.g7=0;
			adresse1.g8=0;
			calcul_adressv6(adresse1, timeout);
		}


        if (adresse1.g8 == adresse2.h8 && adresse1.g7 == adresse2.h7 &&
            adresse1.g6 == adresse2.h6 && adresse1.g5 == adresse2.h5 &&
            adresse1.g4 == adresse2.h4 && adresse2.h3 == adresse1.g3 &&
            adresse1.g2 == adresse2.h2 && adresse1.g1 == adresse2.h1)
        {
  		    lookupv6();
            return;
        }

      /*ip du type  1:FFFFFF:FFFFFF:FFFFFF:FFFFFF:FFFFFF:FFFFF:FFFFF*/
		if (adresse1.g5==65535 && adresse1.g6==65535 && adresse1.g7==65535 &&
            adresse1.g8==65535 && adresse1.g4==65535 && adresse1.g3==65535 &&
            adresse1.g2==65535 && adresse1.g1!=65535)
		{
			adresse1.g1++;
			adresse1.g2=0;
			adresse1.g3=0;
			adresse1.g4=0;
			adresse1.g5=0;
			adresse1.g6=0;
			adresse1.g7=0;
			adresse1.g8=0;
			calcul_adressv6(adresse1, timeout);
		}
	}
	lookupv6();
}




void lookupv4(void)
{
	if (compteurip==0)  /* si compteurip=0 i.e si aucune ip actives n'a été détectée*/
		return;         /* on retourne au menu */

	printf("\n\n%lu adresse(s) IP active(s) detectee(s) : \n", compteurip);
	printf("---------------------------------------\n");

	while(l != NULL)    /*on affiche l'ensemble du contenu de la liste*/
	{
		printf("\n%s", l->target);
		l=l->next;
	}
}





void lookupv6(void)  /* idem à lookupv4() à l'exception de la requete*/
{
	if (compteurip==0)
		return;

	printf("\n\n%lu adresse(s) IP active(s) detectee(s) : \n", compteurip);
	printf("---------------------------------------\n");

	while(l != NULL)
	{
		printf("\n%s", l->target);
		l=l->next;
	}
}





int verif_syntaxv4(adress_v4 adresse1, adress_v4 adresse2)
{
	/* vérification de la syntaxe de l'adresse de debut */
	/*si l'un des membres de l'adresse ip de début est superieur à 255 ou
      inférieur à 0*/
	if (adresse1.d1>255 || adresse1.d2>255 || adresse1.d3>255 || adresse1.d4>255)
		return (-1);

	/* vérification de la syntaxe de l'adresse de fin */
	if (adresse2.f1>255 || adresse2.f2>255 || adresse2.f3>255 || adresse2.f4>255)
		return (-2);

	/* vérification de la cohérence de la plage entrée */
	if ((adresse1.d1*256*256*256 + adresse1.d2*256*256 + adresse1.d3*256 +
         adresse1.d4) >
        (adresse2.f1*256*256*256 + adresse2.f2*256*256 + adresse2.f3*256 +
         adresse2.f4))
		return (-3);

    return SUCCESS;
}




int verif_syntaxv6(adress_v6 adresse1, adress_v6 adresse2)
{
	/* Adresse de début */
	/* si l'un des membres de l'ip est superieure a 65535 i.e FFFFF
       ou inférieur à 0*/
	if (adresse1.g1>65535 || adresse1.g2>65535 || adresse1.g3>65535 ||
        adresse1.g4>65535 || adresse1.g5>65535 || adresse1.g6>65535 ||
        adresse1.g7>65535 || adresse1.g8>65535)
		return (-1);

	/* Adresse de fin */
	if (adresse2.h1>65535 || adresse2.h2>65535 || adresse2.h3>65535 ||
        adresse2.h4>65535 || adresse2.h5>65535 || adresse2.h6>65535 ||
        adresse2.h7>65535 || adresse2.h8>65535)
		return (-2);
		
	if ((adresse1.g1*65537*65537*65537*65537*65537*65537*65537 +
         adresse1.g2*65537*65537*65537*65537*65537*65537 +
         adresse1.g3*65537*65537*65537*65537*65537 +
         adresse1.g4*65537*65537*65537*65537 +
         adresse1.g5*65537*65537*65537 +
         adresse1.g6*65537*65537 +
         adresse1.g7*65537 +
         adresse1.g8) >
         (adresse2.h1*65537*65537*65537*65537*65537*65537*65537 +
         adresse2.h2*65537*65537*65537*65537*65537*65537 +
         adresse2.h3*65537*65537*65537*65537*65537 +
         adresse2.h4*65537*65537*65537*65537 +
         adresse2.h5*65537*65537*65537 +
         adresse2.h6*65537*65537 +
         adresse2.h7*65537 +
         adresse2.h8))
		return (-3);
		
	return SUCCESS;
}



list insert_tailing(char *adresse, list l)
{
    /*réservation de l'espace mémoire pour un maillon*/
	if((m=(maillon *)malloc(sizeof(maillon)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du reverse DNS."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}

  /*si la liste est vide, le maillon devient le seul élément de la liste*/
	if(l==NULL)
	{
		m->target=adresse;
		m->next=l;
		return m;
	}

  /* sinon on procède à un appel récursif afin de recommencer le test avec le
     maillon suivant */
	else
		l->next = insert_tailing(adresse, l->next);

	return l;
}


void namehotechoix(void)
{
	int choix;
    char ip[20];

	printf("\n\n----[Effectuer un reverse DNS sur quelle type d'adresse ?");
	printf("\n\n--------[1 Ipv4\n");
	printf("--------[2 Ipv6\n");
	printf("\nChoix : ");
	scanf("%d%*c",&choix);
	switch(choix)
	{
		case 1 : printf("\n\nEntrez l'adresse : ");
                 scanf("%s%*c",ip);
                 namehotev4(ip);
 	             printf("\n\nAppuyez sur ENTREE pour retourner au menu...");
	             getchar();

	             demande();
                 break;

		case 2 : printf("\n\nEntrez l'adresse : ");
                 scanf("%s%*c",ip);
                 namehotev6(ip);
 	             printf("\n\nAppuyez sur ENTREE pour retourner au menu...");
	             getchar();

	             demande();
                 break;

		default : fprintf(stderr,"\n\n--[Erreur, aucun mode associe a ce choix !"
                       "\n   Appuyez sur une ENTREE pour retourner au menu...");
                  getchar();
			      return;
	}
}




void namehotev4(char *adresse) /* idem à lookupv4 */
{
	char *req_aux, *req, *file=">log.txt", *host="Envoi", hostfind[100];
	int i;
	FILE *hoste; /* pour le cas ou le fichier aurait déjà été ouvert
                     (dans la version en ligne de commande par exemple) */

	req_aux=V4_LOOKUP;  /*req=ping -a*/

    /*réservation de l'expace mémoire pour stocker la requête*/
	if((req=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				       "\n   Abandon du reverse DNS."
				       "\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}

    /*req=ping -a ADRESSE_IP>result.txt*/
	sprintf(req,"%s %s%s", req_aux, adresse, file);

	printf("\nExecution du reverse DNS sur l'adresse %s en cours...", adresse);

	system(req);

  /*on ouvre le fichier où est sauvegardé resultat de la requete*/
	if((hoste = fopen("log.txt", "r"))==NULL)
	{
		fprintf(stderr,"\n\n----[Erreur lors de la lecture du fichier \"log.txt\"!"
				       "\n     Ce fichier contient un compte rendu du reverse DNS."
				       "\n     Vérifiez que vous avez les droits en écriture dans le répertoire courant."
				       "\n     Abandon du scan..."
				       "\n\n     Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}
	fscanf (hoste, "%s", hostfind);

  /*on recherche le nom d'hote parmis les informations*/
	while (strcmp(hostfind, host)!=0)
		fscanf (hoste, "%s", hostfind);

	for (i=0; i < 5; i++)
		fscanf(hoste, "%s", hostfind);

    if(!strcmp(hostfind, adresse))
        printf("\nL'hote specifie ne semble pas etre connecte...\n");

    else
        {  /*on affiche le nom d'hote*/
           printf("\nLe nom d'hote est [ %s ].\n", hostfind);

	       fclose(hoste); /*on ferme log.txt*/
	       system(ERASE); /*on supprime log.txt*/
        }
}


void namehotev6(char *adresse) /* idem à lookupv6 */
{
	char *req_aux, *req, *file=">log.txt", *host="Envoi", hostfind[100];
	int i;
	FILE *hoste;

	req_aux=V6_LOOKUP;

	if((req=(char*)malloc(BUFFER*sizeof(char)))==NULL)
	{
		fprintf(stderr,"\n\n----[Problème d'allocation memoire !"
				"\n   Abandon du reverse DNS."
				"\n\n   Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}

	sprintf(req,"%s %s%s", req_aux, adresse, file);

	printf("\nExecution du reverse DNS sur l'adresse %s en cours...", adresse);

	system(req);

	if((hoste = fopen("log.txt", "r"))==NULL)
	{
		fprintf(stderr,"\n\n----[Erreur lors de la lecture du fichier \"log.txt\"!"
				       "\n     Ce fichier contient un compte rendu du reverse DNS."
				       "\n     Vérifiez que vous avez les droits en écriture dans le répertoire courant."
				       "\n     Abandon du scan..."
				       "\n\n     Appuyez sur ENTREE pour quitter");
		getchar();
		exit(SUCCESS);
	}
	fscanf (hoste, "%s", hostfind);

	while (strcmp(hostfind, host)!=0)
		fscanf (hoste, "%s", hostfind);

	for (i=0; i < 4; i++)
		fscanf(hoste, "%s", hostfind);

    if(!strcmp(hostfind, adresse))
        printf("\nL'hote specifie ne semble pas etre connecte...\n");

    else
        {
	       printf("\nLe nom d'hote est [ %s ].\n", hostfind);

	       fclose(hoste);
	       system(ERASE);
        }
}

void credit(void)
{
    printf("[ IPnSCAN v1.0 - IP Range Scanner & Reverse DNS Tool ]"
           "\n[    By Andras & Skyrunner for DEAD-END Mag #2 !     ]\n\n");
}

void usage(void)
{
    credit();
	printf("options:\n");
	printf("\t-h  Help\n");
	printf("\t-q  IP v.4 range scan (IPNSCAN -q [addr. beginning] [addr. ending])\n");
	printf("\t-s  IP v.6 range scan (IPNSCAN -s [addr. beginning] [addr. ending])\n");
	printf("\t-v  Speed (IPNSCAN -v [1,2,3] -q/s [addr. beginning] [addr. ending])\n");
	printf("\t\t  1->Mode BURN [TimeOut=100]\n"
			"\t\t  2->Mode NORMAL [TimeOut=500]\n"
			"\t\t  3->Mode SHERLOCK [TimeOut=1000]\n");
	printf("\t-d  IP v.4 Reverse DNS (IPNSCAN -d [address])\n");
	printf("\t-f  IP v.6 Reverse DNS (IPNSCAN -f [address])\n");

	exit(SUCCESS);
}



/*-----------------------------Programme Principal----------------------------*/
int main(int argc, char **argv)
{
	char *arg=NULL;
	int i, speed=HIGH_SPEED;
	adress_v4 adresse1, adresse2;
	adress_v6 adresse_1, adresse_2;
	FILE *log;

	if(argc<=1)/* si pas d'arguments on passe à la version intéractive */
	{
		system("title IPnSCAN v1.0 - IP Range Scanner and Reverse DNS Tool by Andras and Skyrunner");
		while(1) demande();
	}
	else
	{
		if((log=fopen("log.txt", "w+"))==NULL)
		{
			fprintf(stderr,"\n\n----[Erreur lors de la lecture du fichier \"log.txt\"!"
					       "\nCe fichier est indispensable a la poursuite du scan ou du reverse DNS..\n");
			exit(SUCCESS);
		}

		for (i = 1; i < argc; i++)
		{
			arg = argv[i];

			switch (arg[0])
			{
				case '-' : switch(arg[1])
						   {
							   case 'h':usage(); break;

							   case 'q':
                                    credit();
								    fprintf(log,"%s\n", argv[++i]);
                                    fprintf(log,"%s", argv[++i]);
                                    rewind(log);
								    fscanf(log,"%d.%d.%d.%d%*c",&adresse1.d1,&adresse1.d2,&adresse1.d3,&adresse1.d4);
								    fscanf(log,"%d.%d.%d.%d%*c",&adresse2.f1,&adresse2.f2,&adresse2.f3,&adresse2.f4);
								    fclose(log);
                                    system(ERASE);
                                    switch(verif_syntaxv4(adresse1,adresse2))
	                                {
		                                case -1 : printf("\n\n----[L'adresse de debut a une syntaxe incorrect !"
				                                         "\n     Chaque partie de l'adresse doit etre comprise entre 0 et 256.\n");
				                                  exit(SUCCESS);
	
		                                case -2 : printf("\n\n----[L'adresse de fin a une syntaxe incorrect !"
			                                             "\n     Chaque partie de l'adresse doit etre comprise entre 0 et 256.\n");
				                                  exit(SUCCESS);
	
		                                case -3 : printf("\n\n----[L'adresse de debut est superieure a l'adresse de fin !\n");
			                                      exit(SUCCESS);
			  
                                        default : break;
	                                }
								    printf("\n\n----[TimeOut = %d !\n"
										    "\nDEMARRAGE DU SCAN...\n", speed);
								    calcul_adressv4(adresse1, speed);
								    incrementev4(adresse1, adresse2, speed);
								    if (!compteurip)
									    printf("\n\nScan termine !"
									 	       "\nAucune IP active dans la tranche scannee\n");
								    else
									    printf("\n\n----[Scan termine !\n");
								    break;

							   case 's':
                                    credit();
                                    fprintf(log,"%s\n", argv[++i]);
								    fprintf(log,"%s", argv[++i]);
								    rewind(log);
								    fscanf(log,"%x:%x:%x:%x:%x:%x:%x:%x%*c",&adresse_1.g1, &adresse_1.g2, &adresse_1.g3, &adresse_1.g4, &adresse_1.g5, &adresse_1.g6, &adresse_1.g7, &adresse_1.g8);
								    fscanf(log,"%x:%x:%x:%x:%x:%x:%x:%x%*c",&adresse_2.h1, &adresse_2.h2, &adresse_2.h3, &adresse_2.h4, &adresse_2.h5, &adresse_2.h6, &adresse_2.h7, &adresse_2.h8);
								    fclose(log);
                                    system(ERASE);
                                    switch(verif_syntaxv6(adresse_1,adresse_2))
	                                {
		                                case -1 : printf("\n\n----[L'adresse de debut a une syntaxe incorrect !"
				                                         "\n     Chaque partie de l'adresse doit etre comprise entre 0 et FFFF.\n");
				                                  exit(SUCCESS);
	
		                                case -2 : printf("\n\n----[L'adresse de fin a une syntaxe incorrect !"
			                                             "\n     Chaque partie de l'adresse doit etre comprise entre 0 et FFFF.\n");
				                                  exit(SUCCESS);
	
		                                case -3 : printf("\n\n----[L'adresse de debut est superieure a l'adresse de fin !\n");
			                                      exit(SUCCESS);
			  
                                        default : break;
	                                }
                                    printf("\n\n----[TimeOut = %d !\n"
										   "\nDEMARRAGE DU SCAN...\n", speed);
								    calcul_adressv6(adresse_1, speed);
								    incrementev6(adresse_1, adresse_2, speed);
								    if (!compteurip)
									    printf("\n\nScan termine !"
									           "\nAucune IP active dans la tranche scannee!\n");
								    else
									    printf("\n\n----[Scan termine !\n");
								    break;

							   case 'v':
                                     switch(arg[i+=2])
                                     {
                                         case '1': speed=HIGH_SPEED; break;
					                     case '2': speed=MEDIUM_SPEED; break;
					                     case '3': speed=LOW_SPEED; break;
                                     }
					                 i--;
                                     break;

                               case 'd' :
                                     credit();
                                     fclose(log);
                                     if(argv[++i]==NULL)
                                     {
                                          printf("\n\n----[Vous n'avez pas specifie d'adresse !\n");
                                          exit(SUCCESS);
                                     }
                                     namehotev4(argv[i]);
                                     break;

                               case 'f' :
                                     credit();
                                     fclose(log);
                                     if(argv[++i]==NULL)
                                     {
                                          printf("\n\n----[Vous n'avez pas specifie d'adresse !\n");
                                          exit(SUCCESS);
                                     }
                                     namehotev6(argv[i]);
                                     break;

                               default :
                                     credit();
                                     printf("\n--[Erreur de saisie ! Argument [ -%c ] invalide...\n"
                                            "   IPnSCAN -h pour les options.\n", arg[1]);
                                     exit(SUCCESS);
                           }
			}
		}
	}
    return 0;
}
/*-----------------------------------E.O.F------------------------------------*/

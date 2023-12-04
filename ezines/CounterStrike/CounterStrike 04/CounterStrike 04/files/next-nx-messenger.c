/* Next[NX] Messenger Generator v1.4.8b by LoginX  TipiaK */
/* Un grand nombre de bugs ont étés fixés (tous les bugs connus) */
/* Le programme est maintenant capable de sauvegarder et recharger */
/* Tous les parametres sans sortir de l'interface... */
/* Les skins ont étés améliorés, le source du script et du fichier HTML aussi */
/* Le programme devrait pouvoir se compiler sans aucun warning ou erreur de compilation */
/* Pour tout commentaire ou suggestion, ecrivez a loginx@tipiak.net */

/* Last modified Thursday July the 7th 2000 at 2.00am by LoginX */

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

/* STRUCTURES */

struct SKIN {                 /* Initialisation de la structure qui contiendra les informations du skin choisi */
	char * textcolor;
	char * background;
	char * tableback;
	char * rollover;
	char * regularlink;
	char * vlink;
	char * alink;
	char * name;
   char * font;
   char * pathtml;
   char * pathmrc;
} skin;                       /* Creation immediate d'une structure de ce type */

/* PROTOTYPES */

void present (void);          /* Ecran de presentation */
int central (void);           /* Menu principal */
int skinmenu (void);          /* Menu SKIN */
int pathmenu (void);          /* Menu path */
int lastmenu (void);          /* Menu creation */
void epsylonskin (void);      /* Choix du skin Epsylon (et remplissage de la structure SKIN -> skin en consequence */
void tipiakskin (void);       /* Choix du skin TipiaK (et remplissage de la structure SKIN -> skin en consequence */
void termskin (void);         /* Choix du skin Terminal (et remplissage de la structure SKIN -> skin en consequence */
void bureau_fr (void);        /* Choix du path bureau de windows version FR */
void bureau_us (void);        /* Choix du path bureau de windows version US */
void custopath (void);        /* Choix d'un path personalise */
void type_error (void);       /* Message d'erreur par defaut */
void generate (void);         /* Generation du script personalise */
void save (void);             /* Sauvegarde des parametres */
void custohtml (void);        /* Choix d'un path personalise pour le fichier HTML */
void html_bus (void);         /* Choix du path bureau de windows version US pour le fichier HTML */
void html_bfr (void);         /* Choix du path bureau de windows version FR pour le fichier HTML */
int htmlmenu (void);          /* Menu choix du path pour le fichier HTML */
int load (void);              /* Chargement automatique de parametres */

/* VARIABLES GLOBALES */

char pager[150];              /* Ces 2 variables contiendront les paths pour le script et le fichier HTML */
char html[150];

/* ********************  MAIN  ******************** */

int main (void)
{

	int centralans, skinans,   /* On defini ici les variables qui stoqueront les retours des fonctions suivantes */
		 pathans, lastans, htmlans;

	present();                 /* On balance l'ecran d'accueil */

	central:                   /* Ici on place juste un label pour plus tard */
	centralans = central();    /* La variable centralans contiens maintenant le retour de la fonction central() */

	switch (centralans)        /* On dirige l'utilisateur vers la fonction qu'il a choisi en fonction
                                 de la valeur de centralans */
	{
		case '1':  { skinans = skinmenu(); goto skinanswer;               }
		case '2':  { pathans = pathmenu(); goto pathanswer;               }
		case '3':  { htmlans = htmlmenu(); goto htmlanswer;               }
		case '4':  { lastans = lastmenu(); goto lastanswer;               }
		case '5':  { return(EXIT_SUCCESS);                                           }
		default:   { type_error(); goto central;                          }
	}

	skinanswer:                /* Et ainsi de suite pour toutes les autres fonctions */
	switch (skinans)
	{
		case '1':  { epsylonskin(); break;                                }
		case '2':  { tipiakskin(); break;                                 }
		case '3':  { termskin(); break;                                   }
		case '4':  { goto central;                                        }
		default:   { type_error(); skinans = skinmenu(); goto skinanswer; }
	}
	goto central;

	pathanswer:                /* Ca commence a avoir de la gueule */
	switch (pathans)
	{
		case '1':  { bureau_fr(); break;                                  }
		case '2':  { bureau_us(); break;                                  }
		case '3':  { custopath(); break;                                  }
		case '4':  { goto central;                                        }
		default:   { type_error();pathans = pathmenu(); goto pathanswer;  }
	}
	goto central;

	htmlanswer:                /* Sous Borland C++ ca tape bien, l'alignement a une sacree gueule */
	switch (htmlans)
	{
		case '1':  { html_bfr(); break;                                   }
		case '2':  { html_bus(); break;                                   }
		case '3':  { custohtml(); break;                                  }
		case '4':  {goto central;                                         }
		default:   { type_error(); htmlans = htmlmenu(); goto htmlanswer; }
	}
	goto central;

	lastanswer:                /* l33t... ph34r */
	switch (lastans)
	{
		case '1':  { generate(); break;                                   }
		case '2':  { save(); break;                                       }
      case '3':  { load(); break;       }
		case '4':  { goto central;                                        }
		default:   { type_error(); lastans = lastmenu(); goto lastanswer; }
	}
	goto central;
}

void present (void)
{
	struct date dmy;           /* la structure date est integree dans dos.h, voici la structure telle qu'on la
                                 trouve dans la lib :
										struct date {
  										int da_year;      current year
  										char da_day;      day of the month
  										char da_mon;      month (1 = Jan)
										}; */

	clrscr();                 /*Ici, on nettoie l'ecran (au sens figure bien sur */
	fflush(stdout);           /* Maintenant, on vide la memoire de stdout */
	gotoxy(16,8);             /* On place le curseur a 14 dans l'axe des x, 8 dans l'axe des y */
	textcolor(14);            /* Le nom de la fonction parle de lui meme */
	cprintf("NEXT[NX] Pager Genrerator by LoginX (TipiaK)");
	gotoxy(14+8,9);           /* On utilise cprintf au lieu de printf seulement quand on a besoin des couleurs */
	cprintf("Get it at http://www.tipiak.net/");
	getdate(&dmy);            /* Remplissage automatique de la structure dmy (la date) */
	gotoxy(12+4,10);
	textcolor(12);
	cprintf("Date : %2.2d/%2.2d/%4.4d Date du programme : 14/06/2000", dmy.da_day, dmy.da_mon, dmy.da_year);
	gotoxy(27,12);            /* sur la ligne au dessus, on affiche juste la date */
	textcolor(138);
	cprintf("  FINAL VERSION 1.50f !  ");
	gotoxy(10+8,14);
	textcolor(15);
	cprintf("Pressez la touche Return pour continuer");
	fflush(stdin);            /* Maintenant on vide le buffer d'entree standart (le clavier en general) */
	getchar();                /* Getchar retourne une valeur apres l'appui de la touche [enter] */
}

int central (void)           /* La ca devient interressant */
{
	clrscr();
	textcolor(15);
	gotoxy(20,7);
	cprintf("1 - Choisir votre Skin");
	gotoxy(20,9);
	cprintf("2 - Choisir l'emplacement du fichier pager-html.mrc");
	gotoxy(20,11);
	cprintf("3 - Choisir l'emplacement du fichier HTML");
	gotoxy(20,13);
	cprintf("4 - G\202n\202rer / Sauver / Charger");
	gotoxy(20,15);
	cprintf("5 - Quitter");
	gotoxy(20,17);
	fflush(stdin);            /* On vient d'afficher le menu a l'ecran, on vide le buffer clavier */
	return getche();          /* Interressant, la fonction getche s'execute et la valeur indiquée par */
                             /* l'utilisateur est donc le retour, grace a retour(), ca devient aussi le
                                retour de la fonction */
}
                             /* Maintenant, c'est plus que de la routine tout ca */
int skinmenu (void)
{
	clrscr();
	textcolor(15);
	gotoxy(20,7);
	cprintf("1 - Skin Epsylon");
	gotoxy(20,9);
	cprintf("2 - Skin TipiaK");
	gotoxy(20,11);
	cprintf("3 - Skin Terminal");
	gotoxy(20,13);
	cprintf("4 - Menu pr\202c\202dent");
	gotoxy(20,15);
	return getche();
}

int pathmenu (void)          /* Encore meme chose */
{
	clrscr();
	textcolor(14);
	gotoxy(20,7);
	cprintf("1 - Bureau Windows 95/98 version FRANCAISE  ** DEFAULT **");
	textcolor(15);
	gotoxy(20,9);
	cprintf("2 - Bureau Windows 95/98 version US");
	gotoxy(20,11);
	cprintf("3 - Personaliser l'emplacement");
	gotoxy(20,13);
	cprintf("4 - Menu pr\202c\202dent");
	gotoxy(20,15);
	return getche();
}

int lastmenu (void)          /* En voila encore une couche */
{
	clrscr();
	textcolor(15);
	gotoxy(20,7);
	cprintf("1 - G\202n\202rer votre pager");
	gotoxy(20,9);
	cprintf("2 - Sauver les param\212tres");
   gotoxy(20,11);
   cprintf("3 - Charger des param\212tres");
	gotoxy(20,13);
	cprintf("4 - Menu Pr\202c\202dent");
	gotoxy(20,13);
	return getche();
}

void epsylonskin (void)      /* Cette fonction remplis la structure SKIN -> skin */
{
	skin.name = "E P S Y L O N";
	skin.textcolor = "#FFFFFF";
	skin.background = "#808080";
	skin.tableback = "#C0C0C0";
	skin.regularlink = "#FFFF00";
	skin.vlink = "#FFFF00";
	skin.alink = "#FFFF00";
	skin.rollover = "#E8E8E8";
   skin.font = "Georgia, \"Times New Roman\", Times, serif";
}

void tipiakskin (void)       /* Pareil */
{
	skin.name = "T I P I A K";
	skin.textcolor = "#FFFFFF";
	skin.background = "#000000";
	skin.tableback = "#000080";
	skin.regularlink = "#008000";
	skin.vlink = "#008000";
	skin.alink = "#008000";
	skin.rollover = "#00FFFF";
   skin.font = "Verdana, Arial, Helvetica, sans-serif";
}

void termskin (void)         /* Le C c'est cool, ca se repete souvent sur les progs de ce genre la */
{
	skin.name = "T E R M I N A L";
	skin.textcolor = "#00FF00";
	skin.background = "#000000";
	skin.tableback = "#006666";
	skin.regularlink = "#FFFFFF";
	skin.vlink = "#FFFFFF";
	skin.alink = "#FFFFFF";
	skin.rollover = "#00FF00";
   skin.font = "\"Courier New\", Courier, mono";
}

void bureau_fr (void)        /* Aucun secret, on remplis juste la variable de path pager avec le
                                path du bureau de windows version FR. Nouveau, on remplis aussi
                                skin.pathmrc parce que ca rentre dans la structure skin donc quand
                                on veut sauver tous les parametres, on sauve que la structure dans
                                un fichier, c'est plus facile a ecrire et plus facile a lire.
                                Si l'option de sauvegarde/chargement des parametres n'existait pas
                                on aurait pas besoin de la premiere ligne de ce module.          */
{
	skin.pathmrc = "c:\\windows\\bureau\\pager-html.mrc";
	strcpy(pager, "c:\\windows\\bureau\\pager-html.mrc\0");
}

void bureau_us (void)        /* Pareil, mais version US */
{
   skin.pathmrc = "c:\\windows\\desktop\\pager-html.mrc";
 	strcpy(pager, "c:\\windows\\desktop\\pager-html.mrc\0");
}

void custopath (void)        /* La c'est different, la personne doit indiquer elle meme son path */
{

	char path1[150];          /* La reponse de la personne sera stoquee la dedans */

	clrscr();
	textcolor(15);
	gotoxy(20,14);
	cprintf("Entrez ici le path");
	gotoxy(20,15);
	cprintf("Mais PAS le nom du fichier ni le \"\\\"");
	gotoxy(20,16);
	cprintf("a la fin du path.");
	gotoxy(20,17);
	cprintf("Ex : c:\\mirc");
	gotoxy(20,19);
	getch();
	gets(path1);              /* gets() parce que je le vaux bien */
	kbhit();                  /* ca, c'est juste pour qu'il vous affiche pas votre derniere reponse */
	strcat(pager, path1);     /* strcat, tout le monde connait... je commente pas trop */
	strcat(pager, "\\pager-html.mrc\0");
	skin.pathmrc = pager;     /* Pour sauvegarder, on redirige le contenu de pager dans skin.pathmrc comme avant */
}

void type_error (void)       /* La c'est un truc qui reviendra a chaque erreur, comme vous pouvez voir
                                sur les switch`s de main(); */
{
	gotoxy(20,20);
	textcolor(12);
	cprintf("Erreur, fonction non reconnue ! Veuillez re-essayer");
	sleep(2);                 /* Un petit delais pour que le mec refflechisse avant de faire des
                                conneries, et pour eviter un overflow */
}

void generate (void)         /* La on ouvre un fichier, on ecrit le script dedans, les variables
                                sont les personalisation par le programme */
{
	FILE * pfile;

	pfile = fopen(skin.pathmrc, "wt");

	fprintf(pfile, ";NOM DU SKIN :       %s\n", skin.name);
	fprintf(pfile, "on *:text:page*:?:{\n");
	fprintf(pfile, "  if ($exists(%s) = $true) {\n", html);
	fprintf(pfile, "//write %s       <tr> $crlf         <td width=\"50%\"> $+ $nick $+ </td> $crlf         ", html);
   fprintf(pfile, "<td width=\"50%\"> $+ $2- $+</td> $crlf       </tr>\n");
	fprintf(pfile, "  }\n");
	fprintf(pfile, "  else {\n");
	fprintf(pfile, "//write %s <html> $crlf  $crlf <head> $crlf <meta name=\"GENERATOR\" ", html);
   fprintf(pfile, "content=\"Next[NX] HTML Pager v1.5f by TipiaK\"> $crlf <title>Next[NX] Messenger v1.5f ");
   fprintf(pfile, "- by TipiaK</title> $crlf <style type=\"text/css\"> $crlf <!-- $crlf body ");
   fprintf(pfile, "{font-family: %s} $crlf a:hover \{color: %s\; text-decoration: none}", skin.font, skin.rollover);
   fprintf(pfile, " $crlf a:link \{color: %s\; text-decoration: none}", skin.regularlink);
   fprintf(pfile, "  $crlf --> $crlf </style> ");
   fprintf(pfile, "$crlf </head> $crlf  $crlf <body bgcolor=\"%s\" text=\"%s\" ", skin.background, skin.textcolor);
   fprintf(pfile, "vlink=\"%s\" alink=\"%s\" > $crlf  $crlf", skin.vlink, skin.alink);
   fprintf(pfile, " <p align=\"center\"><b><u><font size=\"5\">NEXT[NX] PAGER v1.5f par TipiaK</font></u></b>");
   fprintf(pfile, "</p> $crlf <p align=\"right\">\n");
	fprintf(pfile, "//write %s <font size=\"5\"><font size=\"1\" color=\"%s\">", html, skin.textcolor);
   fprintf(pfile, "Voici vos nouveaux  $crlf   messages.<br> $crlf   Cet Add-on a été ecrit par TipiaK");
   fprintf(pfile, " - <a href=\"http://www.tipiak.net\">http://www.tipiak.net</a><br> $crlf   Des mises");
   fprintf(pfile, " a jour ont étées prévues pour cet Add-on</font></font></p> $crlf <div align=\"center\">");
   fprintf(pfile, "  $crlf   <center> $crlf \n");
	fprintf(pfile, "//write %s      <table border=\"1\" cellspacing=\"1\" width=\"90%\" bgcolor=\"", html);
   fprintf(pfile, "%s\" align=\"center\" bordercolor=\"#FFFFFF\"> $crlf       <tr>  $crlf         ", skin.tableback);
   fprintf(pfile, "<td width=\"50\45\">  $crlf           <p align=\"center\"><b>N I C K</b>  $crlf");
   fprintf(pfile, "         </td> $crlf         <td width=\"50\45\">  $crlf           <p align=\"center\">");
   fprintf(pfile, "<b>M E S S A G E</b>  $crlf         </td> $crlf       </tr> $crlf \n ");
	fprintf(pfile, "//write %s       <tr> $crlf         <td width=\"50\45\"> $+ $nick $+", html);
   fprintf(pfile, " </td> $crlf         <td width=\"50\45\"> $+ $2- $+</td> $crlf       </tr>");
	fprintf(pfile, "\n  }\n");
	fprintf(pfile, "if ( $window(@Next[NX]-Messenger) != $null ) {\n");
	fprintf(pfile, "  /echo 14 @Next[NX]-Messenger $nick : $2-\n");
	fprintf(pfile, "\n}\n");
	fprintf(pfile, "else {\n");
	fprintf(pfile, "  /window -dk(0) +befs @Next[NX]-Messenger 0 1000 300 100\n");
	fprintf(pfile, "  /echo 1 @Next[NX]-Messenger NEXT[NX] - 14 M e s s e n g e r\n");
	fprintf(pfile, "  /echo 14 @Next[NX]-Messenger ______________________________\n");
	fprintf(pfile, "  /echo 14 @Next[NX]-Messenger Nouveaux Messages :\n");
	fprintf(pfile, "  /echo 14 @Next[NX]-Messenger  $+ $nick $+  : $2-\n\n");
	fprintf(pfile, "  }\n");
	fprintf(pfile, "}\n; ************* FIN DU SCRIPT *************");

	fclose(pfile);            /* Je commente pas, c'est du source mIRC, je commente pas le code mIRC moi, desole... */
                             /* Et bien sur, on ferme le fichier */
	textcolor(15);
	clrscr();
	gotoxy(20,10);
	cprintf("Fichier %s g\202n\202r\202 !", pager);
	gotoxy(20,11);
	cprintf("Lancez mIRC et tapez :");
	gotoxy(20,12);
	cprintf("/load -rs %s", pager);
	gotoxy(20,13);
	cprintf("Rapportez tout bug a loginx@tipiak.net");
	getch();                  /* no comment */
}                            /* je sais que c'est con de mettre un comment avec comme texte "no comment" mais bon... */

void html_bfr (void)         /* La c'est pour generer le script, faut lui dire ou mIRC doit balancer le fichier HTML */
                             /* On retrouve notre structure comme pour le path du script, toujours pour la meme raison */
{
   skin.pathtml = "c:\\windows\\bureau\\NEXTNX.html";
	strcpy (html, "c:\\windows\\bureau\\NEXTNX.html\0");
}

void html_bus (void)         /* Re-Belotte */
{
   skin.pathtml = "c:\\windows\\desktop\\NEXTNX.html";
	strcpy (html, "c:\\windows\\desktop\\NEXTNX.html\0");
}

void custohtml (void)        /* On reprends maintenant les concepts du module custopath */
{

	char html1[150];

	clrscr();
	textcolor(15);
	gotoxy(20,14);
	cprintf("Entrez ici le path");
	gotoxy(20,15);
	cprintf("Mais PAS le nom du fichier ni le \"\\\"");
	gotoxy(20,16);
	cprintf("a la fin du path.");
	gotoxy(20,17);
	cprintf("Ex : c:\\windows\\bureau");
	gotoxy(20,19);
	kbhit();
	getch();
	gets(html1);
	strcat(html, html1);
	strcat(html, "\\NEXTNX.html\0"); /* Oubliez pas le zero nul quand vous utilisez strcat !!! */
   skin.pathtml = html;      /* Et on met aussi le resultat dans la structure pour pouvoir sauver dans un file */
}

int htmlmenu (void)          /* La c'est le menu qui permet de faire le choix en question */
{
	clrscr();
	textcolor(14);
	gotoxy(20,7);
	cprintf("1 - Bureau Windows 95/98 version FRANCAISE  ** DEFAULT **");
	textcolor(15);
	gotoxy(20,9);
	cprintf("2 - Bureau Windows 95/98 version US");
	gotoxy(20,11);
	cprintf("3 - Personaliser l'emplacement");
	gotoxy(20,13);
	cprintf("4 - Menu pr\202c\202dent");
	gotoxy(20,15);
	return getche();
}

void save (void)             /* La c'est pour sauver les parametres, ca marche mais ca sert a rien,
                                j'ai pas encore le temps de faire la fonction qui les lit  ;)  */
{
	FILE * fileptr;

	fileptr = fopen("data_sav.epy", "w+");

	fwrite(&skin, sizeof(skin), 1 , fileptr); /* envoi au fichier de toute la structure */

	clrscr();
	textcolor(15);
	gotoxy(25,10);
	cprintf("Param\212tres sauvegard\202s");
   gotoxy(25,12);
   cprintf("Skin = %s", skin.name);
   gotoxy(25,14);
   cprintf("Fichier Script = %s", skin.pathmrc);
   gotoxy(25,16);
   cprintf("Fichier HTML = %s", skin.pathtml);

   getch();                  /* On affiche les informations, comme ca on peut verifier tous les parametres
                                Si ca ne va pas, on pourra toujours modifier depuis le programme */

   fclose(fileptr);          /* Encore un truc que j'avais oublié dans l'ancienne version...  ;)  */
}
                             /* Module de chargement des parametres */
int load (void)
{
	FILE * fileptr2;

   clrscr();
   textcolor(15);
   gotoxy(25,10);

   if ((fileptr2 = fopen("data_sav.epy", "r+")) == NULL)
   {
   	cprintf("Aucun parametre sauvegard\202");
      sleep(2);
      return(0);             /* Si le fichier data_sav.epy n'existe pas, il n'y a pas eu de sauvegarde, donc on annule */
   }

   fread(&skin, sizeof(skin), 1, fileptr2);
                             /* Mais sinon, on recupere toute la struct du fichier
                             (d'ou envoyer les paths dans la struct) */
   strcpy(pager, skin.pathmrc);
   strcpy(html, skin.pathtml);
                             /* On remet les info de path a leur place */
   cprintf("Parametres charg\202s.");
   gotoxy(25,12);            /* Ben ouais, parametres chargés, la structure est lue, les pointeurs sont redirigés */
   cprintf("Skin = %s", skin.name);
   gotoxy(25,14);
   cprintf("Fichier Script = %s", skin.pathmrc);
   gotoxy(25,16);
   cprintf("Fichier HTML = %s", skin.pathtml);

   getch();                  /* On affiche les informations, comme ca on peut verifier tous les parametres
                                Si ca ne va pas, on pourra toujours modifier depuis le programme */

   fclose(fileptr2);          /* Tiens, celui la aussi, je l'avais oublié */
   return(0);
}

/* Voila, ce programme est terminé, il fonctionne tres bien, le format est bon et regulier, le code a une
belle gueule, ce programme a probablement du vous apprendre pas mal de choses si vous debutez dans le
developpement en C sous windows, il gere tres bien l'ecriture/lecture dans un fichier, les couleurs,
le positionnement sur l'ecran, les structures, les pointeurs, les systemes de menus, tout est entierement
modulaire, on peut donc tres facilement ameliorer le programme, ajouter des skins, etc...

Maintenant pour ce qui est du script mIRC généré, c'est la partie la plus bordelique du programme donc
essayez d'y toucher le moins possible, surtout la partie du code HTML, c'est une partie assez sensible au
moindre changement.
Le script peut bien sur etre amelioré, pour par exemple se mettre en marche que quand vous etes away
et indiquer a la personne qui vous parlais que son message a bien été pris en compte et logué, on
peut aussi changer les propriétés de la fenetre, les couleurs, tout peut etre changé.

En reponse a une petite question, c'est vrai qu'on pourrait tout mettre dans des variables mIRC et faire
un petit script qui definies les variables dans le script directement, ce serait beaucoup plus facile, mais
ce n'est pas que pour le sens pratique des choses que j'ai fait cet add-on, c'est aussi pour le coté technique
Ca me permet de faire un programme commenté et vous apprendre le language C par cet intermediaire, pas trop vous
apprendre mais en tout cas, vous donner des astuces de programmation, ca c'est certain.

Ce programme et le script mIRC qui en sortent sont tous deux en license GNU-like, vous pouvez modifier ce programme
ainsi que le script généré mais mon nom ainsi que le nom du programme doit figurer la ou il figure sur la version
originale.
De plus, je vous invite chaudement a m'envoyer le programme une fois que vous l'avez modifié, ce qui me permettra
de faire avancer la version originale du programme si besoin est.
Vous avez aussi le droit d'emprunter des routines, modules ou juste des methodes de programmation que ce soit
sur ce programme ou sur le script mIRC mais un special thanks to LoginX from TipiaK doit apparaitre quelque part
meme si c'est dans un petit recoin du programme.

Pour tout commentaire ou suggestion au sujet du programme ou du script, je vous invite a m'ecrire par email
a l'adresse suivante : loginx@tipiak.net ou encore ecomdespe@hotmail.com
Pour me contacter par ICQ, mon # est 61291004
Venez aussi rejoindre tout le groupe TipiaK sur IRC, server : us.undernet.org
channel : #TipiaK                                                                */

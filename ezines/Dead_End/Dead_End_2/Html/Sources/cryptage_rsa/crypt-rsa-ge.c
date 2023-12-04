#include <stdio.h>
#include "prototypes.h"
#include "cryptage.h"
#include "decryptage.h"
#include "brutus_attack.h"

void demande(void)
{
   int choix;
   unsigned long int res, nb;
   cle c;
   FILE *premier;

   if ((premier = fopen("nb_premier_phi.txt","r")) != NULL)
     {                                            /* On supprime les eventuels fichier résultant d'une */
      fclose(premier);                            /*              execution du programme               */
      system("del nb_premier_phi.txt");
     }

   system("cls");
   /*------------------------------------------Menu---------------------------------------------*/
   printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[Bienvenue dans CRYPT-RSA-GE]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n\n");
   printf("ÄÄÄÄ[Que voulez vous faire ?");

   printf("\n\nÄÄÄÄÄÄÄÄ[1 Crypter un nombre\n");
   printf("ÄÄÄÄÄÄÄÄ[2 Decrypter un nombre\n");
   printf("ÄÄÄÄÄÄÄÄ[3 Tenter un brute force d'un chiffre crypte\n");
   printf("ÄÄÄÄÄÄÄÄ[4 Quitter le programme\n");
   printf("\nChoix : ");
   scanf("%d",&choix);
   printf("\n°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°");
   /*-------------------------------------------------------------------------------------------*/

   switch(choix)
     {
      case 1 : c = cleCryptage();  res = cryptage(c.nb, c.e, c.n); printf("Le nombre cypte est C = %lu",res);  break;  /* si choix=1 on appelle cleCryptage() puis cryptage() */
      case 2 : c = cleDecryptage();  res = decryptage(c.nb, c.d, c.n); printf("Le nombre decypte est M = %lu",res);  break;  /* si choix=2 on appelle cleDecryptage() puis decryptage() */
      case 3 : c = cleCassage();  res = brutus_attack(c.nb, c.e, c.n); printf("Le nombre decypte est M = %lu",res);  break;  /* si choix=3 on appelle cleCassage() puis cassage() */
      default: exit(0);  /* si choix autre que 1, 2 ou 3 alors on sort du programme */
     }

   getch();
   demande();
}

unsigned long int puissance(int a, int b)
{
   int i;
   unsigned long int res = 1;

   for (i=0; i<b; i++)
      res = res * a;

   return res;
}

cle cleCryptage(void)
{
   cle c;
   unsigned long int i;
   FILE *premier;

   premier = fopen("nb_premier_phi.txt","a");        /* On crée un fichier pour stocker les nombres */
   if (premier == NULL)                              /*                premiers avec phi            */
     {
      printf("\n\nErreur création du fichier nb_premier_phi.txt");
      exit(42);
     }

   printf ("\n\nEntrez le nombre a crypter : ");
   scanf("%lu",&c.nb);
   printf("Entrez le nombre premier p : ");
   scanf("%lu",&c.p);
   printf("Entrez le nombre premier q : ");
   scanf("%lu",&c.q);
   printf("\nCALCUL DE LA CLE N EN COURS...");
   c.n = c.p*c.q;
   printf("\nLa cle n = %lu\n",c.n);
   printf("\nCALCUL DE LA CLE PHI EN COURS...");
   c.phi = (c.p-1)*(c.q-1);
   printf("\nLa cle phi = %lu\n",c.phi);

   for (i = 2; i< c.phi/2; i++)        /*  On incrit dans le fichier tous les nombres    */
     if ( c.phi % i != 0)             /*      premiers avec phi jusqu'à phi / 2         */
        fprintf(premier,"%lu\n",i);
   fclose(premier);

   printf("\nUne liste des cles possibles d est disponible dans le fichier nb_premier_phi.txt");
   printf("\nEntrez la cle d (premier avec phi) : ");
   scanf("%lu",&c.d);
   printf("\nCALCUL DE LA CLE E EN COURS...");

   for (c.e=1; c.e < (c.phi*c.phi); c.e++)    /*  On recherche une chiffre tel que e*d-1 soit divisible par phi */
      if (((c.e*c.d) % c.phi) == 1)
         break;

   printf("\ne = %lu\n",c.e);
   return c;

}

cle cleDecryptage(void)
{
   cle c;
   printf ("\n\nEntrez le nombre a decrypter : ");
   scanf("%lu",&c.nb);
   printf("Entrez la cle prive d : ");
   scanf("%lu",&c.d);
   printf("Entrez la cle public n : ");
   scanf("%lu",&c.n);
   return c;
}

cle cleCassage(void)
{
   cle c;
   printf ("\n\nEntrez le nombre a decrypter : ");
   scanf("%lu",&c.nb);
   printf("Entrez la cle public e : ");
   scanf("%lu",&c.e);
   printf("Entrez la cle public n : ");
   scanf("%lu",&c.n);
   return c;
}

int main(void)
{
  system("title CRYPT-RSA-GE by Skyrunner");
  demande();
  return 0;

}

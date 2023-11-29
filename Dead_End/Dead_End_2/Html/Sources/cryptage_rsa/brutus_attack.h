/*------------------------------------------------BRUTE FORCE----------------------------------------------------------*/
unsigned long int brutus_attack(unsigned long int c, unsigned long int puiss, unsigned long mod)  /* mod = clé n  et  puiss = clé e  */
{
   unsigned long int res = 1, nvar, nvar2, nb;
   unsigned long int resultat[100000];
   int i, j, k,l; // compteurs
   int nnb, compt, var, caze;
   int tab[100000], tab2[100000];
   int  puissa = puiss;

   printf("\nTENTATIVE DE BRUTE FORCING EN COURS...\n");
   for (i=0; i<100000; i++)  // on remplit le tableau avec des 5
      tab[i]=5;

   for (i=0, compt = -1; puiss > 0; i++, compt++ )   // on convertit la puissance en binaire
     {
      if (puiss % 2 > 0)
      nnb = 1;
      if (puiss % 2 == 0)
      nnb = 0;
      tab[i]=nnb;
      puiss = puiss / 2;
     }

   for (i=0, j=0; tab[i] != 5; i++) // on convertit le binaire en puissance de 2 : par exemple (11101)b = 16 + 8+ 4 + 1
     {
      if (tab[i] == 1)
        {
         var = puissance(2,i);
         tab2[j] = var;
         j++;
        }
     }

   l = j; // on sauvegarde la valeur de j cad le nombre d'element ds le tableau tab2[]

   /*   On essaye toutes les valeurs possibles de M^e (mod n) avec M < n  jusqu'à ce que le resultat egal à C*/
   for (nb=1; nb < mod; nb++)
    {
      resultat[0]=0;
      resultat[1]=nb;                               /* On met dans le tableau resultat[] les restes du nombre nb par les  */
      nvar2 = nb;                                    /* puissances successives jusqu'à la puissance egal au plus grand nombre    */
      nvar = nb;                                    /* du tableau tab2[] par le modulo mod  */
      for (i=2; i<=tab2[j-1]+1; i++)
       {                                           /* Par exemple pour 29 = 16 + 8 + 4 +1 et pour nb =3 on et dans le tableau  */
         nvar2 = nvar * nvar2;                        /* les restes de 3^1, 3^2 jusqu'à 3^16 par le modulo mod      */
         nvar2 = nvar2%mod;
         resultat[i] = nvar2;
       }


      for (i=0, res=1; i<l; i++)  /*  Dans la case i du tableau resultat se trouve nb^i  */
       {
         caze = tab2[i];         /*  Donc à partir du tableau tab2[i] qui contient la décomposition de la puissance puiss */
         res = res * resultat[caze]; /*  en puissance de 2  ( 29 = 16 + 8 + 4 + 1), on peut obtenir nb^puiss en multipliant */
         if (res > mod)         /*  par exemple nb^16, on multiplie les cases resultat[16]*resultat[8]*resultat[4]*resultat[1] */
            res = res%mod;      /*  pour obtenir enfin le chiifre crypté on fait le resultat precedent modulo mod */
       }
         if ( res == c)
            break;

    }
   return nb;
}
/*--------------------------------------------FIN BRUTE FORCE------------------------------------------------------------*/



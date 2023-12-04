/////////////////////////////////////////////
// Cryptage RC4 - 2000                     //
// Ecrit par Sabrina - Sabrina@inorbit.com //
/////////////////////////////////////////////


#include <stdio.h>
#include <conio.h>


int tableS[256],tableK[256];

void swaper(int i,int j)
// On permute les elements d'indices i et j de TableS
{
int temp;
temp=tableS[i];
tableS[i]=tableS[j];
tableS[j]=temp;
}

int main(int argc, char *argv[])
{
char clef[256];
char t,buf;
long taille; // taille de la cl‚
long i,j;
FILE * entre, * sortie , * cle;

if(argc != 4)
 {
 // Comment ca marche ?
 printf("usage: %s <fichier_in> <fichier_out> <fichier_cl‚>\n", argv[0]);
 exit(-1);
 }


entre=fopen(argv[1], "rb");
sortie=fopen(argv[2], "wb"); // On ouvre
cle=fopen(argv[3], "rb");

fseek(cle, 0, SEEK_END);
taille = ftell(cle); // on choppe la taille de la cl‚
fseek(cle, 0,0);
fread( clef, taille, 1, cle);

// Creation de la table //
for (i=0 ; i<256 ; i++)
 tableS[i]=i;
for (i=0 ; i<256 ; i++)
  tableK[i]=clef[(i % taille) % 256];

j=0;
for (i=0 ; i<256 ; i++)
 {
 j= (j+tableS[i]+tableK[i]) % 256;
 swaper(i,j);
 }

 // Cryptage //
i=0;
j=0;

do
 {
 i=(i+1) % 256;
 j=(j+tableS[i]) % 256;
 swaper(i,j);
 fread(&buf, 1, 1, entre);
 t=tableS[(tableS[i]+tableS[j]) % 256] ;
 buf = buf ^ t;
 fwrite(&buf, 1, 1, sortie);
 }
while (!feof(entre));

fclose(cle);
fclose(sortie); // On ferme
fclose(entre);

printf("Au revoir :-)\n");
}
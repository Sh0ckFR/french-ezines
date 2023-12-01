/* crk3_patchbf.c */

#include <stdio.h>
#include <fcntl.h>

int main()
{

  int fd,fd2;
  char oc;

  if ((fd=open("crk3", O_WRONLY)) == -1)
    perror("Erreur");

  if ((fd2=open("crk3_autobf.data", O_RDONLY)) == -1)
    perror("Erreur");

  /* patch de l'entry point */
  if (lseek(fd, 0x18, SEEK_SET) == -1)
    perror("Erreur");
  oc = 0x54; write(fd,&oc,1);
  oc = 0x80; write(fd,&oc,1);


  /* ecriture de notre code de brute force */
  if (lseek(fd, 0x54, SEEK_SET) == -1)
    perror("Erreur");

  while (read(fd2,&oc,1)) {
    write(fd,&oc,1);
  }

  close (fd);
  close (fd2);


  return 0;
}

/* crk3_patcher.c */

#include <stdio.h>
#include <fcntl.h>

int main()
{

  int fd;
  char c = 0x35;

  if((fd=open("crk3", O_WRONLY)) == -1)
    perror("Erreur");

  if(lseek(fd, 0x16b, SEEK_SET) == -1)
    perror("Erreur");

  if(write(fd,&c,1) != 1)
    perror("Erreur");
  else
    printf("w00t you can now fire up your brute forcer\n");

  close(fd);


  return 0;
}

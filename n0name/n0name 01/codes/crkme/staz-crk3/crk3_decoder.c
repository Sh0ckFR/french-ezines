/* crk3_decoder.c */

#include <fcntl.h>
#include <stdio.h>

int main()
{

  int fd, fd2, i;
  unsigned char bl;
  char oc;

  bl = 0xa5;
  fd = open("crk3", O_RDONLY);
  fd2 = open("crk3_d", O_WRONLY | O_CREAT, 0600);

  
  for(i=0;i<0x54;i++) {
    read(fd,&oc,1);
    write(fd2,&oc,1);
  }

  for(i=0;i<0x1d0;i++) {
    read(fd,&oc,1);
    oc = oc ^ bl;
    printf("%x ", oc);
    write(fd2,&oc,1);
  }

  while (read(fd,&oc,1)) {
    write(fd2,&oc,1);
  }

  close(fd);
  close(fd2);

  return 0;
}


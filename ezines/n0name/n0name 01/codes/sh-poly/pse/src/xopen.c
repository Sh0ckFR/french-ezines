/*
** xopen.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 16:58:41 2005 eee
** 

Last update Tue Apr 12 16:59:21 2005 eee
*/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int	xopen(char *str, int flags, int mode)
{
  int	fd;

  if ((fd = open(str, flags, mode)) < 0)
    {
      

perror("open");
      exit(1);
    }
  return (fd);
}

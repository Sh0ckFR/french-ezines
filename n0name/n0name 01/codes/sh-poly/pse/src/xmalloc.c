/*
** xmalloc.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 22:35:03 2005 eee
** Last update Tue Apr 12 22:35:13 2005 eee
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	*xmalloc(int size)
{
  void	*ptr;
  char	*rst;

  if ((ptr = malloc(size)) == 0)
    {
      perror("malloc");
      exit(1);
    }
  rst = ptr;
  while (size)
    rst[--size] = 0;
  return (ptr);
}

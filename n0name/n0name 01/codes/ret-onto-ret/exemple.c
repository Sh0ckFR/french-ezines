#include <stdio.h>
#include <string.h>
#include <unistd.h>

void    copy(char *str)
{
  char	buf[64];
  int	i;

  memset(buf, 0, 64);
  for (i = 0; str[i]; i++) {
	 buf[i] = str[i];
  }
}

int     main(int ac, char **av)
{
  if (ac < 2) {
	 fprintf(stderr, "I take at least one argument.\n");
	 return (0);
  }
  copy(av[1]);
  return (0);
}
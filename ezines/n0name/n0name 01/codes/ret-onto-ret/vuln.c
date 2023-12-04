#include        <stdio.h>
#include        <string.h>
#include        <unistd.h>

void    copy(char *str)
{
  char	buf[1024];
  char	*new = strdup(str);
  int	i;

  for (i = 0; new[i]; i++) {
	 buf[i] = new[i];
  }
  buf[i] = 0;
}

int     main(int ac, char **av)
{
  if (ac < 2) {
	 fprintf(stderr, "I take at least one argument.\n");
	 return (0);
  }
  copy(av[1]);
  printf("pid = %d\n", getpid());
  while (1);
  return (0);
}
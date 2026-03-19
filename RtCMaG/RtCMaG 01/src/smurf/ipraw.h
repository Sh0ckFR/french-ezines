/******************************************************************************
 *                     ipraw.h by S/ash (member of RtC)                       *
 *          contain function prototypes for sending our own IP packets        *
 * RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st          *
 * RtC Tech : http://www.rtctech.fr.st                                        *
 * S/ash    : sl4sh@ifrance.com                                               *
 ******************************************************************************/

#ifndef __IPRAW_H__
#define __IPRAW_H__

#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

/*
 * File ipraw.c
 */
#ifdef DEBUGGING
void HEXDUMP(unsigned, unsigned char*);
#endif
unsigned short in_chksum(u_short*, int);
int tcp_send(int, struct sockaddr_in*, unsigned long, unsigned long, unsigned, unsigned,
	     unsigned char, unsigned long, unsigned long, unsigned, char*, unsigned);
int udp_send(int, struct sockaddr_in*, unsigned long, unsigned long, unsigned, unsigned,
	     char*, unsigned);
int icmp_send(int, struct sockaddr_in*, unsigned long, unsigned long, u_short, u_short,
	      u_short, u_short, char*, int);
struct in_addr resolve(char*);

#endif

/******************************************************************************
 *                   rtcsmurf.h by S/ash (member of RtC)                      *
 *            contain function prototypes for the tool RtC Smurf              *
 * RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st          *
 * RtC Tech : http://www.rtctech.fr.st                                        *
 * S/ash    : sl4sh@ifrance.com                                               *
 ******************************************************************************/
#ifndef __RTCSMURF_H__
#define __RTCSMURF_H__

/*#define DEBUGGING*/
#include "ipraw.h"


/* constants */
#define DEFDELAY            100
#define DEFDELAY_STR       "100"
#define DEFPACKETNB         1
#define DEFPACKETNB_STR    "1"
#define DEFPACKSIZE         256
#define DEFPACKSIZE_STR    "256"

/* error strings */
#define IPHDRWRITE_ERROR   "error : couldn't write ip header.\n"
#define CANTOPENFILE_ERROR "Cannot open file %s.\n"
#define CANTRESOLVE_ERROR  "Cannot resolve %s.\n"
#define MEMORY_ERROR       "error : not enough memory\n"

/* interface strings */
#define RESOLVINGTARG_STR  "Resolving target name...\n"
#define TARGETIP_STR       "Target is %s (%s)\n"
#define RESOLVINGBCL_STR   "Resolving broadcast list...\n"
#define ADDED_STR          "Added "
#define BROADCASTIP_STR    "\t%s\t(%s); \n"
#define START_STR          "\nFlooding..."
#define PACKETNB_STR       "\n%d..."
#define END_STR            "done.\n"

#define VERSTR             "/------------------------------------------------------------------------------\\\n"\
                           "|                                 RtC Smurf                                    |\n"\
                           "|                    Created by Sly and S/ash (members of RtC)                 |\n"\
                           "|                                Smurfing DoS                                  |\n"\
                           "|                         This is a tool of RtC Tech                           |\n"\
                           "| RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st            |\n"\
                           "| RtC Tech : http://www.rtctech.fr.st                                          |\n"\
                           "| Sly      : rtcsly@yahoo.fr                                                   |\n"\
                           "| S/ash    : sl4sh@ifrance.com                                                 |\n"\
                          "\\------------------------------------------------------------------------------/\n"

#define SYNTAXSTR          "syntax : rtcsmurf method destip {[-p pcksize] | destport fromport} bcfile\n"\
                           "                             [packetnb [delay]]\n"\
                           "           method   0 : ICMP Echo Request   1 : SYN TCP Packet   2 : UDP packet\n"\
                           "           destip     : ip of host to smurf\n"\
                           "           -p pcksize : size of packet for methods 0 (default " DEFPACKSIZE_STR ")\n"\
                           "           destport   : port to flood for methods 1-2\n"\
                           "           fromport   : port to use for flood (of broadcast ip) for methods 1-2\n"\
                           "           bcfile     : file containing the ip list to use for broadcast\n"\
                           "           paquetnb   : number of packets to send (default " DEFPACKETNB_STR ")\n"\
                           "           delay      : delay between two sending in ms (default " DEFDELAY_STR ")\n"


/*
 * File rtcsmurf.c
 */
void send_packet(int, struct sockaddr_in, struct sockaddr_in, int, long, long);
void smurf(struct sockaddr_in, struct sockaddr_in*, int, long, long, int);
void argument();
int main(int, char**);

#endif

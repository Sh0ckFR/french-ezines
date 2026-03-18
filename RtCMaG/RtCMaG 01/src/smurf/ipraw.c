/******************************************************************************
 *                     ipraw.c by S/ash (member of RtC)                       *
 *         contain function definitions for sending our own IP packets        *
 * RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st          *
 * RtC Tech : http://www.rtctech.fr.st                                        *
 * S/ash    : sl4sh@ifrance.com                                               *
 ******************************************************************************/
#include "ipraw.h"

/* tcp/ip raw functions */

/******************************************************************************
 * Function HEXDUMP for debugging -- RIPPED                                   *
 * No comment                                                                 *
 ******************************************************************************/
#ifdef DEBUGGING
void HEXDUMP(unsigned len, unsigned char *buf)
{
  unsigned i;
  for(i=0; i<len; i++)
    printf("%02X%c", *(buf+i), ((i+1)%20) ? ' ' : '\n');
}
#endif

/******************************************************************************
 * in_cksum -- RIPPED                                                         *
 *  Checksum routine for Internet Protocol family headers (C Version)         *
 ******************************************************************************/
unsigned short in_cksum(addr, len)
    u_short *addr;
    int len;
{
    register int nleft = len;
    register u_short *w = addr;
    register int sum = 0;
    u_short answer = 0;
 
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)  {
        sum += *w++;
        nleft -= 2;
    }
 
    /* mop up an odd byte, if necessary */
    if (nleft == 1) {
        *(u_char *)(&answer) = *(u_char *)w ;
        sum += answer;
    }
 
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);   /* add hi 16 to low 16 */
    sum += (sum >> 16);                   /* add carry */
    answer = ~sum;                        /* truncate to 16 bits */
    return(answer);
}


/******************************************************************************
 * Function tcp_send - MOSTLY RIPPED                                          *
 * send a customized TCP Header                                               *
 * Input  : socket : socket to write                                          *
 *          address : destination address                                     *
 *          s_addr, t_addr : source and destination address for checksum      *
 *          s_port, t_port : source and dest port                             *
 *          tcpflags : flags of tcp header                                    *
 *          seq : sequence number                                             *
 *          ack : ack number                                                  *
 *          win : window size                                                 *
 *          datagram : data to send in tcp packet                             *
 *          datasize : size of datagram                                       *
 * Output : tcp_send : result of send                                         *
 ******************************************************************************/
int tcp_send(int      socket,
	     struct sockaddr_in *address,
	     unsigned long s_addr,
	     unsigned long t_addr,
	     unsigned      s_port,
	     unsigned      t_port,
	     unsigned char tcpflags,
	     unsigned long seq,
	     unsigned long ack,
	     unsigned      win,
	     char          *datagram,
	     unsigned      datasize)
{
  struct pseudohdr  {
    u_int32_t saddr;
    u_int32_t daddr;
    u_int8_t  useless;
    u_int8_t  protocol;
    u_int16_t tcplength;
  };

  unsigned char packet[2048];
  struct iphdr        *ip     = (struct iphdr *)(packet);
  struct pseudohdr    *pseudo = (struct pseudohdr *)(packet+sizeof(struct iphdr)-sizeof(struct pseudohdr));
  struct tcphdr       *tcp    = (struct tcphdr *)(packet+sizeof(struct iphdr));
  unsigned char       *data   = (unsigned char *)(packet+sizeof(struct tcphdr)+sizeof(struct iphdr));      

  /*
   * The above casts will save us a lot of memcpy's later.
   * The pseudo-header makes this way become easier than a union.
   */
	
  memcpy(data,datagram,datasize);
  memset(packet,0,sizeof(struct iphdr)+sizeof(struct tcphdr));

  /* The data is in place, all headers are zeroed. */
  tcp->th_sport   = htons(s_port);
  tcp->th_dport   = htons(t_port);
  tcp->th_off     = 5;          /* 20 bytes, (no options) */
  tcp->th_flags   = tcpflags;
  tcp->th_seq     = htonl(seq);
  tcp->th_ack     = htonl(ack);
  tcp->th_win     = htons(win); /* we don't need any bigger, I guess. */
  
  /* pseudo header for cheksum */
  pseudo->saddr = s_addr;
  pseudo->daddr = t_addr;
  pseudo->protocol = IPPROTO_TCP;   
  pseudo->tcplength = htons(sizeof(struct tcphdr)+datasize);  

  /* The necessary TCP header fields are set. */
  
  tcp->th_sum = in_cksum((u_short*)pseudo, sizeof(struct tcphdr)+sizeof(struct pseudohdr)+datasize);
  
  /* Make the ip header */
  memset(packet, 0, sizeof(struct iphdr));
  ip->saddr    = s_addr;
  ip->daddr    = t_addr;
  ip->version  = 4;
  ip->ihl      = 5;
  ip->ttl      = 255;
  ip->id       = random()%1996;
  ip->protocol = IPPROTO_TCP; /* should be 6 */
  ip->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + datasize);
  ip->check    = in_cksum((char *)packet,sizeof(struct iphdr));

#ifdef DEBUGGING
  printf("Packet ready. Dump: \n");
#ifdef DEBUGGING_DATA
  HEXDUMP(sizeof(struct iphdr)+sizeof(struct tcphdr)+datasize,packet);
#else
  HEXDUMP(sizeof(struct iphdr)+sizeof(struct tcphdr),packet);
#endif
  printf("\n");
#endif
  
  return sendto(socket, packet, sizeof(struct iphdr)+sizeof(struct tcphdr)+datasize, 0, (struct sockaddr *)address, sizeof(struct sockaddr));
  
  /* And off into the raw socket it goes. */
}

/******************************************************************************
 * Function udp_send - MOSTLY RIPPED                                          *
 * send a customized UDP Header                                               *
 * Input  : socket : socket to write                                          *
 *          address : destination address                                     *
 *          s_addr, t_addr : source and destination address for checksum      *
 *          s_port, t_port : source and dest port                             *
 *          datagram : data to send in udp packet                             *
 *          datasize : size of datagram                                       *
 * Output : udp_send : result of send                                         *
 ******************************************************************************/
int udp_send(int      socket,
	     struct sockaddr_in *address,
	     unsigned long s_addr,
	     unsigned long t_addr,
	     unsigned      s_port,
	     unsigned      t_port,
	     char          *datagram,
	     unsigned      datasize)
{
  struct pseudohdr  {
    u_int32_t saddr;
    u_int32_t daddr;
    u_int8_t  useless;
    u_int8_t  protocol;
    u_int16_t udplength;
  };

  unsigned char packet[2048];
  struct iphdr        *ip     = (struct iphdr *)(packet);
  struct pseudohdr    *pseudo = (struct pseudohdr *)(packet+sizeof(struct iphdr)-sizeof(struct pseudohdr));
  struct udphdr       *udp    = (struct udphdr *)(packet+sizeof(struct iphdr));
  unsigned char       *data   = (unsigned char *)(packet+sizeof(struct udphdr)+sizeof(struct iphdr));      

  /*
   * The above casts will save us a lot of memcpy's later.
   * The pseudo-header makes this way become easier than a union.
   */
	
  memcpy(data,datagram,datasize);
  memset(packet,0,sizeof(struct iphdr)+sizeof(struct udphdr));

  /* The data is in place, all headers are zeroed. */
  udp->uh_sport   = htons(s_port);
  udp->uh_dport   = htons(t_port);
  udp->uh_ulen    = htons(sizeof(struct udphdr) + datasize);
  
  /* pseudo header for cheksum */
  pseudo->saddr = s_addr;
  pseudo->daddr = t_addr;
  pseudo->protocol = IPPROTO_UDP;   
  pseudo->udplength = htons(sizeof(struct udphdr)+datasize);  

  /* The necessary UDP header fields are set. */
  
  udp->uh_sum = in_cksum((u_short*)pseudo, sizeof(struct udphdr)+sizeof(struct pseudohdr)+datasize);
  
  /* Make the ip header */
  memset(packet, 0, sizeof(struct iphdr));
  ip->saddr    = s_addr;
  ip->daddr    = t_addr;
  ip->version  = 4;
  ip->ihl      = 5;
  ip->ttl      = 255;
  ip->id       = random()%1996;
  ip->protocol = IPPROTO_UDP;
  ip->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + datasize);
  ip->check    = in_cksum((char *)packet,sizeof(struct iphdr));

#ifdef DEBUGGING
  printf("Packet ready. Dump: \n");
#ifdef DEBUGGING_DATA
  HEXDUMP(sizeof(struct iphdr)+sizeof(struct udphdr)+datasize,packet);
#else
  HEXDUMP(sizeof(struct iphdr)+sizeof(struct udphdr),packet);
#endif
  printf("\n");
#endif
  
  return sendto(socket, packet, sizeof(struct iphdr)+sizeof(struct udphdr)+datasize, 0, (struct sockaddr *)address, sizeof(struct sockaddr));
  
  /* And off into the raw socket it goes. */
}

/******************************************************************************
 * icmp_send -- Send an icmp packet                                           *
 * Input  : sock   : socket descriptor                                        *
 *          dest   : destination address                                      *
 *          s_addr, t_addr : source and destination address for checksum      *
 *          type   : icmp type                                                *
 *          code   : icmp code                                                *
 *          ident  : icmp ident                                               *
 *          seq_nb : icmp sequence number                                     *
 *          data   : optional data                                            *
 *          sdata  : size of data                                             *
 * Output : return : result of send                                           *
 ******************************************************************************/
int icmp_send(int sock, struct sockaddr_in *dest,
	      unsigned long s_addr, unsigned long t_addr,
	      u_short type, u_short code, u_short ident, u_short seq_nb, 
	      char *data, int sdata)
{
  char packet[2048];
  struct iphdr   *ip       = (struct iphdr *)(packet);
  struct icmphdr *icmp     = (struct icmphdr*)(packet + sizeof(struct iphdr));
  unsigned char  *datagram = (unsigned char*)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));

  /* making the ip header   */
  memset(ip, 0, sizeof(struct iphdr));
  ip->saddr    = s_addr;
  ip->daddr    = t_addr;
  ip->version  = 4;
  ip->ihl      = 5;
  ip->ttl      = 255;
  ip->id       = random()%1996;
  ip->protocol = IPPROTO_ICMP;
  ip->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct icmphdr) + sdata);
  ip->check    = in_cksum((char *)packet,sizeof(struct iphdr));

  /* making the icmp header */
  memcpy(datagram, data, sdata);
  memset(icmp, 0, sizeof(struct icmphdr));

  icmp->type = type;
  icmp->code = code;
  icmp->un.echo.id = ident;
  icmp->un.echo.sequence = seq_nb;

  /* calculate the checksum */
  icmp->checksum = in_cksum((u_short*)packet, sdata+8);

#ifdef DEBUGGING
#ifdef DEBUGGING_DATA
  printf("ICMP packet ready, Dump : \n");
  HEXDUMP(sdata+sizeof(struct icmphdr)+sizeof(struct iphdr), packet);
  printf("\n");
#else
  printf("ICMP packet ready, Dump : \n");
  HEXDUMP(sizeof(struct icmphdr)+sizeof(struct iphdr), packet);
  printf("\n");
#endif
#endif
  return sendto(sock, packet, sizeof(struct icmphdr)+sizeof(struct iphdr)+sdata, 0, (struct sockaddr*)&dest, sizeof(struct sockaddr));
}

/******************************************************************************
 * Function resolve - resolve an host name                                    *
 * Input  : name : host name                                                  *
 * Output : resolve : in_addr struct for hostname                             *
 ******************************************************************************/
struct in_addr resolve(char *name)
{
  struct hostent *ent;
  struct in_addr in;

  if (!(ent=gethostbyname(name)))
    {
      in.s_addr=INADDR_NONE;
      return in;
    }

  return *(struct in_addr *)ent->h_addr; 
}

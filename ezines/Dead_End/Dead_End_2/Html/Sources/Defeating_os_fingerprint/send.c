#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
struct paquet_icmp
{
struct iphdr partieIP;
struct icmphdr partieICMP;
char icmpdata[4096];
};


int sendpacket()
{
struct paquet_icmp *icmppaquet;
struct sockaddr_in sock;
int sockraw,i,tailleDATA;
int calccheckICMP=0; 

icmppaquet=(struct paquet_icmp *)malloc(sizeof(struct paquet_icmp));
bzero(icmppaquet,sizeof(struct paquet_icmp));
makeheaderIP(&icmppaquet->partieIP);
makeheaderICMP(&icmppaquet->partieICMP);
tailleDATA=makeDATA((char *)&icmppaquet->icmpdata);
icmppaquet->partieICMP.checksum=calcule((unsigned short *)&icmppaquet->partieICMP,sizeof(struct icmphdr)+tailleDATA);

sockraw=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
sock.sin_family=AF_INET;
sock.sin_addr.s_addr=icmppaquet->partieIP.daddr;

if(sendto(sockraw,icmppaquet,sizeof(struct icmphdr)+sizeof(struct iphdr)+tailleDATA,0,(struct sockaddr *)&sock,sizeof(struct sockaddr))==-1)
{
printf("erreur d'envoi");
}

close(sockraw);
return(0);
}

int makeheaderICMP(struct icmphdr *icmp)
{
icmp->type=8; /* echo request */
icmp->code=0;
icmp->un.echo.id=1337;
icmp->un.echo.sequence=1337;
return(0);
}

int makeheaderIP(struct iphdr *ip)
{
int j=0;
char ipdest[50];
char ipsource[50];
struct hostent *host;
ip->version=4;
ip->ihl=5;
ip->tos=15; /* type of service */
ip->id=12345;
ip->frag_off=htons(16384);
ip->ttl=200;
ip->protocol=1;
printf("entrez ip de la machine dont vous souhaitiez connaitre OS");
scanf("%s",ipdest);
printf("entrez votre ip");
scanf("%s",ipsource);

host=gethostbyname(ipsource);
if(host==NULL)
{
printf("erreur source host");
exit(1);
}
memcpy(&ip->saddr,host->h_addr,host->h_length);
host=gethostbyname(ipsource);
if(host==NULL)
{
printf("erreur dest host");
exit(1);
}
memcpy(&ip->daddr,host->h_addr,host->h_length);
return(0);
}

int makeDATA(char *data)
{
int compteur=0;
char caractere;
while((caractere=getchar()) != '\n')
{
compteur++;
*data=caractere;
data++;
}
return(compteur);
}

unsigned short calcule(u_short *addr,int len)
{
register int nleft=len;
register int sum=0;
u_short answer=0;
while(nleft>1)
{
sum +=*addr++;
nleft -=2;
}
if(nleft==1)
{
*(u_char *)(&answer)=*(u_char *)addr;
sum +=answer;
}
sum=(sum >>16)+(sum & 0xffff);
sum +=(sum >> 16);
answer = ~sum;
return(answer);
}

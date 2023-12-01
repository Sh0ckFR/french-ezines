/* OrganiKs Crew */
/* Scan via wingate By Lionel */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int connect_tcp(struct in_addr addr,unsigned short port);
int fdprintf(int dafd,char *fmt,...);
struct in_addr victim;

int main (int argc,char **argv)
{
char recvbuf[1024];
int sockfd;
int i,h; 
char *w1 = "kodama.rs-eng.co.jp";

printf("OrganiKs Crew\n");
printf("Scan via wingate By Lionel\n");

if (argc < 4)
{
  printf("Usage: %s <host> <bas port> <haut port> [wingate]\n",argv[0]);
  exit(0);
}
/* adresses wingate */
        if(argc>4)
                w1=argv[4];
printf("connection on %s from %s\n" ,argv[1],w1);
printf("Du port %s à %s\n" ,argv[2],argv[3]);
if (!host_to_ip(w1,&victim))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
/* teste wingate */
if ((sockfd=connect_tcp(victim,23)) < 0)
{
  fprintf(stderr,"service wingate fermé\n");
}
if ((sockfd=connect_tcp(victim,23)) > 0)
{
  printf("service wingate ouvert\n");
}
sleep(2);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}

/* scan de la victime */
for (h=atoi(argv[2]);h<=atoi(argv[3]);h++) {	
if ((sockfd=connect_tcp(victim,23)) > 0)
{
sleep(2);
fdprintf(sockfd,"%s %d\n",argv[1],h);
printf("demande connection sur port %d\n" ,h);
sleep(4);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"Connected")) {
printf("Port %d ouvert\n" ,h);
}
else{
printf("Port %d fermé\n" ,h);
}
close(sockfd);
}
}
printf("Fin du Scaning\n");
}

/* ecrit dans une connection */

int fdprintf(int dafd,char *fmt,...)
{
char mybuffer[4096];
va_list va;

va_start(va,fmt);
vsnprintf(mybuffer,4096,fmt,va);
write(dafd,mybuffer,strlen(mybuffer));
va_end(va);
return(1);
}

/* connextion tcp usage: host port */

int connect_tcp(struct in_addr addr,unsigned short port)
{
struct sockaddr_in serv;
int thesock,flags;

thesock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
bzero(&serv,sizeof(serv));
memcpy(&serv.sin_addr,&addr,sizeof(struct in_addr));
serv.sin_port=htons(port);
serv.sin_family=AF_INET;
if (connect(thesock,(struct sockaddr *)&serv,sizeof(serv)) < 0)
  return(-1);
else
  return(thesock);
}

/* gethostname */
int host_to_ip(char *hostname,struct in_addr *addr)
{
struct hostent *res;

res=gethostbyname(hostname);
if (res==NULL)
  return(0);
memcpy((char *)addr,res->h_addr,res->h_length);
return(1);
}
/* EOF */

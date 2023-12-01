/* OrganiKs Crew */
/* Spoofing Tools via wingates By Lionel */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

void prise(int leesock);
int connect_tcp(struct in_addr addr,unsigned short port);
int fdprintf(int dafd,char *fmt,...);
struct in_addr victim;

int main (int argc,char **argv)
{
char recvbuf[1024];
int sockfd;
int i; 

printf("OrganiKs Crew\n");
printf("Spoof connection tools via wingate By Lionel\n");

if (argc != 4)
{
  printf("Usage: %s <wingate> <host> <port>\n",argv[0]);
  exit(0);
}

printf("connection on %s from %s\n" ,argv[2],argv[1]);
if (!host_to_ip(argv[1],&victim))
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
sleep(2);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* connection sur victime */
if ((sockfd=connect_tcp(victim,23)) > 0)
{
printf("connection sur wingate\n");
sleep(2);
fdprintf(sockfd,"%s %s\n",argv[2],argv[3]);
printf("envoie demande de connection au wingate\n");
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
prise(sockfd);
}
}

/* prise de connection */

void prise(int lesock)
{
int n;
char recvbuf[1024];
fd_set rset;

while (1)
{
  FD_ZERO(&rset);
  FD_SET(lesock,&rset);
  FD_SET(STDIN_FILENO,&rset);
  select(lesock+1,&rset,NULL,NULL,NULL);
  if (FD_ISSET(lesock,&rset))
  {
    n=read(lesock,recvbuf,1024);
    if (n <= 0)
    {
      printf("Connection fermée\n");
      exit(0);
    }
    recvbuf[n]=0;
    printf("%s",recvbuf);      
  }
  if (FD_ISSET(STDIN_FILENO,&rset))
  {
    n=read(STDIN_FILENO,recvbuf,1024);
    if (n>0)
    {
      recvbuf[n]=0;
      write(lesock,recvbuf,n);
    }
  }
}
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

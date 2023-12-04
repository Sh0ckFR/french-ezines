/* OrganiKs Crew */
/* Spoofing Tools via wingates v.2.0 By Lionel */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

void prise(int lesock);
int connect_tcp(struct in_addr addr,unsigned short port);
int fdprintf(int dafd,char *fmt,...);
struct wing {
  char *name;
};
struct in_addr victim;
struct in_addr victim2;
struct in_addr victim3;
struct in_addr victim4;
struct in_addr victim5;
struct in_addr victim6;

int main (int argc,char **argv)
{
char recvbuf[1024];
int sockfd;
int i;
char *w1 = "altona.lnk.telstra.net",*w2 = "mipox.vip.best.com";
char *w3 = "kodama.rs-eng.co.jp",*w4 = "l2tp-178.awalnet.net.sa";
char *w5 = "195.46.19.68",*w6 = "dns.gincorp.co.jp";

printf("OrganiKs Crew\n");
printf("Spoof connection tools via wingates v2.0 By Lionel\n");

if (argc < 3)
{
  printf("Usage: %s host port <w1> <w2> <w3> <w4> <w5> <w6>\n",argv[0]);
  exit(0);
}

/* adresses wingates */
        if(argc>3)
                w1=argv[3];
        if(argc>4)
                w2=argv[4];
        if(argc>5)
                w3=argv[5];
        if(argc>6)
                w4=argv[6];
        if(argc>7)
                w5=argv[7];
        if(argc>8)
                w6=argv[8];

printf("connection on %s from %s/%s/%s/%s/%s/%s\n" ,argv[1],w1,w2,w3,w4,w5,w6);
/* lookup */
if (!host_to_ip(w1,&victim))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
if (!host_to_ip(w2,&victim2))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
if (!host_to_ip(w3,&victim3))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
if (!host_to_ip(w4,&victim4))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
if (!host_to_ip(w5,&victim5))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}
if (!host_to_ip(w6,&victim6))
{
  fprintf(stderr,"Erreur de resolution hostname\n");
  exit(0);
}

/* ------------------------------------------------------------- */
/* teste wingate 1 */
if ((sockfd=connect_tcp(victim,23)) < 0)
{
  fprintf(stderr,"service wingate 1 fermé\n");
}
if ((sockfd=connect_tcp(victim,23)) > 0)
{
  printf("service wingate 1 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* teste wingate 2 */
if ((sockfd=connect_tcp(victim2,23)) < 0)
{
  fprintf(stderr,"service wingate 2 fermé\n");
}
if ((sockfd=connect_tcp(victim2,23)) > 0)
{
  printf("service wingate 2 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* teste wingate 3 */
if ((sockfd=connect_tcp(victim3,23)) < 0)
{
  fprintf(stderr,"service wingate 3 fermé\n");
}
if ((sockfd=connect_tcp(victim3,23)) > 0)
{
  printf("service wingate 3 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* teste wingate 4 */
if ((sockfd=connect_tcp(victim4,23)) < 0)
{
  fprintf(stderr,"service wingate 4 fermé\n");
}
if ((sockfd=connect_tcp(victim4,23)) > 0)
{
  printf("service wingate 4 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* teste wingate 5 */
if ((sockfd=connect_tcp(victim5,23)) < 0)
{
  fprintf(stderr,"service wingate 5 fermé\n");
}
if ((sockfd=connect_tcp(victim5,23)) > 0)
{
  printf("service wingate 5 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* teste wingate 6 */
if ((sockfd=connect_tcp(victim6,23)) < 0)
{
  fprintf(stderr,"service wingate 6 fermé\n");
}
if ((sockfd=connect_tcp(victim6,23)) > 0)
{
  printf("service wingate 6 ouvert\n");
sleep(2);
bzero(recvbuf,1024);
read(sockfd,recvbuf,1024);
if (strstr(recvbuf,"WinGate>")) {
printf("Bon wingate\n");
}
else{
printf("Mauvais wingate\n");
}
}
/* -------------------------------------------------------------- */
/* connection sur victime */
if ((sockfd=connect_tcp(victim,23)) > 0)
{
printf("connection sur wingate\n");
sleep(2);
printf("envoie demande de connection sur wingate 2\n");
fdprintf(sockfd,"%s 23\n",w2);
sleep(5);
printf("envoie demande de connection sur wingate 3\n");
fdprintf(sockfd,"%s 23\n",w3);
sleep(8);
printf("envoie demande de connection sur wingate 4\n");
fdprintf(sockfd,"%s 23\n",w4);
sleep(10);
printf("envoie demande de connection sur wingate 5\n");
fdprintf(sockfd,"%s 23\n",w5);
sleep(13);
printf("envoie demande de connection sur wingate 6\n");
fdprintf(sockfd,"%s 23\n",w6);
sleep(15);
printf("envoie demande de connection sur l'host\n");
fdprintf(sockfd,"%s %s\n" ,argv[1],argv[2]);
sleep(15);
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

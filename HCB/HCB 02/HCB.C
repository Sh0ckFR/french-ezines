#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>

#define  _SERV "127.0.0.1"
#define  PORT  12345

static void myip(t)
char *t;
{
 printf("adress: %u,%u,%u,%u\n", t[0]&0xFF,t[1]&0xFF,t[2]&0xFF,t[3]&0xFF);
}


void main()
{
 int port,sockfd,newfd,size,rcv,total=0;
 char *buff;
 char msg[]= "\t\t\033[1;32m Bienvenue dans la surveillance de connexion\033[0m\n";
 time_t td;

 struct sockaddr_in  serv_addr;
 struct sockaddr_in  remote; 
 struct hostent      *server;

 port = PORT;
 server = gethostbyname(_SERV);
 if ( !server)
 {
  fprintf(stderr, "can't reseolve\"%s\"\n",_SERV);
  exit(1);
}

sockfd = socket(AF_INET,SOCK_STREAM,0);
bzero (&serv_addr,sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy(server->h_addr,&serv_addr.sin_addr.s_addr,server->h_length);
serv_addr.sin_port = htons(port);


if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
{
  perror("socket");  exit(1);
}


if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
{
  perror("bind"); exit(1);
}

if(listen(sockfd,5) == -1)
{
  perror("listen"); exit(1);
}

size = sizeof(struct sockaddr_in);


while(1)
{
 if((newfd=accept(sockfd,(struct sockaddr *)&remote,&size)) == -1)
 {
   perror("accept") ;
 }

system("clear");
time(&td);
printf("port open at: %s", ctime(&td));
printf("\nport number : %d\n", ntohs(serv_addr.sin_port));
printf("\nip start at : %s\n",server->h_name);
printf("total :%d \n",total);



for (;;)
{
 send(newfd,msg,sizeof(msg), 0);
 break;
 size = sizeof(remote);
 rcv = recv(newfd,buff,size, 0);

 if(rcv<0){ 
   perror("recvfrom:"); 
   exit(1);
 }else{
 total += rcv;
 printf("%d characters from ");
 myip(&serv_addr.sin_addr);

 break;
 close(newfd);

 }
}

}
}

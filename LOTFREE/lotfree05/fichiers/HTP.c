#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

char * GetArgument(char * commande){
  return (char*)(rindex(commande,':')+1);
}

char * toUpperCase(char * x){
  char *y=x;
  while(*y!='\0'){
    if (*y>96 && *y<123){*y=(char)(*y-32);}
    y++;
  }
}

char * toLowerCase(char * x){
  char *y=x;
  while(*y!='\0'){
    if(*y>64 && *y<91){*y=(char)(*y+32);}
    y++;
  }
}

int main(int argc, char **argv){
  char * buffer, *body, * argument;
  int OnContinue=1;
  int ok=0;
  int taille;

  if(argc!=2){printf("Usage: %s <pseudo_du_hacker>\n",argv[0]);exit(0);}

  buffer=(char*)malloc(80);
  argument=(char*)malloc(30);
  
  while(OnContinue==1){
    
    buffer=0;
    //argument=0;
    
    
    fflush(stdout);
    getline(&buffer,&taille,stdin);
    
    //buffer[strlen(buffer)-1]=0;
    buffer[strlen(buffer)-2]=0;//telnet
    if(*buffer=='\0')OnContinue=0;
    else{
      toLowerCase(buffer);
      
      
      if (strncmp(buffer,"host",3)==0){
	strcpy(argument,GetArgument(buffer));
	ok=1;      
      }
    }
    
    
  }

  printf("HTTP/1.1 200 OK\x0d\x0a");
  printf("Content-Type: text/html\x0d\x0a");
  if(!ok)asprintf(&body,"<h1><center>Hacked by %s</center></h1>",argv[1]);
  else asprintf(&body,"<center><h1>%s 0wns %s</h1></center>",argv[1],argument);
  printf("Content-Length: %d\x0d\x0a\x0d\x0a",strlen(body));
  printf("%s",body);
  free(buffer);
  free(argument);  

}

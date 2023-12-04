#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "lss_util.h"

#define TAILLE_MAX 200

void fin_de_service(int signum) {
  int pid, status;
  while ((pid=waitpid(WAIT_ANY,&status,WNOHANG))>0){}
}

int main(int argc, char * argv[]){
  unsigned short port=8080;
  int sock; // descripteur de la socket
  int num; // descripteur de fichier
  struct sigaction action;
  char * buffer;
  char * welcome,* service;
  
  if(argc!=2){printf("Usage: %s <pseudo_du_hacker>\n",argv[0]);exit(0);}
  
  
  // entre standard : 0, sortie standard : 1
  
  if (num=fork())return 0;
  
  setsid();
  
  /* gestion du SIGCHLD par fin_de_service sans mise en erreur des entr�es */
  sigaction(SIGCHLD,0,&action); /* lecture de l'�tat actuel */
  action.sa_handler=fin_de_service; /* changement de gestionnaire */
  action.sa_flags|=SA_RESTART; /* rajout d'un param�tre */
  sigaction(SIGCHLD,&action,0); /* re�criture de l'�tat */
  
  sock=ouvre_inet_stream_socket(INADDR_ANY,port); /* le syst�me d�terminera l'adresse tout seul INADDR_ANY */
  /* ouvre une socket reseau, protocole TCP */
  
  if (listen(sock,5)) { // on attend une connexion, pas plus de 5 dans la file d'attente

    exit(1);
  }
  
  
  while(1){ // boucle multi-clients
    struct sockaddr_in nom_client;
    int taille=sizeof(struct sockaddr_in), connexion;
    if ((connexion=accept(sock,(struct sockaddr *)&nom_client,&taille))<0) {}
    else { // quelqu'un s'est connecte
      if (!(num=fork())) { // le fils gere le client, le pere continue d'attendre des connexions
	char * hostname, * ip;
	unsigned short port;
	close(sock);
	decode_iaddr(&hostname,&ip,&port,nom_client);
	dup2(connexion,STDIN_FILENO);
	dup2(connexion,STDOUT_FILENO);
	close(connexion);
 	execl("./HTP","HTP",argv[1],0);
	exit(1);
      }
      close(connexion);
    }
  } // fin boucle multi-clients
}

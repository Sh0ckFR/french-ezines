/**************************************************************************
Cette fonction ecoute sur une socket et renvoie PONG quand elle recoit PING
**************************************************************************/

#include <stdio.h>      /* sprintf */
#include <unistd.h>     /* read */
#include <sys/socket.h> /* socket */


#define MAX_SIZE 512
#define CMD_SIZE 16
#define SVR_SIZE 64

void pong(int sock)
{
    /* un buffer qui va contenir les messages (question et reponse) */
    char buff[MAX_SIZE];
    /* Un petit buffer juste pour la commande (PING) */
    char commande[CMD_SIZE];
    /* Et un autre pour le nom du serveur qui a envoye le PING */
    char serveur[SVR_SIZE];
    /* On lit sur la socket */
    read(sock, buff, MAX_SIZE-1);
    /* On separe la commande et le nonm de serveur */
    sscanf(buff, "%s %s", commande, serveur);
    /* On regarde si c'est un PING */
    if(!strcmp(commande, "PING")) {
	/* On prepare le PONG */
	sprintf(buff, "PONG :%s\n", serveur);
	/* Et on l'envoie ! */
	send(sock, buff, sizeof(buff), 0);
    }
}

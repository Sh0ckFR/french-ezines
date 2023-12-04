/************************************************************************
Ce programme est un simple exemple de bot qui reste connecte a un serveur
************************************************************************/

#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* sockaddr_in, in_addr */
#include <netdb.h>      /* gethostbyname */
#include <fcntl.h>      /* fcntl */ 
#include <stdio.h>      /* sprintf */
#include <unistd.h>     /* read */

/**********************************************************
Envoie les infos necessaires pour se logguer au serveur IRC
**********************************************************/ 

/* On passe le numero de socket comme argument */
void log_in(int sock)
{
    /* On utilise un petit buffer */
    char buff[512];
    /* Le format est : USER <username> <hostname> <servername> <realname>
                       NICK <nickname> */
    sprintf(buff, "NICK Zebot\nUSER fred . . NULL\nJOIN #toto\n");
    /* Et on envoie les trois commandes d'un coup */
    send(sock, buff, strlen(buff), 0);
}

/**************************************************************************
Cette fonction ecoute sur une socket et renvoie PONG quand elle recoit PING
**************************************************************************/

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
    /* Il faudrait verifier qu'on a bien le \n a la fin des donnees */
    read(sock, buff, MAX_SIZE-1);
    /* On separe la commande et le nonm de serveur */
    sscanf(buff, "%s %s", commande, serveur);
    /* On regarde si c'est un PING */
    if(!strcmp(commande, "PING")) {
	/* On prepare le PONG (ne pas oublier le \n) */
	sprintf(buff, "PONG %s\n", serveur);
	/* Et on l'envoie ! */
	send(sock, buff, strlen(buff), 0);
    }
}

/*******************************************************************************
Le programme principal ouvre une socket, lance le log_in et fait tourner le PONG
en boucle
********************************************************************************/

#define SERVNAME "tibob.via.ecp.fr"

int main()
{
    /* Les sockets sont reperees par un entier */
    int sock;
    /* hostent est une structure predefinie dans laquelle on peut stocker les infos sur une machine : nom, adresse IP, etc... */
    struct hostent *h;
    /* structure dans laquelle on stocke des infos sur la connexion a etablir */
    struct sockaddr_in sockname;

    /* Notre socket sera AF_INET car cela correspond aux protocoles ARPA utilises sur Internet. Ce protocole sera plus precisement TCP, ce que le specifie par SOCK_STREAM. */
    if((sock = socket(AF_INET, SOCK_STREAM,0))<0) {
	/* Si l'ouverture de la socket echoue, on abandonne */
	return(-1);
    }
    /* On va maintenant tenter d'obtenir les infos necessaires dans hostent grace a une resolution DNS */
    if((h = gethostbyname(SERVNAME))==NULL) {
	/* La resolution a echoue */
	/* On essaie donc par IP, au cas ou SERVNAME serait une IP et non pas un nom DNS */
	if((h = gethostbyaddr(SERVNAME, sizeof(SERVNAME), AF_INET))==NULL)
{
	    /* La resolution par IP a echoue aussi, alors on abandonne. */
	    return(-1);
	}
    }
    /* Maintenant la structure hostent (h) a ete remplie, soit par gethostbyname, soit par gethostbyaddr */

    /* On va maintenant remplir la structure sockaddr_in, qui ressemble a hostent, mais en plus complete. */
    /* On recupere les infos sur l'hote stockees dans h */
    memcpy(&sockname.sin_addr.s_addr, h->h_addr, h->h_length);
    /* Et on complete avec des infos liees a la connexion */
    sockname.sin_family=AF_INET;
    sockname.sin_port=htons(6667); /* htons est necessaire, pour des histoires de facon d'ordonner les octets */

    /* Le grand moment est enfin arrive, on se connecte ! */
    /* Notons que connect prend comme argument une structure sockaddr, plus simple que sockaddr_in, mais compatible */
    if(connect(sock, (struct sockaddr *)&sockname, sizeof(sockname))<0) {
	/* Echec et mat */
	return(-1);
    }

    /* On essaie de rendre la socket non bloquante */
    fcntl(sock,F_SETFL,O_NONBLOCK);

    /* On se log */
    log_in(sock);

    /* Et on repond aux pings jusqu'a plus soif */
    while(1)
        pong(sock);
    /* Normalement on arrive jamais la */
    return (0);
}

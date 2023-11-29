#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include "libipq/libipq.h"

#define BUFSIZE 65536
unsigned char buf[BUFSIZE];

u_short in_chksum(u_short *ptr, int nbytes)
{
  register long           sum;            /* assumes long == 32 bits */
  u_short                 oddbyte;
  register u_short        answer;         /* assumes u_short == 16 bits */
  sum = 0;
  while (nbytes > 1)
  {
    sum += *ptr++;
    nbytes -= 2;
  }
  if (nbytes == 1)
  {
     oddbyte = 0;            /* make sure top half is zero */
     *((u_char *) &oddbyte) = *(u_char *)ptr;   /* one byte only */
     sum += oddbyte;
  }
  sum  = (sum >> 16) + (sum & 0xffff);    /* add high-16 to low-16 */
  sum += (sum >> 16);                     /* add carry */
  answer = ~sum;          /* ones-complement, then truncate to 16 bits */                                            
  return((u_short) answer);
  }


void scan_queue()
{
	int len,status,loop,nplen;
	struct ipq_handle *h;
	ipq_packet_msg_t *m;

	/* Initialisation de la connexion */
	h = ipq_create_handle(0, PF_INET);
	if (!h)
		return (0);
	status = ipq_set_mode(h, IPQ_COPY_PACKET, BUFSIZE);
	if (status < 0)
		return (0);

	do {
		len = ipq_read(h, buf, BUFSIZE, 0);
	        
                switch (ipq_message_type(buf)) {

	        case IPQM_PACKET:
			m = ipq_get_packet(buf);
			struct iphdr *iph = ((struct iphdr *)m->payload);
                                    
				    
				    iph->tos=0; 
				    memset(&iph->check,0,2);
				    iph->check=in_chksum((u_short *)iph,iph->ihl*4);
				    status = ipq_set_verdict(h, m->packet_id, NF_ACCEPT, 
						 m->data_len, m->payload); 
                                    break;

		default:
			
			break;
		}
	} while (loop);
	ipq_destroy_handle(h);
}


int main(int argc, char **argv)
{
scan_queue();
return(0);
}


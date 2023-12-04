#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <linux/if.h>  
#include <stdio.h> 
#include <arpa/inet.h> 
#include <linux/socket.h> 
#include <linux/ip.h> 
#include <linux/icmp.h> 
#include <linux/if_ether.h>
#include <sys/ioctl.h> 
#include <sys/stat.h> 
#include <fcntl.h> 

int sconnect(char *device)
{ 
  int fd; 
	 int s; 
	 fd=socket(AF_INET, SOCK_PACKET, htons(ETH_P_ALL)); 
	if(fd < 0) 
	{ 
		perror("cant get SOCK_PACKET socket"); 
		exit(0); 
	} 
  return fd; 
} 

int checkos(int mytos) /* pas sur */
{ 
  int sock, octets_recus, lendata, j, i=0;
  unsigned char *so, *dest;
  struct recvpacquet       
  { 
    struct ethhdr eth; 
    struct iphdr  ip; 
    struct icmphdr icmp; 
    char data[8000]; 
  } buffer; 
  
  struct iphdr *ip;        
  struct icmphdr *icmp;      
  char *data;              
  
  ip=(struct iphdr *)(((unsigned long)&buffer.ip)-2); 
  icmp=(struct icmphdr *)(((unsigned long)&buffer.icmp)-2); 
  
  
  so = (unsigned char *)&(ip->saddr);   /* Utiliser pour affciher les adresses ip */
  dest = (unsigned char *)&(ip->daddr); /*      "         "              "      " */
  
  sock = sconnect("eth0");
  sendpacket(); 
  while(1) 
    {  
    octets_recus = read(sock, (struct recvpacquet *)&buffer, sizeof(struct recvpacquet));          
      if(ip->protocol==1 && icmp->type==0) /* paquet echo reply (icmp;)*/
{
if(ip->tos==0)
{
printf("l'os de la machine est windows \n");
}
else if(ip->tos==mytos)
{
  printf("l'os de la machine est Linux \n");
}
else{ printf("unknow os\n");}
break;
}
     
     }
 }
 
 int main(){
 checkos(15);
 return(0);
 }
     

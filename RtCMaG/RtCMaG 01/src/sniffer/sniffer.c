/* Exemple de sniffer Basic coder par RaPass <rapasss@multimania.com>*/

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <linux/if.h>  
#include <stdio.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <linux/socket.h> 
#include <linux/ip.h> 
#include <linux/tcp.h> 
#include <linux/if_ether.h>
#include <sys/ioctl.h> 
#include <sys/stat.h> 
#include <ctype.h>
#include <fcntl.h> 

int setup_interface(char *device) /*met la carte reseau en mode promiscuous */
{ 
  int fd; 
	struct ifreq ifr; 
	int s; 
	 
	//open up our magic SOCK_PACKET 
	fd=socket(AF_INET, SOCK_PACKET, htons(ETH_P_ALL)); 
	if(fd < 0) 
	{ 
		perror("cant get SOCK_PACKET socket"); 
		exit(0); 
	} 
 
	//set our device into promiscuous mode 
	strcpy(ifr.ifr_name, device); 
	s=ioctl(fd, SIOCGIFFLAGS, &ifr); 
	if(s < 0) 
	{ 
		close(fd); 
		perror("cant get flags"); 
		exit(0); 
	} 
	ifr.ifr_flags |= IFF_PROMISC; 
	s=ioctl(fd, SIOCSIFFLAGS, &ifr); 
	if(s < 0) perror("cant set promiscuous mode"); 
	return fd; 
} 

int main() 
{ 
  int sock, datasize, i, j;
  unsigned char *so, *dest;
  char paquet[4096]; /* c''est la dedans qu'on va tout mettre ce que l'on va recevoir */ 
  struct iphdr  *ip; 
  struct tcphdr *tcp; 
  char *data; 
  
  ip=(struct iphdr *)(paquet + sizeof(struct ethhdr)); /* pointeur sur l'entetete ip */
  tcp=(struct tcphdr *)(paquet + sizeof(struct ethhdr) + sizeof(struct iphdr));/*pointeur sur l'entete tcp */
  
  
  so = (unsigned char *)&(ip->saddr);   /* Utiliser pour affciher les adresses ip */
  dest = (unsigned char *)&(ip->daddr); /*      "         "              "      " */
  
  sock = setup_interface("eth0");/* mise en mode promiscuous de la carte reseau eth0*/

  for (i=1 ; ; i++) /*Il aurait fallut gerer les signaux pour fermer la socket en partant :( */
    {  
      read(sock, (char *)&paquet, 4096);
      if (ip->protocol != IPPROTO_TCP)  /* SI c'est pas le protocol TCP, on passe notre route*/
	continue;
      printf("\n-------Packet %d---------\n", i);         
      printf("Adresse ::: %u.%u.%u.%u -----> %u.%u.%u.%u\n", /* On affcihe les adresses ips */
                                   so[0],so[1],so[2],so[3],
                           dest[0],dest[1],dest[2],dest[3]); 
      printf("Port ::: %d  -------> %d\n",ntohs(tcp->source), ntohs(tcp->dest)); /*les port (source et dest) */
      printf("TTL ::: %d\n", ip->ttl);  /* TTL(Time To Live) */
      printf("Flags ::: SYN=%d | ACK=%d | RST=%d | FIN=%d\n",tcp->syn, tcp->ack, tcp->rst, tcp->fin);/* qqs flags */
   
      /* Ce pointeur pointe sur les donnees */
      data = (char *) (paquet+sizeof(struct ethhdr)+sizeof(struct iphdr)+sizeof(struct tcphdr)+12);
      /* On calcule la taille des donnes en fonction du champs tot_len du paquet IP */
      datasize = ntohs(ip->tot_len) -sizeof(struct iphdr)-sizeof(struct tcphdr)-12;
      
      /* Si il y a des donnees, on les affiche */
      if (datasize > 0) {
	printf("DATA[%d] ::: \n", datasize);
	for(j=0 ; j < datasize; j++) {
	  if (isprint(data[j])) 
	    putchar(data[j]); 
	  else 
	    putchar(data[j]);
	}	
   }      
  }
  /* Si vous voulez utilisez ce programme, il faudra penser a fermer la socket !! */
} 
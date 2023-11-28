#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

uint32_t adresse_ip(const char * hostname) {
  struct hostent * info=gethostbyname(hostname);
  if (!info) {
    perror();
    exit(1);
  }
  return ntohl(* (uint32_t *)info->h_addr);
}

void decode_iaddr(char ** phost, char ** pip, unsigned short * pp,
		  struct sockaddr_in nom) {
  union {
    uint32_t unb;
    unsigned char qnb[4];
  } aip; 
  struct hostent * info=gethostbyaddr((char *)&nom.sin_addr.s_addr,4,AF_INET);
  if (info) asprintf(phost,"%s",info->h_name);
  else asprintf(phost,"%s","(inconnu)");
  *pp=ntohs(nom.sin_port);
  aip.unb=ntohl(nom.sin_addr.s_addr);
  asprintf(pip,"%d.%d.%d.%d", aip.qnb[3], aip.qnb[2], aip.qnb[1], aip.qnb[0]);
}

struct sockaddr_in iaddr(uint32_t ip, unsigned short port) {
  struct sockaddr_in nom;
  nom.sin_family=AF_INET;
  nom.sin_addr.s_addr=htonl(ip);
  nom.sin_port=htons(port);
  return nom;
}

int ouvre_inet_stream_socket(uint32_t ip, unsigned short port) {
  struct sockaddr_in nom=iaddr(ip,port);
  int sock=socket(PF_INET,SOCK_STREAM,0);
  if (sock<0) {
    perror("création socket ");
    exit(1);
  }
  if (bind(sock,(struct sockaddr *) &nom,sizeof(nom))) {
    perror("assignation socket ");
    exit(1);
  }
  return sock;
}

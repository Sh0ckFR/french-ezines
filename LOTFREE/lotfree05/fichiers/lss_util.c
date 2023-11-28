#include <sys/socket.h>
#include <sys/un.h>

struct sockaddr_un laddr(const char * sockname) {
  struct sockaddr_un nom;
  nom.sun_family=AF_LOCAL;
  strncpy(nom.sun_path,sockname,sizeof(nom.sun_path));
  return nom;
}

int ouvre_local_stream_socket(const char * sockname) {
  struct sockaddr_un nom=laddr(sockname);
  int sock=socket(PF_LOCAL,SOCK_STREAM,0);
  if (sock<0) {
    perror("création socket ");
    exit(1);
  }
  if (bind(sock,(struct sockaddr *)&nom,SUN_LEN(&nom))) {
    perror("assignation socket ");
    exit(1);
  }
  return sock;
}

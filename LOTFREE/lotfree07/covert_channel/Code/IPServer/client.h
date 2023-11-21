#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/tcp.h>

void forgepacket(unsigned int, unsigned int, unsigned short, unsigned short, char *,
					  int);
					  
unsigned int host_convert(char *);
unsigned short in_cksum(unsigned short *, int);
void usage(char *);


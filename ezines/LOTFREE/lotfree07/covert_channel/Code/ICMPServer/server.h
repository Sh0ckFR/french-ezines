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
#include <linux/icmp.h>

void forgepacket(unsigned int, unsigned int, char *);
					  
unsigned int host_convert(char *);
void usage(char *);

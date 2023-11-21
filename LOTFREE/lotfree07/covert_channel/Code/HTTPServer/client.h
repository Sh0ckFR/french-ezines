#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 31337 /* the port client will be connecting to */
#define MAXDATASIZE 1024 /* max number of bytes we can get at once */

char *CreateQuery(char *, char *, int);

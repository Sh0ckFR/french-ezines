#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MYPORT 31337    /* the port users will be connecting to */
#define BACKLOG 10     /* how many pending connections queue will hold */
#define MAXDATASIZE 2048

int ParseResp(char *);
char *CreateQuery(char *);

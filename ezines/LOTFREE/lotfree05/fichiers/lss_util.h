#ifndef LSS_UTIL_H
#define LSS_UTIL_H

struct sockaddr_un laddr(const char *);
int ouvre_local_stream_socket(const char *);

#endif

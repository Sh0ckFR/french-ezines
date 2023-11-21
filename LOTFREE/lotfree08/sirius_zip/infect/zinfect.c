#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/* Les autres fichiers sont dans le repertoire zinfect */
#include "zip.h"

/* dans cet exemple la plupart des valeurs sont en dur */
#define FORTUNE "Le vrai moyen d'être trompé, c'est de se croire plus fin que les autres.\n\
\t-+- François de La Rochefoucauld (1613-1680), Maximes 127 -+-\n"
#define FILE_NAME "fortune2.txt"
#define TMP_FILE ".infect.zip"
#define VICTIM "fortune.zip"

int main(int argc,char *argv[])
{
  struct lfh z;
  struct lfh head;
  struct cdir cd;
  struct cdir cds;
  struct eocdir eocd;
  int fdin, fdout;
  unsigned long sign;
  char *s;
  char buf[CDIR_LEN+1];
  unsigned long reste, x;
  unsigned long pos_lfh;
  
  /* On remplit les structures de l'entrée à rajouter */
  z.version=cd.vmadeby=cd.vneeded=10; /* v1.0 */
  z.gpf=cd.gpf=0;                     /* pas de cryptage... */
  z.comp=cd.comp=0;                   /* pas de compression */
  z.time=cd.time=22963;
  z.date=cd.date=12899;
  z.crc=cd.crc=0x3c253319L;           /* ! le CRC doit être exact ! */
  z.usize=cd.usize=sizeof(FORTUNE)-1;
  z.zsize=cd.zsize=sizeof(FORTUNE)-1;
  z.name_len=cd.name_len=strlen(FILE_NAME);
  z.xtra_len=cd.xtra_len=0;

  cd.comm_len=0;  /* pas besoin de s'emmerder avec les champs optionnels */
  cd.disk_num=0;
  cd.int_attr=1;  /* 1 pour ascii, 0 pour binaire */
  cd.ext_attr=32; /* euh pas le courage de savoir pourquoi */

  /* Cette variable contiendra l'ofset où injecter le Local File Header */
  pos_lfh=0L;
  fdin=open(VICTIM,O_RDONLY);
  fdout=creat(TMP_FILE,S_IRUSR|S_IWUSR);
  while(1)
  {
    /* On lit 4 octets (taille d'une signature zip) */
    if((x=read(fdin,buf,sizeof(LFH_SIGN)))==-1)
    {
      exit(1);
    }
    bcopy(buf,&sign,sizeof(LFH_SIGN));
    /* S'il s'agit d'un LFH ou d'une structure Special Spanning : copie brute */
    if(sign==LFH_SIGN || sign==SPSPAN_SIGN)
    {
      write(fdout,&sign,sizeof(LFH_SIGN));
      if(sign==LFH_SIGN)
      {
	if((x=read(fdin,buf,LFH_LEN-sizeof(LFH_SIGN)))==-1)
	{
	  perror("read");
	  exit(1);
	}
	/* On doit lire les headers pour gérer les noms de fichiers etc */
	buff2lfh(buf,&head);
	write(fdout,buf,x);
	if(head.name_len!=0)
	{
	  s=(char*)malloc(head.name_len);
	  if(read(fdin,s,head.name_len)==-1)
	  {
	    perror("read");
	    exit(1);
	  }
	  write(fdout,s,head.name_len);
	  free(s);
	}
	if(head.xtra_len!=0)
	{
	  s=(char*)malloc(head.xtra_len);
	  if(read(fdin,s,head.xtra_len)==-1)
	  {
	    perror("read");
	    exit(1);
	  }
	  write(fdout,s,head.xtra_len);
	  free(s);
	}
	if(head.zsize!=0)
	{
	  reste=head.zsize;
	  while((x=read(fdin,buf,(reste>sizeof(buf))?sizeof(buf):reste)))
	  {
	    reste-=x;
	    write(fdout,buf,x);
	  }
	}
      }
      /* Special Spanning : 12 octets de données */
      else
      {
	read(fdin,buf,12);
	write(fdout,buf,12);
      }
    }
    else
    {
      /* Pour savoir si on a déjà infecté le LFH */
      if(!pos_lfh)
      {
        /* On injecte notre header + filename + data */
	pos_lfh=lseek(fdin,0,SEEK_CUR)-4;
	writelfh(fdout,z);
	write(fdout,FILE_NAME,strlen(FILE_NAME));
	write(fdout,FORTUNE,sizeof(FORTUNE)-1);
      }
      if(sign==CDIR_SIGN)
      {
        /* recopie... */
	write(fdout,&sign,sizeof(CDIR_SIGN));
	if((x=read(fdin,buf,CDIR_LEN-sizeof(CDIR_SIGN)))==-1)
	{
	  perror("read");
	  exit(1);
	}
	buff2cdir(buf,&cds);
	write(fdout,buf,x);
	if(cds.name_len!=0)
	{
	  s=(char*)malloc(cds.name_len);
	  if(read(fdin,s,cds.name_len)==-1)
	  {
	    perror("read");
	    exit(1);
	  }
	  write(fdout,s,cds.name_len);
	  free(s);
	}
	if(cds.xtra_len!=0)
	{
	  s=(char*)malloc(cds.xtra_len);
	  if(read(fdin,s,cds.xtra_len)==-1)
	  {
	    perror("read");
	    exit(1);
	  }
	  write(fdout,s,cds.xtra_len);
	  free(s);
	}
	if(cds.comm_len!=0)
	{
	  s=(char*)malloc(cds.comm_len);
	  if(read(fdin,s,cds.comm_len)==-1)
	  {
	    perror("read");
	    exit(1);
	  }
	  write(fdout,s,cds.comm_len);
	  free(s);
	}
      }
      else if(sign==EOCDIR_SIGN) /* c'est le moment d'insérer notre cdir */
      {
	cd.roffset=pos_lfh;
	writecdir(fdout,cd);
	write(fdout,FILE_NAME,strlen(FILE_NAME));
	if((x=read(fdin,buf,EOCDIR_LEN-sizeof(EOCDIR_SIGN)))==-1)
	{
	  perror("read");
	  exit(1);
	}
	/* Le plus dur du code... */
	buff2eocdir(buf,&eocd); /* On décode le header */
	eocd.nb_cd++;           /* On incrémente le nombre d'entrées */
	eocd.nb_ent_cd++;       /* pareil */
	eocd.cd_size+=CDIR_LEN+strlen(FILE_NAME); /* 46 + longeur nom fichier */
	/* L'offset du Central Directory a changé... on le met à jour */
	eocd.first_disk+=LFH_LEN+strlen(FILE_NAME)+sizeof(FORTUNE)-1;
	/* On écrit le nomveau EOCDIR */
	writeeocdir(fdout,eocd);
	break;
      }
      else /* l'appot con pris ! (un header qu'on ne gère pas) */
      {
	break;
      }
    }
  }
  /* Terminus, tout le monde descend */
  close(fdout);
  close(fdin);

  /* On remplace le fichier original par la version infectée */
  unlink(VICTIM);
  link(TMP_FILE,VICTIM);
  unlink(TMP_FILE);
  return 0;
}

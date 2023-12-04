#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#include "zip.h"

void buff2lfh(char *s,struct lfh *z)
{
  z->version=SH(s);
  z->gpf=SH(s+2);
  z->comp=SH(s+4);
  z->time=SH(s+6);
  z->date=SH(s+8);
  bcopy(s+10,&(z->crc),4);
  bcopy(s+14,&(z->zsize),4);
  bcopy(s+18,&(z->usize),4);
  z->name_len=SH(s+22);
  z->xtra_len=SH(s+24);
}

void buff2cdir(char *s,struct cdir *cd)
{
  cd->vmadeby=SH(s);
  cd->vneeded=SH(s+2);
  cd->gpf=SH(s+4);
  cd->comp=SH(s+6);
  cd->time=SH(s+8);
  cd->date=SH(s+10);
  bcopy(s+12,&(cd->crc),4);
  /* cd->crc=SH(s+12); */
  bcopy(s+16,&(cd->zsize),4);
  bcopy(s+20,&(cd->usize),4);
  cd->name_len=SH(s+24);
  cd->xtra_len=SH(s+26);
  cd->comm_len=SH(s+28);
  cd->disk_num=SH(s+30);
  cd->int_attr=SH(s+32);
  bcopy(s+34,&(cd->ext_attr),4);
  bcopy(s+38,&(cd->roffset),4);
}

void buff2eocdir(char *s, struct eocdir *eocd)
{
  eocd->disk_num=SH(s);
  eocd->disk_cd_num=SH(s+2);
  eocd->nb_cd=SH(s+4);
  eocd->nb_ent_cd=SH(s+6);
  bcopy(s+8,&(eocd->cd_size),4);
  bcopy(s+12,&(eocd->first_disk),4);
  eocd->comm_len=SH(s+16);
}

void writelfh(int fd,struct lfh z)
{
  unsigned long ltemp;
  unsigned short stemp;

  ltemp=LFH_SIGN;
  write(fd,&ltemp,4);
  stemp=z.version;
  write(fd,&stemp,2);
  stemp=z.gpf;
  write(fd,&stemp,2);
  stemp=z.comp;
  write(fd,&stemp,2);
  stemp=z.time;
  write(fd,&stemp,2);
  stemp=z.date;
  write(fd,&stemp,2);
  ltemp=z.crc;
  write(fd,&ltemp,4);
  ltemp=z.usize;
  write(fd,&ltemp,4);
  ltemp=z.zsize;
  write(fd,&ltemp,4);
  stemp=z.name_len;
  write(fd,&stemp,2);
  stemp=z.xtra_len;
  write(fd,&stemp,2);
}

void writecdir(int fd,struct cdir cd)
{
  unsigned long ltemp;
  unsigned short stemp;

  ltemp=CDIR_SIGN;
  write(fd,&ltemp,4);

  stemp=cd.vmadeby;
  write(fd,&stemp,2);
  stemp=cd.vneeded;
  write(fd,&stemp,2);
  stemp=cd.gpf;
  write(fd,&stemp,2);
  stemp=cd.comp;
  write(fd,&stemp,2);
  stemp=cd.time;
  write(fd,&stemp,2);
  stemp=cd.date;
  write(fd,&stemp,2);

  ltemp=cd.crc;
  write(fd,&ltemp,4);
  ltemp=cd.usize;
  write(fd,&ltemp,4);
  ltemp=cd.zsize;
  write(fd,&ltemp,4);

  stemp=cd.name_len;
  write(fd,&stemp,2);
  stemp=cd.xtra_len;
  write(fd,&stemp,2);
  stemp=cd.comm_len;
  write(fd,&stemp,2);
  stemp=cd.disk_num;
  write(fd,&stemp,2);
  stemp=cd.int_attr;
  write(fd,&stemp,2);

  ltemp=cd.ext_attr;
  write(fd,&ltemp,4);
  ltemp=cd.roffset;
  write(fd,&ltemp,4);
}

void writeeocdir(int fd,struct eocdir eocd)
{
  unsigned long ltemp;
  unsigned short stemp;

  ltemp=EOCDIR_SIGN;
  write(fd,&ltemp,4);
  
  stemp=eocd.disk_num;
  write(fd,&stemp,2);
  stemp=eocd.disk_cd_num;
  write(fd,&stemp,2);
  stemp=eocd.nb_cd;
  write(fd,&stemp,2);
  stemp=eocd.nb_ent_cd;
  write(fd,&stemp,2);

  ltemp=eocd.cd_size;
  write(fd,&ltemp,4);
  ltemp=eocd.first_disk;
  write(fd,&ltemp,4);
  
  stemp=eocd.comm_len;
  write(fd,&stemp,2);
}

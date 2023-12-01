#include <stdio.h>

void main()
{
	char *nom[2];
	nom[0]="/bin/sh";
	nom[1]=NULL;
	execve(nom[0],nom,NULL);
}
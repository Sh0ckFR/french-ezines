#include <stdio.h>
int main(int argc, char **argv)

	{
char buffer[200];
if(argc>1)
strcpy(buffer,argv[1]);
printf("vuln1 : buffer overflow\n");
	
	} 
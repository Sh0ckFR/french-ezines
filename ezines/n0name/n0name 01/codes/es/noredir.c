#include <stdlib.h>
#include <stdio.h>
#define TTY "/dev/tty"

int
main(int argc, char *argv[])
{
	freopen(TTY, "w", stdout);

	if (argc >1)
	{
		int i;
	
		for (i = 1; i < argc; i++)
			printf("%s", argv[i]);
		printf("\n");
	}
	else
	{
		for(;;)
		{
			int c = getchar();
	
			if ( c == EOF)
				break;
			putchar(c);
		}
	}
	return EXIT_SUCCESS;
}

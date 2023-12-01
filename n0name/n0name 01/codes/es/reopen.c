#include <stdlib.h>
#include <stdio.h>

#define NOM_LOG "execution.log"

int
main(void)
{
	freopen(NOM_LOG, "w", stdout);
	printf("   ) Debut d'execution de reopen  )\n");
	printf("  /                     …                        / \n");
	printf(" ( Fin d'execution de reopen __ ( \n");
	return EXIT_SUCCESS;
}

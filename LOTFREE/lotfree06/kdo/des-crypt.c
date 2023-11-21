#include <stdio.h>  /* pour printf    */
#include <stdlib.h>
#include <unistd.h> /* pour crypt     */
#include <getopt.h> /* pour getopt    */
#include <errno.h>  /* pour perror    */
#include <string.h> /* pour memset    */
#include <signal.h> /* pour sigaction */

#define alpha "abcdefghijklmnopqrstuvwxyz"

char *wordlist=0;
char *hash=0;
int incmode=0;
char *word;
int *ptr;

int strpos(char c)
{
  int i;
  int x=0;
  for(i=0;i<26;i++)if(c==alpha[i])x=i;
  return x;
}

void gestion_signal(int signal_nb)
{
  FILE *restore;
  printf("Mot en cours : %s\n",word);
  if(signal_nb==2){ /* recu SIGINT */
    if(!(restore=fopen("restore.des","w"))){
      perror("Erreur");
    }
    else{
      fputs(word,restore);
      fclose(restore);
    }
    exit(0);
  }
}

usage(char *prog)
{
  printf("Usage: %s [-f dictionnaire | -i] pass_crypté\n\n",prog);
  exit(0);
}

int read_line(FILE *fic,char *word)
{
  int pos=0;
  char c=fgetc(fic);

  while(c!='\n' && c!=EOF){
    if(pos <= 7){
      word[pos]=c;
      pos++;
    }
    if(pos==8)word[pos]='\0';
    c=fgetc(fic);
  }
  if(pos <8)word[pos-1]='\0';
  if(c==EOF)return 0;
  else return 1;

}

int restore()
{
  FILE *fic;
  int i;
  if(!(fic=fopen("restore.des","r"))){
    perror("Erreur");
  }
  else{
    read_line(fic,word);
    printf("Reprise à : %s\n",word);
    for(i=0;i<strlen(word);i++)ptr[i]=strpos(word[i]);
    fclose(fic);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  FILE*dico;
  int opt;
  char *salt;
  int pos;
  int i;
  int r=0;
  struct sigaction sa;

  printf("DE(S)CRYPTEUR v1.0 par sirius_black\n");

  if(argc<=1)usage(argv[0]);
  while((opt = getopt(argc, argv, "F:f:IiRr")) != -1){
    switch(opt){
    case 'F':
    case 'f':
      if(optarg){
	wordlist=(char*)malloc(strlen(optarg)+1);
	strncpy(wordlist,optarg,strlen(optarg));
      }
      break;
    case 'I':
    case 'i':
      incmode=1;
      break;
    case 'R':
    case 'r':
      incmode=1;
      r=1;
      break;
    case '?':
      break;
    default:
      printf("?? getopt returned character code 0%o ??\n",opt);
    }
  }
  if(incmode && wordlist){
    printf("Vous ne pouvez pas choisir les deux modes en même temps\n");
    exit(0);
  }
  if(optind == argc)usage(argv[0]);
  else{
    if(strlen(argv[argc-1])!=13)printf("Ce mot de passe n'est pas encrypé en DES\n");
    else{
      hash=(char*)malloc(strlen(argv[argc-1])+1);
      strncpy(hash,argv[argc-1],strlen(argv[argc-1]));
    }
  }

  /* on a récupéré tous les arguments : wordlist, mode incremental et hash */
  printf("PID= %d\n",getpid());
  salt=(char*)malloc(4);
  salt[0]=hash[0];
  salt[1]=hash[1];
  salt[2]='\0';
  /* attaque par dictionnaire */
  if(!incmode){
    printf("Dictionnary attack\n");
    word=(char*)malloc(256);
    if(!(dico=fopen(wordlist,"r"))){
      perror("Erreur");
    }
    else{
      while(read_line(dico,word)){
	if(!strcmp((char*)crypt(word,salt),hash)){
	  printf("Le mot de passe est: %s\n",word);
	  free(word);
	  free(salt);
	  return 0;
	}
      }
      fclose(dico);
    }
  }
  /* brute force */
  else{
    printf("Brute force attack\n");
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler=gestion_signal;
    sa.sa_flags=0;
    sigemptyset(&(sa.sa_mask));
    if(sigaction(SIGINT,&sa,NULL)!=0)perror("Erreur sur sigaction");
    if(sigaction(SIGUSR1,&sa,NULL)!=0)perror("Erreur sur sigaction");
    word=(char*)malloc(8);
    ptr=(int*)malloc(8*sizeof(int));
    if(r)restore();
    else memset(word,alpha[0],8);
    for(i=0;i<8;i++){
      ptr[i]=0;
    }
    pos=7;

    /* boucle brute force */
    while(1){
      if((unsigned char)ptr[7]>25){
	ptr[7]=0;
	word[7]=alpha[0];
	pos--;
      }
      while(pos!=7){
	ptr[pos]++;
	word[pos]=alpha[ptr[pos]];
	if((unsigned char)ptr[pos]>25){
	  ptr[pos]=0;
	  word[pos]=alpha[0];
	  pos--;
	  if(pos<0)return;
	  continue;
	}
	else pos=7;
      }
      if(!strcmp((char*)crypt(word,salt),hash)){
	printf("Le mot de passe est: %s\n",word);
	free(word);
	free(salt);
	return 0;
      }
      ptr[7]++;
      word[7]=alpha[ptr[7]];
    }

  }
  free(word);
  free(salt);

  return;
}

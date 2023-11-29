typedef struct
   {
    unsigned long int p;
    unsigned long int q;
    unsigned long int n;
    unsigned long int phi;
    unsigned long int d;
    unsigned long int e;
    unsigned long int nb;
   }cle;


unsigned long int puissance(int a, int b);
unsigned long int cryptage(unsigned long int nb, unsigned long int puiss, unsigned long int mod);
unsigned long int decryptage(unsigned long int nb, unsigned long int puiss, unsigned long int mod);
unsigned long int brutus_attack(unsigned long int c, unsigned long int puiss, unsigned long mod);
cle cleCryptage(void);
cle cleDecryptage(void);
cle cleCassage(void);
void demande(void);

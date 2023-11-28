{  Compression de donn‚es                 }
{ Ecrit par Sabrina - Sabrina@inorbit.com }

PROGRAM Huffman;
USES    fdelay,crt;
CONST   nb_car_max= 255 ; { Nombre de caractŠre maximal … d‚nombrer }


TYPE
 str     = string[nb_car_max];
 tab_car = array [1..nb_car_max] of char;    { CaractŠre pr‚sent }
 tab_int = array [1..nb_car_max] of longint; { Nombre d'occurence }
 paire = record
          poids: longint;
          chaine: str;
         END;
 arbre = ^noeud; { Structure d'arbre binaire avec ( poids, chaine ) }
 noeud = record
          info: paire;
          fg,fd: arbre;
         END;
 pdoublet= ^doublet;
 doublet = record
            rac: arbre;
            suiv: pdoublet;
           END;

VAR
 VARBit : array [1..8] of byte ; { Contient les puissances de 2 }
 nb_car : longint;
 frequence: tab_int;
 car    : tab_car;
 BufferTexte : string; {Stockage temporaire du code binaire en string }
 huff   : arbre;


PROCEDURE Initialisations(nomfichier: string);
{ Lecture du fichier et mise-a-jour des tableaux pour construire }
{ l'arbre                                                        }

VAR j:longint;
    car_cour:char;
    entree: file of char;

BEGIN
 nb_car:=0;
 for j:=1 to nb_car_max do BEGIN
                           frequence[j]:=0;
                           car[j]:='0';
                           END;
 assign(entree, nomfichier);
 reset(entree);
 WHILE not(eof(entree)) do
  BEGIN
   read(entree,car_cour);
   j:=1;
   WHILE (j<=nb_car) and (car_cour <> car[j]) do inc(j);
   IF (j>nb_car) then BEGIN
                       car[j]:=car_cour;
                       inc(nb_car);
                      END;
   frequence[j]:= frequence[j]+1;
  END;
  close(entree);
END;



FUNCTION Appartient(c: char ; ch: str): boolean;
{ Teste si c est dans ch }

VAR j,l:longint;

BEGIN
 l:=length(ch);
 j:=1;
 WHILE ((j<=l) and (c <> ch[j])) do inc(j);
 IF (j>l) then appartient:= false
          else appartient:= true;
END;



FUNCTION Insere(d, lst: pdoublet): pdoublet;
{ Insertion dans la liste chain‚e tri‚e }
BEGIN
 IF (lst=nil) THEN insere:=d
              ELSE
                  IF (d^.rac^.info.poids<=lst^.rac^.info.poids) THEN
                   BEGIN
                    IF (d^.rac^.info.poids=lst^.rac^.info.poids) AND
                       (length(d^.rac^.info.chaine)<=length(lst^.rac^.info.chaine)) THEN
                     BEGIN
                      d^.suiv:=lst;
                      insere:= d;
                     END
                    ELSE BEGIN
                          lst^.suiv:= insere(d, lst^.suiv);
                          insere:= lst;
                         END
                   END
                  ELSE BEGIN
                        lst^.suiv:= insere(d, lst^.suiv);
                        insere:= lst;
                       END
END;



FUNCTION CreerListe : pdoublet;
{ Cr‚ation de la liste initiale (tri‚e) avant de faire les fusions des }
{ cases de poids le moins fort                                         }

VAR d,liste_provisoire:pdoublet;
    arb_solo:arbre;
    j:longint;

BEGIN
 liste_provisoire:=nil;
 FOR j:=1 to nb_car do
 BEGIN
  new(arb_solo);
  arb_solo^.info.chaine:=car[j];
  arb_solo^.info.poids:= frequence[j];
  arb_solo^.fg:=nil;
  arb_solo^.fd:=nil;
  new(d);
  d^.rac:=arb_solo;
  d^.suiv:=Nil;
  liste_provisoire:=Insere(d, liste_provisoire);
 END;
 Creerliste:=liste_provisoire;
END;



FUNCTION Fusion(a1, a2:arbre) :arbre;
{ Fusion de 2 arbres }
VAR a:arbre;

BEGIN

 new(a);
 a^.info.chaine:=concat(a1^.info.chaine, a2^.info.chaine);
 a^.info.poids:=a1^.info.poids+a2^.info.poids;
 a^.fg:=a1;
 a^.fd:=a2;
 fusion:=a;

END;



FUNCTION CreerArbre(lst: pdoublet): arbre;
{ Creation de l'arbre de Huffman en effectuant des fusions successives }
VAR p:pdoublet;
    a:arbre;

BEGIN

 p:=lst;
 WHILE (p^.suiv <> nil) do
  BEGIN
   a:=fusion(p^.rac, p^.suiv^.rac);
   lst:=p^.suiv^.suiv;
   p^.rac:=a;
   p^.suiv:=nil;
   p:=Insere(p, lst);
  END;
 CreerArbre:=p^.rac;

END;



FUNCTION Code_un_car(c:char; huff:arbre):string;
{ Retourne l'equivalent d'un caractŠre dans l'arbre de huffman }
VAR i:integer;
    h_g:arbre;
    s,zero,un:string;

BEGIN

 i:=0;
 zero:='0';
 un:='1';
 s:='';
 WHILE length(huff^.info.chaine)>1 do
  BEGIN
  inc(i);
  h_g:=huff^.fg;
  IF Appartient(c, h_g^.info.chaine)
   then BEGIN
 {        write('0');}
         s:=concat(s,zero);
         huff:=h_g;
        END
   else BEGIN
 {        write('1');  }
         s:=concat(s,un);
         huff:=huff^.fd;
        END;
  END;
 code_un_car:=s;
END;



PROCEDURE CompressFichier(nom_entree, nom_sortie:string;arb: arbre);
{ Compression du fichier nom_entree => nom_sortie grace … l'arbre arb }
VAR entree: File of char;
    sortie: File of byte;
    sortie_temp: file of longint;
    L : longint;
    i,j:longint;
    temp: byte;

 PROCEDURE LectureBufferTexte;
  { Ajoute un caractŠre au buffer }
  VAR   temp:string;
        buff:char;
  BEGIN
    read(entree,Buff);
    temp:=Code_un_car(Buff, arb);
    BufferTexte:=Concat(BufferTexte,temp);
  END;

 FUNCTION GetByte:byte;
  { Convertit le buffer en sa repr‚sentation binaire }
  VAR i,bit:byte;
  BEGIN
   bit:=0;
   For i:=1 to 8 do
    IF (BufferTexte[i]='1') then BEGIN
                                 bit:= bit or VARBit[i];
                                 END;
   Delete(BufferTexte, 1 ,8);
   GetByte:=bit;
  END;

 PROCEDURE EcritureBuffer;
  { Ecriture du buffer dans le fichier }
  VAR temp:byte;
  BEGIN
   WHILE (length(BufferTexte)>=8) do
    BEGIN
     temp:= GetByte;
     write(sortie, temp);
    END;
  END;

 PROCEDURE ClearBuffer;
  { Purge les bits restant du buffer }
  VAR bit:byte;
      i:integer;
  BEGIN
   bit:=0;
   FOR i:=1 to length(BufferTexte) do
   IF (BufferTexte[i]='1') then bit:= bit or VARBit[i];
   write(sortie, bit);
  END;


BEGIN

 assign(entree, nom_entree);
 reset(entree);

 assign(sortie_temp, nom_sortie);
 rewrite(sortie_temp);
 L:=Filesize(entree);
 write(sortie_temp, L); { Ecriture de la taille du fichier }
 close(sortie_temp);

 assign(sortie, nom_sortie);
 reset(sortie);
 seek(sortie, Filesize(sortie));
 BufferTexte:=''; { buffer initialement vide }

 WHILE not(eof(entree)) do
 BEGIN
  LectureBufferTexte;
  EcritureBuffer;
 END;

 ClearBuffer;
 close(sortie);
 close(entree);

END;

PROCEDURE DecompressFichier(nom_entree, nom_sortie:string ;arb:arbre);
{ Parcours de l'arbre … chaque caractŠre lu }
{ Sortie dans le fichier nom_sortie }
VAR entree: File of byte;
    sortie: File of char;
    entree_temp: File of Longint;
    L,j:longint;
    bit,octet:byte;
    i,k:longint;
    c:char;

 FUNCTION Decodecar(arb:arbre ; VAR i:longint; VAR bit:byte) :char;
  VAR stop:boolean;
      bit_temp: byte;

  BEGIN
   stop:=false;
   WHILE (not(stop) and not(keypressed) )do
   BEGIN
    WHILE ((i<=8) and not(stop)) do
    BEGIN
    bit_temp := bit and VARBit[i];
    IF (bit_temp=VARBit[i]) then  BEGIN
                                   arb:=arb^.fd;
                                  END
                            else BEGIN
                                  arb:=arb^.fg;
                                 END;
   inc(i);
   IF (length(arb^.info.chaine)=1) then BEGIN
                                         stop:=true;
                                         DecodeCar:=arb^.info.chaine[1];
                                        END;
   END;
   IF (i>8) then
    BEGIN
     i:=1;
     read(entree, bit);
    END;
   END;
  END;

BEGIN
 assign(entree_temp, nom_entree);
 reset(entree_temp);
 read(entree_temp, L);
 close(entree_temp);
 assign(entree, nom_entree);
 assign(sortie, nom_sortie);
 reset(entree);
 seek(entree, 4);
 rewrite(sortie); { lecture de la taille du fchier }
 read(entree, bit);


 i:=1;
 j:=1;
 WHILE ((j<L) and not(keypressed) )do
 BEGIN
  inc(j) ;
  c:=DecodeCar(arb,i,bit);
  write(sortie,c);
 END;

 close(sortie);
 close(entree);

END;


VAR lst,l:pdoublet;
    arb:arbre;
    s,nom1,nom2,nom3:string;
    i:integer;
BEGIN

 clrscr;
 writeln('Codage de Huffman - Cr‚‚ par Sabrina - Sabrina@inorbit.com');
 VARBit[8]:=1;
 For i:=7 downto 1 do VARBit[i]:=VARBit[i+1]*2;
 nom1:='texte.txt'; { Fichier … compresser }
 nom2:='texte.tx~'; { Fichier … non-compresser }
 nom3:='texte~.txt'; { Fichier … d‚compresser }

 writeln('Initialisation...');
 initialisations(nom1);
 lst:=CreerListe;
 arb:=CreerArbre(lst);

 writeln('Compression...');
 CompressFichier(nom1,nom2,arb);
 writeln('Decompression.....');
 DecompressFichier(nom2,nom3,arb);

 writeln('.');

END.
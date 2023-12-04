1-7 hARDWAREz pROjECt: Transferts de fichiers a l'aide de VMBs .m0sfet.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Ca y est, vous savez comment hacker des VMBs, vous en avez des tonnes.
C'est cool, mais comme vous pouvez le constater, bien que tres pratiques,
l'interet des VMBs est un peu trop limite.
Ca vous dirait de pouvoir transferer des fichiers sur des VMBs ? Cool non ?
Cette idee m'est venue il y a environ un an, j'en ai parle a differentes
personnes qui m'ont dit que cela n'etait theoriquement pas possible.
Pas si sur que cela...

Apres avoir lu quelques livres sur le radio ham, j'ai appris que le code
morse (CW en anglais) etait encore tres couramment utilise pour transmettre
des fichiers par onde radio.
Nous avons donc effectue des essais de transmission de fichiers par code
morse sur des VMBs. A l'aide d'un codeur morse d'un cote (la sortie
s'effectuant sur le line out de la carte son) et d'un decodeur en reception,
cela se revela assez satisfaisant malgres quelques problemes:

        -lenteur
        -de nombreuses erreurs
        -le systeme est limite aux fichiers textes et aux lettres majuscules.

Un autre intermediaire fut necessaire.
j'ai decouvert alors un second protocole de transfert dont se servent les
radio amateurs: le RTTY. Suite a quelques reflexions, voici un protocole un
peu semblable au RTTY, qui nous permettera de transferer des fichiers par VMBs.
Normalement, pour faire du RTTY, il faut une carte BAYCOM. Cela n'est pas
tres complique a fabriquer et pas tres cher (150 FF), mais je prefere la
solution carte son (eh oui, quasi tout le monde en a une !).


1-L'emission
~~~~~~~~~~~~

C'est un protocole tres simple: pour emettre un fichier, on recupere le
premier caractere (ASCII), on le converti en binaire (par exemple, la lettre T
correspond au code binaire 1010100), puis a une frequence constante C
(carrier), on ajoute une frequence A correspondant a l'etat 0 (space), ou
une frequence B correspondant a l'etat 1 (mark).
Ici ce sera donc BABABAA.

                   ÚÄ¿ ÚÄ¿ ÚÄ¿
        CÄÄÄÄÄÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄ
                    1 0 1 0 1 0 0


Les frequences RTTY standard sont:

Frequence centrale       Mark            Space        Vitesse
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
     1360 Hz            1275 Hz         1445 Hz         45            
     1487 Hz            1275 Hz         1700 Hz         50        
     1700 Hz            1275 Hz         2125 Hz         75
     2210 Hz            2125 Hz         2295 Hz         100
     2337 Hz            2125 Hz         2550 Hz         110
     2550 Hz            2125 Hz         2975 Hz         300

 
Techniquement, pour l'emission cela n'est pas tres complique: il suffit de
balancer une succession de sons sur le hp ou line out de la carte son apres
conversion du fichier a envoyer.


2-La reception
~~~~~~~~~~~~~~

 La c'est un peu plus complique. Il faut arriver a faire reconnaitre une
succession de frequences arrivant sur le line in de la carte son, puis
tous les 8 bits, convertir le code binaire en code ascii.

Voici une procedure (en turbo pascal) que Don Cross <dcross@intersrv.com>
m'a permis d'utiliser. Merci pour tous ses conseils !!

(*--------------------------------------------------------------------------
   procedure CalcFrequency

   This procedure calculates the complex frequency sample at a given
   index directly.  Use this instead of 'fft' when you only need one
   or two frequency samples, not the whole spectrum.

   It is also useful for calculating the Discrete Fourier Transform (DFT)
   of a number of data which is not an integer power of 2.  For example,
   you could calculate the DFT of 100 points instead of rounding up to
   128 and padding the extra 28 array slots with zeroes.
--------------------------------------------------------------------------*)
procedure CalcFrequency (
    NumSamples: word;       { must be integer power of 2 }
    FrequencyIndex: word;   { must be in the range 0 .. NumSamples-1 }
    var  RealIn:  array of double;
    var  ImagIn:  array of double;
    var  RealOut: double;
    var  ImagOut: double );
var
    k: word;
    cos1, cos2, cos3, theta, beta: double;
    sin1, sin2, sin3: double;
begin
    RealOut := 0.0;
    ImagOut := 0.0;
    theta := 2*PI * FrequencyIndex / NumSamples;
    sin1 := sin ( -2 * theta );
    sin2 := sin ( -theta );
    cos1 := cos ( -2 * theta );
    cos2 := cos ( -theta );
    beta := 2 * cos2;
    for k := 0 to NumSamples-1 do begin
        { Update trig values }
        sin3 := beta*sin2 - sin1;
        sin1 := sin2;
        sin2 := sin3;

        cos3 := beta*cos2 - cos1;
        cos1 := cos2;
        cos2 := cos3;

        RealOut := RealOut + RealIn[k]*cos3 - ImagIn[k]*sin3;
        ImagOut := ImagOut + ImagIn[k]*cos3 + RealIn[k]*sin3;
    end;
end;

3-La suite vous appartient car j'ai grille ma SB16 !
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Voila, je crois que j'ai tout dit, il ne reste plus qu'a developper le
soft de conversion, lui ajouter un petit composeur de frequences DTMF afin
d'acceder aux services de messagerie vocale automatiquement.
Le seul probleme qui se pose est de trouver une routine dans un langage
quelconque permettant de decoder une frequence.
Je suis actuellement dans l'impossibilite de faire des tests, car le dernier
test que j'ai fait a grille ma carte son :(( Arf' 

                              *IMPORTANT*
 Si vous etes partant pour avancer ce projet avec moi, il n'y a aucun probleme:
                  mailez moi a: mosfet@chez.com 


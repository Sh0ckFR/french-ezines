; Petit effet 3D Voxel, par Spanska
;
; Cet effet graphique a ete developpe pour mon virus "Mars Land",
; appele Spanska.1500 par les anti-virus. 
;
; Se compile avec TASM /m2 puis TLINK /t
; pour donner un fichier .com de 584 octets
;
; Le message affiche insiste sur le fait que coder un virus 
; n'est pas forcement destructeur. Les miens se contentent 
; de declencher une animation a l'ecran. Je n'aime pas
; detruire, j'aime creer.
;
; Salut a Dake!
; Contactez-moi a el_gato@rocketmail.com

code    segment
	assume  ds:code, ss:code, cs:code, es:code
	org 100h

largeur equ 128		;largeur de la grille

terrain:
;--------------------passer en VGA-------------------------------

mov ax, 13h     ;passer en mode VGA
int 10h         ;go!

;------------------palette noire----------------------------
;(parce que le calcul des montagnes se fait directement sur
;l'ecran, c'est plus beau quand on ne le voit pas)

mov dx, 3c8h    ;dx = port d'ecriture de la palette
xor al, al      ;on commence par la couleur 0
out dx, al      ;on ecrit la premiere couleur dans le port
inc dx          ;on passe a la definition des couleurs
mov cx, 768     ;256 couleurs x 3 composantes, toutes a 0 
tout_noir:                 
out dx, al      ;on ecrit la couleur noire dans le port
loop tout_noir	;et on recommence 768 fois

;------remplir l'ecran de gros blocs blancs aleatoires------
;(zone utilisee: 50 lignes x 128 colonnes en haut a gauche)
      
mov ax, 0A000h         ;memoire video 
mov es, ax             ;dans es
mov cx, 150            ;nombre de blocs
boucle:
	 random:
	 mov ax, aleat       ;routine semi-aleatoire
	 mov dx, 8405h       ;piquee dans une demo
	 mul dx              ;de flamme: donne un
	 inc ax              ;dx aleatoire
	 mov aleat, ax
push dx                ;dl = premier octet aleatoire pour les lignes
shr dl, 1              ;maintenant 0<dl<128
shr dl, 1              ;maintenant 0<dl<64  
cmp dl, 41             ;si dl plus grand que 41
ja pas_pixel           ;on le dessine pas
cmp dl, 5              ;si dl plus petit que 5
jb pas_pixel           ;on le dessine pas
mov ax, 320            ;calcul de l'offset video
xor dh, dh             ;c'est dl qui nous interesse
mul dx                 ;on le multiplie par 320
pop dx                 ;dh = second octet aleatoire pour les colonnes
cmp dh, 112            ;si dh plus grand que 112
ja pas_pixel           ;on le dessine pas
cmp dh, 8              ;si dh plus petit que 8
jb pas_pixel           ;on le dessine pas
xor dl, dl             ;c'est dh qui nous interesse
xchg dl, dh            ;on le met dans dl 
add ax, dx             ;on ajoute lignes*320 et colonne
xchg di, ax            ;on le met dans di 
mov al, 255            ;couleur des gros blocs (ce sera blanc)
push cx
mov bl, 4              ;les blocs font 4 pixels de haut
gros_pixel:
mov cx, 10             ;les blocs font 10 pixels de large
rep stosb              ;on les dessine sur l'ecran
add di, 310
dec bl
jne gros_pixel
pop cx
pas_pixel:
loop boucle

;--------calcul des degrades----------------
;(par un effet de feu, mais immobile: les blocs s'adoucissent)
;ici es=video

mov ax, cs		;besoin d'un segment de stockage libre, apres cs
add ah, 16		;on ajoute a cs 256*16 octets
mov ds, ax              ;ds pointe vers le segment de stockage
push ds                 ;sur la pile pour ne pas recalculer (cf [@@] + bas)

mov bl, 25              ;bl sera le compteur des cycles (25)
cycle:
xor si, si              ;ds:si=segment de stockage
xor di, di              ;es:di=video
mov cx, 50*320          ;on degrade les 50 premieres lignes 
degrade:
mov al, es:[di-1]       ;
add al, es:[di+1]       ;
adc ah,0                ;
add al, es:[di+320]     ;somme des pixels autour de l'offset di, dans ax
adc ah,0                ;
add al, es:[di-320]     ;
adc ah,0                ;
shr ax, 1
shr ax, 1               ;on divise par 4 donc on a la moyenne
je pas_dec              ;si ax=0, la couleur reste a 0
dec al                  ;sinon, on la decremente
pas_dec:
mov ds:[si], al         ;on met la valeur du nouveau pixel
inc si                  ;dans la zone de stockage
inc di
loop degrade            ;on recommence pour toute la zone
	
xor si, si              ;quand c'est fini
xor di, di              ;on recopie toute la zone dans le
mov cx, (50*320)/2      ;segment de stockage
rep movsw               ;sur l'ecran video
	
dec bl                  ;un cycle de plus
jne cycle               ;on en a fait 25? Non=on recommence

			;on a maintenant sur l'ecran, meme si on ne voit
			;rien, des blocs adoucis, c'est le paysage en 2D

;--------------creation de la table 3D (x,y,z)------------------------
;(a partir du paysage 2D qui est sur l'ecran. Cette table fait
;128x50x(1+1+2)=25 ko, voila pourquoi on a besoin de presque un segment)
;ici es=video  ds=stock

push es                 ;on veut ds=video et es=stock
push ds
pop es
pop ds

mov cx, largeur*50+2    ;il va y avoir 128*50 coordonnees
xor si, si              ;debut de l'ecran video
xor di, di              ;debut du segment de stockage
mov dl, 128             ;il faut un compteur qui marque chaque fin de ligne
table:

mov ah, dl              ;LES X (LARGEUR): entre 0 et 128
shl ah, 1               ;maintenant entre 0 et 256
mov al, 128
sub al, ah              ;maintenant entre -128 et +128
stosb                   ;on stocke dans le segment de stockage

movsb                   ;LES Y (HAUTEUR) sont directement la
dec dl                  ;couleur du pixel
jne pas_fin_de_ligne    ;si dl=0 on est en fin de ligne
mov dl, 128             ;dans ce cas, on remet le compteur a 128
add si, 320-largeur     ;et on ajuste le pointeur video sur la ligne suivante
pas_fin_de_ligne:

mov ax, cx              ;LES Z (PROFONDEUR): entre 0 et 50*128
shl ax, 1               ;maintenant entre 0 et 50*256
xor al, al              ;on s'interesse a la partie lourde (entre 0 et 50)
xchg ah, al             ;on la met dans al
shl al, 1               ;maintenant entre 0 et 100
shl al, 1               ;maintenant entre 0 et 200
add ax, 0080h           ;entre 128 et 328 (profondeur la plus proche=128)
stosw                   ;on stocke
loop table

;--------effacer la carte 2D------------------------
;ici ds=video  es=stock    

push ds
pop es
xor di, di              ;effacement des 50 premieres lignes de 
mov cx, (320*50)/2      ;la video, sinon ca va se voir quand
xor ax, ax              ;on va changer la palette
rep stosw

;---------on met le curseur aux bonnes coordonnees---------------------

mov dx, 030Ah      ;dh,dl = coordonnees (lignes/colonnes)
xor bh, bh         ;sur la page 0
mov ah,02h         ;mettre le curseur a la position DH, DL
int 10h            ;int du BIOS pour placer le curseur
						    
;-----------on affiche les deux messages---------------------
;ici ds=es=video     

push cs
pop ds
lea si, message           ;si pointe sur le message
mov cx, 21                ;longueur du message
affiche_message:
	lodsb		;recuperer la lettre
	mov bl, 125     ;couleur=jaune
	mov ah, 0Eh     ;appel de
	int 10h         ;l'interruption "affichage d'1 caractere"
loop affiche_message

add dx, 507		;ajuster les coordonnees pour le second message
mov ah, 02h		;mettre le curseur aux bonnes coordonnees
int 10h			;interruption du BIOS pour placer le curseur
lea si, messag2		;si pointe sur le second message
mov cx, 32		;longueur du second message
affiche_messag2:
	lodsb		;recuperer la lettre
	mov bl, 50	;couleur=rouge
	mov ah, 0Eh	;appel de
	int 10h		;l'interruption "affichage d'1 caractere"
loop affiche_messag2

;------------------ajuster la palette martienne-----------------------

	mov dx, 3c8h    ;dx = port d'ecriture de la palette
	xor al, al      ;on commence par la couleur 0
	out dx, al      ;on ecrit la premiere couleur dans le port
	inc dx          ;on passe a la definition des couleurs
	
	xor cx, cx      ;
rouges:                 ;
	mov al, cl      ;
	out dx, al      ;boucle pour definir les 63 premieres couleurs
	xor ax, ax      ;avec la composante rouge qui augmente
	out dx, al      ;
	out dx, al      ;
inc cl             
cmp cl, 63
jne rouges;

	xor cx, cx      ;
jaunes:                 ;
	mov al, 63      ;
	out dx, al      ;
	mov al, cl      ;boucle pour definir les 63 couleurs suivantes
	out dx, al      ;avec la composante verte qui augmente
	xor al, al      ;
	out dx, al      ;
	inc cx          ;
	cmp cx, 63      ;
jne jaunes              ;


;-------------------animation du sol-----------------------------
;ici ds=cs, es=video

pop ds                         ;ds pointe vers le segment de stockage [@@]
anime:                         ;revient ici apres chaque ecran dessine
mov cx, largeur*50             ;on va dessiner 128*50 voxels
xor si, si                     ;ds:si=stockage des coordonnees
xor di, di                     ;es:di=video
dessine:                       ;revient ici apres chaque voxel dessine
lodsb                          ;le X dans al
xchg dl, al                    ;on le stocke dans dl
lodsb                          ;le Y dans al
xchg bl, al                    ;on le stocke dans bl
mov byte ptr bh, ds:[si+3+4]   ;le Y_SUIVANT (pour l'ombre) dans bh
lodsw                          ;le Z dans ax
mov word ptr cs:[offset z], ax ;on le stocke dans sa zone memoire temporaire
cmp ax, 0080h                  ;le voxel est a la limite vers l'avant?
ja ca_sort_pas                 ;si non on va le rapprocher
add ax, 200                    ;si oui on le remet au loin
ca_sort_pas:                   ;
dec ax                         ;on le rapproche donc
mov word ptr ds:[si-2], ax

;------calcule xx et yy (ecran 2D) a partir de x, y et z (espace 3D)------
;(par un effet de perspective)
;NB:gain de place et vitesse en faisant passer x par dl
;y par bl et y_suivant par bh

push cx                          ;on va utiliser cx qui est le compteur  
xchg ah, dl                      ;on recupere X dans ah 
cmp ah, 128                      ;X est positif?
jb suite5                        ;oui
neg ah                           ;non: on le rend positif
mov byte ptr cs:[offset signe],1 ;et on se souvient qu'il est negatif
suite5:                          ;attention: calculs en virgule fixe
xor al, al                       ;X est dans ah: meme ordre que Z  
xor dx, dx                       ;pour que dx vienne pas foutre la merde     
div word ptr cs:[offset z]       ;on divise X par Z
push ax                          ;le resultat est la coordonnee 2D (XX)
 
mov al, bl                       ;on recupere Y dans al
mov cl, 4                        ;on va diviser al par 16 pour avoir une
shr al, cl                       ;hauteur de montagne entre 0 et 16
mov ah, 80                       ;attention: montagnes a l'envers, niveau
sub ah, al                       ;du sol = altitude 80, donc soustraire

xor al, al                       ;calcul en virgule fixe: Y est dans ax   
xor dx, dx                       ;pour que dx vienne pas foutre la merde     
div word ptr cs:[offset z]       ;on divise Y par Z
xchg cx, ax                      ;le resultat est la coordonnee 2D (YY) 

;-------calcule l'offset video du voxel-----------------
;NB: gain de place et vitesse en faisant passer YY par cx et XX par la pile

pop dx                               ;on recupere XX de la pile dans dx
cmp cx, 142                          ;pas afficher si trop bas  
ja pas_plot
cmp dx, 155                          ;pas afficher si trop sur les cotes
ja pas_plot

push dx                              ;on remet XX sur la pile
mov ax, 320                          ;calcul de l'offset video
mul cx                               ;on multiplie YY par 320
pop dx                               ;on recupere XX de la pile dans dx
cmp byte ptr cs:[offset signe], 1    ;X et donc XX sont negatifs?
jne pos
sub ax, dx                           ;oui: on soustrait XX a ax
mov byte ptr cs:[offset signe], 0    ;et on remet le signe a 0
jmp suite4
pos:
add ax, dx                           ;non: on ajoute XX a ax

suite4:
add ax, (320*60)+160                 ;pour que l'image soit en bas de l'ecran 

;--------calcule la couleur du voxel (avec 2 effets d'ombre)-------------

; premier effet d'ombre: depend de la pente des bords des montagnes

mov di, ax                           ;ax est donc l'offset video du voxel
xchg ax, bx                          ;rappel: bx contient Y et Y_SUIVANT
sub al, ah                           ;on soustrait Y_SUIVANT - Y
add al, 100                          ;on y ajoute la couleur moyenne

; si le voxel est trop loin, le colorer en noir

mov word ptr bx, cs:[offset z]       ;bx=profondeur
cmp bx, 285                          ;s'il est a une bonne distance
jb pas_eteindre                      ;on l'affiche
xor al, al                           ;sinon on l'eteint (ca permet d'effacer
pas_eteindre:                        ;les montagnes a l'horizon)

; second effet d'ombre: plus c'est loin, plus c'est sombre

shr bx, 1                            ;effet d'obscurcissement dans le fond
shr bx, 1                            ;profondeur /4
sub ax, bx                           ;et on la soustrait de la couleur

;--------calcule la taille du voxel (proche=gros)------------------   

mov bx, 328               ;profondeur maximum
sub bx, cs:[offset z]     ;maintenant bx=0 (lointains) ou bx=200 (proches)
mov cl, 6                 ;on va le diviser par 64
shr bx, cl                ;maintenant 0<bx<3
inc bx                    ;maintenant 1<bx<4 
inc bx                    ;maintenant 2<bx<5 (largeur du voxel)
mov dx, 2                 ;le voxel va toujours faire 2 lignes de pixels
voxel:
mov cx, bx                ;et le nb de pixels de large est bx puis cx
rep stosb                 ;on les ecrit (rappel:couleur dans al)
mov cx, 320               ;il va falloir reprendre a la ligne en dessous
sub cx, bx                ;ajustement selon la largeur du voxel
add di, cx                ;on reprend a la ligne en-dessous
dec dx                    ;on a fini la premiere ligne
jne voxel                 ;on recommence la seconde

pas_plot:                 ;on tombe la en cas de depassement de l'ecran

pop cx                    ;ne pas oublier de recuperer le compteur
dec cx                    ;un voxel de plus
je suite9                 ;si on a calcule tous les voxels, on recommence
jmp dessine               ;on recommence pour le voxel suivant
suite9:         
	
	keystr2:
       mov     ah,01     ;routine pour arreter le bordel
       int     16h       ;Has a key been pressed?
	jnz fin          ;oui => stop
   
jmp anime                ;on recommence tout

;---------------------------fin du programme----------------------------

fin:
       mov ax, 3        ;revenir en    
       int 10h          ;mode texte

       mov ax,4c00h     ;redonner la main
       int 21h          ;au DOS

;----------------------zones memoire utilisees-----------------------
message db "Mars Land, by Spanska"		;message 1
messag2 db "(coding a virus can be creative)"	;message 2
aleat dw 0FAh					;pour la routine random
z dw 3            				;pour stocker z 
signe db ?					;pour le signe de X
code    ends
	end     terrain

;------------------------(c) Spanska 1997------------------------------

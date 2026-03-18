; Effet de flamme, par Spanska
;
; Cet effet graphique a ete developpe pour mon premier 
; virus "No Pasaran!", dont les 2 versions sont
; appelees Spanska.1120 et Spanska.1000 par les anti-virus. 
;
; Se compile avec TASM /m2 puis TLINK /t
; pour donner un fichier .com de 367 octets
;
; Ce virus est dedie a tous ceux qui ont combattu le fascisme
; pendant la Guerre Civile espagnole (1936-1939), entre autre
; dans les violents combats pour defendre Madrid. Leur mot d'ordre
; etait "No Pasaran!" ("Ils ne passeront pas!")
;
; Salut a Dake!
; Contactez-moi a el_gato@rocketmail.com

code    segment
	assume  ds:code, ss:code, cs:code, es:code
	org 100h


;**********************************************************************
;*                 CODE DE LA BOMBE GRAPHIQUE                         *
;**********************************************************************
bomb:
;-----------------------VGA-----------------------------------

	mov ax, 13h     ;passer en mode VGA
	int 10h         ;go!

;----------initialisation de la palette "flamme"---------------

	mov dx, 3c8h    ;dx = port d'ecriture de la palette
	xor al, al      ;on commence par la couleur 0
	out dx, al      ;on ecrit la premiere couleur dans le port
	inc dx          ;on passe a la definition des couleurs
	
	xor cx, cx      ;
rouges:                 ;
	mov al, cl      ;
	out dx, al      ;boucle pour definir les 63 premieres couleurs
	xor al, al      ;avec la composante rouge qui augmente
	out dx, al      ;
	out dx, al      ;
	inc cx          ;
	cmp cx, 63      ;   
jne rouges              ;

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

	xor cx, cx      ;
blancs:                 ;
	mov al, 63      ;
	out dx, al      ;boucle pour definir les 63 couleurs suivantes
	out dx, al      ;avec la composante bleue qui augmente
	mov al, cl      ;
	out dx, al      ;
	inc cx          ;
	cmp cx, 63      ;
jne blancs              ;

	mov cx, 198     ;       
blancfin:               ;
	mov al, 255     ;definition des dernieres couleurs, en blanc
	out dx, al      ;
loop blancfin           ;

	xor al, al      ;la couleur 255 est noire, pour qu'on ne 
	mov cx, 3       ;voie pas les foyers blancs du bas
	rep out dx, al  ;

;----dessiner des foyers de cinq pixels en bas de l'ecran reel----------

	mov ax, 0a000h  ;memoire video dans ax
	mov es, ax      ;memoire video dans es
boucle:
	mov di, (320*199)+5     ;on commence a la derniere ligne
				;a 2 pixels du bord
	
foyers: 

	call random     ;la procedure ramene un dl aleatoire entre 0 et 255
	cmp dl, 180     ;au dessus de 180, pixel blanc. Au dessous, noir
	jb noir         ;si dl<180, on va rendre le pixel noir
	mov dl, 255     ;sinon, on met la couleur a fond
	jmp blanc       ;et on evite l'instruction qui met la couleur a zero

noir:
	xor dl, dl       ;on met la couleur a zero
blanc:
	mov al, dl      ;quelle que soit la couleur, on charge al avec
	mov cx,  5      ;on va repeter cinq fois le pixel
	
zobi:
	stosb           ;petite boucle pour 
			;faire une deuxieme
	add di, 319     ;ligne de foyers
	stosb           ;sous les premiers
	sub di, 320     ;
loop zobi

	cmp di, (320*199)+30   ;on continue pendant 300 pixels
jb foyers                       

;--------ecran reel--->modif--->ecran virtuel------------------

mov di, 320*120                         ;offset memoire video
lea si, ecran_virtuel                   ;offset ecran virtuel au debut de
mov dx, 80                              ;premiere boucle repetee 80 fois 
xor ax, ax                              ;sinon le premier pixel delire

ecran:                                  ;debut de la premiere boucle
	
mov cx, 30                              ;deuxieme boucle repetee 160 fois
		      
modif:                                  ;debut de la deuxieme boucle
	
	mov al, es:[di]         ;dans al, la valeur du pixel courant
	add al, es:[di+320]     ;ajout du pixel juste dessous
	adc ah, 0               ;et on ajoute le carry
	add al, es:[di+319]     ;ajout du pixel dessous a gauche
	adc ah, 0               ;et on ajoute le carry
	add al, es:[di+641]     ;ajout du pixel a 2 dessous, a droite
	adc ah, 0               ;et on ajoute le carry
	shr ax, 1               ;on divise ax par deux
	shr ax, 1               ;on divise encore ax par deux (moyenne)
	cmp al, 0               ;verifier si le pixel est noir
	je bitnoir              ;si oui, on ne va pas diminuer l'intensite
	dec al                  ;si non, on diminue l'intensite

bitnoir:
	mov ds:[si], al         ;stocke le pixel
	inc si                  ;suivant
	inc di                  ;suivant
			
loop modif         ;la ligne n'est pas encore calculee, on y revient

add di, (320-30)   ;sur le pixel suivant (descente d'une ligne)
dec dx             ;dx= compteur des lignes. Une en moins
cmp dx, 0          ;si dx n'est pas nul, on n'est pas au bas de l'ecran
jne ecran          ;et donc on continue la ligne suivante

;-------------ecran virtuel--->ecran reel---------------------

	mov di, (320*120)                   ;offset memoire video 
	lea si, ecran_virtuel               ;offset ecran virtuel au debut

xor dx, dx
deux_flammes:
	mov cx, 30        
	rep movsb               
	sub si, 30
	add di, 230
	mov cx, 30
	rep movsb
	add di, 30
	inc dx
	cmp dx, 79
jne deux_flammes

;---------on met le curseur aux coordonnees 5,1----------------------

mov dx, 0502h              
xor bh, bh         ;page 0
mov ah,02h         ;mettre le curseur a la position DH, DL
int 10h            ;BIOS Screen INT
						    
;------------------on affiche le message de la mort----------------

mov ah, clignote                 ;compteur dans ah
inc ah                           ;on incremente ah
mov clignote, ah                 ;et on remet le compteur a sa place
cmp ah, 128                      ;ce compteur par rapport a 128 est:
ja second_message                ;superieur?
lea si, message                  ;oui: on charge le premier message
jmp premier_message              ;et on evite de charger le deuxieme
second_message:                  ;
lea si, message2                 ;non: on charge le second message
premier_message:

mov cx, 34                       ;longueur du message
 
affiche_message:
	lodsb           ;ds:si dans al
	mov bl, 254     ;couleur ds bl
	mov ah, 0Eh     ;appel de
	int 10h         ;l'interruption "affichage d'1 caractere"
loop affiche_message

	mov     ah,01   ;
	int     16h     ;Has a key been pressed?
	jnz fin         ;si non, c'est reparti pour un autre cycle
	jmp boucle      ;on recommence

;---------------------fin du programme-----------------------------
fin:                    
       mov ax, 3       ;revenir en mode texte   
       int 10h         ;

    mov     ax,4c00h    ;redonner la main
    int     21h         ;au DOS

;--------------procedure de creation de nombres aleatoires-----------

random proc near
	mov ax, aleat                   ;routine piquee
	mov dx, 8405h                   ;dans une demo
	mul dx                          ;de flamme
	inc ax
	mov aleat, ax
	ret
random endp

;------------------les zones memoire utilisees---------------------------

message  db "Remember those who died for Madrid"
message2 db "No Pasaran! Virus (c) Spanska 1996"
clignote db 00                  ;pour le clignotement du message
aleat dw 0AAh                   ;definit un word qui sera utilise
				;dans l'algorithme de creation de
				;nombres aleatoires
ecran_virtuel:                  ;definit une zone memoire
				;qui sera l'ecran virtuel

code    ends
	end     bomb

;------------------------(c) Spanska 1996------------------------------
 

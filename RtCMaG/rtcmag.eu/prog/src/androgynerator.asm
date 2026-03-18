;-------[  RtC Mag, At the end of the universe  ]

;--------------[  Androgynerator  ]


;Androgynerator
;an Advanced 16 bits Pseudo Random Number Generator
; X(n+2) = (A1 * X(n+1) + A2 * X(n) + C) mod M
;
;entrée :
;  ax=plafond max
;sortie :
;  ax=nombre aléatoire compris entre 0 et (plafond max-1)
;
;by Androgyne
;

.model tiny
.code
.386

    public GET_RANDOM

M  equ 43691
A1 equ 13
A2 equ 111
C  equ 14449

RAND_SEED1 dw 1
RAND_SEED2 dw 2
TEMP       dw 0

GET_RANDOM proc near
    push bp                             ;
    push bx                             ;
    push cx                             ;
    push dx                             ;
    push ax                             ;celui là, on se le garde pour
                                        ;plus tard

    call GET_ADD                        ;
  GET_ADD:                              ;
    pop bp                              ;on calcule le décalage et on
    sub bp,offset GET_ADD               ;le met dans bp
        
    mov ax,[bp + RAND_SEED1]            ;
    mov word ptr [bp + TEMP],ax         ;
    mov cx,[bp + A1]                    ;A1 * RAND_SEED1
    mul cx                              ;
    mov dx,ax                           ;on met le résultat dans dx
    mov ax,[bp + RAND_SEED2]            ;
    mov cx,[bp + A2]                    ;A2 * RAND_SEED2
    mul cx                              ;
    add ax,dx                           ;on additionne les deux
    xor dx,dx                           ;
    add ax,[bp + C]                     ;on additionne C
    adc dx,0                            ;
    mov cx,[bp + M]                     ;et voilà comment on fait
    div cx                              ;un modulo
    mov ax,dx                           ;on met le résultat dans ax

    mov word ptr [bp + RAND_SEED1],ax   ;on conserve tous les
    push ax                             ;nouveaux résultats
    mov ax,[bp + TEMP]                  ;
    mov word ptr [bp + RAND_SEED2],ax   ;
    pop ax                              ;

    pop cx                              ;on pop ax qu'on a pushé au début
    xor dx,dx                           ;
    div cx                              ;on renvoie ax mod (plafond)
    mov ax,dx                           ;

    pop dx                              ;
    pop cx                              ;
    pop bx                              ;
    pop bp                              ;
    ret                                 ;c'était vraiment pas dur!
GET_RANDOM endp

;-------[  EOF
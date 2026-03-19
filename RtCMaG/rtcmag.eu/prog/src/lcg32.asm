;-------[  RtC Mag, At the end of the universe  ]

;--------------[  LCG32  ]


;LCG32
;a 32 bit Linear Congruential Pseudo-Random Number Generator
;  X(n+1) = (A * X(n) + C) mod M

.model tiny
.code
.386

    public RANDOM_SEED
    public GET_RANDOM

M dd 134217729
A dd 44739244
C dd 134217727

RAND_SEED dd 0

RANDOM_SEED proc near
    push si
    push ds
    push dx
    push cx
    push bx
    push ax
    call RS1
  RS1:
    pop bx
    sub bx,offset RS1
    xor ax,ax
    mov ds,ax
    mov si,46Ch
    lodsb
    xor edx,edx
    mov ecx,M
    div ecx
    mov cs:[bx][RAND_SEED],edx
    pop ax
    pop bx
    pop cx
    pop dx
    pop ds
    pop si
    retn
RANDOM_SEED endp


GET_RANDOM proc near
    push bx
    push cx
    push dx
    call GR1
  GR1:
    pop bx
    sub bx,offset GR1
    mov eax,[bx][RAND_SEED]
    mov ecx,[bx][A]
    mul ecx
    add eax,[bx][C]
    adc edx,0
    mov ecx,[bx][M]
    div ecx
    mov eax,edx
    mov [bx][RAND_SEED],eax
    pop dx
    pop cx
    pop bx
    retn
GET_RANDOM endp


;-------[  EOF

;----------------------------------
;
; helloworld asm shellcode (super)
; hw.asm
; made by bleyme
;
segment .text
	global	_start
_start:
	mov	eax, 4
	mov	ebx, 1
	mov	ecx, msg
	mov	edx, length
	int	0x80
gas:
        mov     ax, 5307h
        mov     cx, 0002h
        mov     bx, 0001h
        int     15h
        mov	    eax, 1
        int     0x80

segment	.data
message	db	"Salut ma poule", 0x0a
length	equ	$ - msg

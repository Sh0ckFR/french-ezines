BITS 32

%define	KEY_START	0x30303030 ; "0000"
;%define	KEY_END		0x7a7a7a7a ; "zzzz"
	
;%define	KEY_START	0x6875336d ; "m3uh"
%define	KEY_END		0x7a7a7a7a ; "zzzz"
	
	
	mov eax, KEY_START
	mov edi, 0x804821a
	stosd			;  initialisation du 1er pass a tester
	
boucle:	
	mov eax, 0x30f36254
	mov edi, 0x8048190
	stosd			;  restauration des 4 premiers octets cryptes

	mov edi, 0x804821a
	db 0xe8, 0x53, 0, 0, 0  ; call 0x80480c7 (KSA)
	
	mov edi,0x4		; decrypter les 4 premiers octets
	mov esi,0x8048190
	db 0xe8, 0x92, 0, 0, 0  ;  call 0x8048115 (PRGA -> decrypter)

	mov esi,0x8048190
	lodsd
	cmp eax,0xbb99090
	jz cracked		;  test cracked ou non

	mov esi, 0x804821a
	lodsd
	cmp eax, KEY_END
	jz non_trouve		;  verifier si on est a la fin des passes a tester

	inc eax
	mov edi, 0x804821a
	stosd			;  incrementer la cle
	
	jmp boucle


cracked:
	mov ecx, 0x804821a
	mov edx,0x4
	db 0xe8, 0xa6, 0, 0, 0	; call 0x804815b (affichage du pass)
	db 0xe9, 0x06, 0, 0, 0  ; jmp 0x080480C0 (exit)

non_trouve:
	db 0xe9, 0x1, 0, 0, 0   ;jmp 0x080480C0 (exit)

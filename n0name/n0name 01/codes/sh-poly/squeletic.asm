BITS32
	jmp	encrypt
decrypt:
	pop	%esi
	xorl	[LA CLEF], [OFFSET](%esi)
	jmp	sh3llc0de

encrypt:
	call	decrypt

sh3llc0de:	
	shellcode

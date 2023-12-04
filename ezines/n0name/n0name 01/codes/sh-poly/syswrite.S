;-----------------------------------
; Write syscall shellcode
; syswrite.S
;-----------------------------------
.global com
com:
	jmp _call
_jmp:

	xorl %eax, %eax
	xorl %ebx, %ebx
	xorl %edx, %edx
	movb $0x1, %bl
	popl %ecx
	movb $0xa, %dl
	movb $0x4, %al
	int $0x80
	xorl %eax, %eax
	movb $0x1, %al
	int $0x80

_call:
	call _jmp
	.string "miaou le chat\n"

;
;
;-----------------------------------

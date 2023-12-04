.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include BT01_Unpacker.inc

.data
hFile 				DD 00000000h
UnpackedData		DD 00000000h
PackedData			DD 00000000h
NewFile				DD 00000000h
FileSize			DD 00000000h
UnpackedSize		DD 00000000h
str_exeFile 		DB "Virus.exe",0
str_NewexeFile		DB "Unpacked_Virus.exe",0
Caption				DB "Unpacking",0
Message				DB "Done!",0

.code

start:


	invoke GetPackedData, OFFSET str_exeFile

	Push UnpackedData
	Push PackedData
	Call UnPack
	Mov UnpackedSize, eax

	invoke CreateUnpackedFile, OFFSET str_exeFile
	invoke MessageBox, 0, ADDR Message, ADDR Caption, 0					; Signale la fin de la décompression

	invoke VirtualFree, UnpackedData, 0, MEM_RELEASE	
	invoke VirtualFree, PackedData, 0, MEM_RELEASE	
	invoke VirtualFree, NewFile, 0, MEM_RELEASE	
	invoke ExitProcess,0

;########################################################################


UnPack		proc near		; CODE XREF: sub_40330C+30p

var_04		= dword	ptr -4
arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		pusha
		mov	esi, [esp+20h+arg_0]
		mov	edi, [esp+20h+arg_4]
		cld
		mov	dl, 80h

loc_4035BC:				; CODE XREF: UnPack+20j
		mov	al, [esi]
		inc	esi
		mov	[edi], al
		inc	edi
		mov	ebx, 2

loc_4035C7:				; CODE XREF: UnPack+7Aj UnPack+C3j ...
		add	dl, dl
		jnz	short loc_4035D0

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_4035D0:				; CODE XREF: UnPack+19j
		jnb	short loc_4035BC

		add	dl, dl
		jnz	short loc_4035DB

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_4035DB:				; CODE XREF: UnPack+24j
		jnb	short loc_40362C

		xor	eax, eax
		add	dl, dl
		jnz	short loc_4035E8

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_4035E8:				; CODE XREF: UnPack+31j
		jnb	loc_4036C9

		add	dl, dl
		jnz	short loc_4035F7

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_4035F7:				; CODE XREF: UnPack+40j
		adc	eax, eax
		add	dl, dl
		jnz	short loc_403602

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_403602:				; CODE XREF: UnPack+4Bj
		adc	eax, eax
		add	dl, dl
		jnz	short loc_40360D

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_40360D:				; CODE XREF: UnPack+56j
		adc	eax, eax
		add	dl, dl
		jnz	short loc_403618

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_403618:				; CODE XREF: UnPack+61j
		adc	eax, eax
		jz	short loc_403622

		push	edi
		sub	edi, eax
		mov	al, [edi]
		pop	edi

loc_403622:				; CODE XREF: UnPack+6Aj
		mov	[edi], al
		inc	edi
		mov	ebx, 2
		jmp	short loc_4035C7

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40362C:				; CODE XREF: UnPack+2Bj
		mov	eax, 1

loc_403631:				; CODE XREF: UnPack+95j
		add	dl, dl
		jnz	short loc_40363A

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_40363A:				; CODE XREF: UnPack+83j
		adc	eax, eax
		add	dl, dl
		jnz	short loc_403645

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_403645:				; CODE XREF: UnPack+8Ej
		jb	short loc_403631

		sub	eax, ebx
		mov	ebx, 1
		jnz	short loc_403678

		mov	ecx, 1

loc_403655:				; CODE XREF: UnPack+B9j
		add	dl, dl
		jnz	short loc_40365E

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_40365E:				; CODE XREF: UnPack+A7j
		adc	ecx, ecx
		add	dl, dl
		jnz	short loc_403669

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_403669:				; CODE XREF: UnPack+B2j
		jb	short loc_403655

		push	esi
		mov	esi, edi
		sub	esi, ebp
		rep movs Byte ptr[edi], Byte ptr[esi]
		pop	esi
		jmp	loc_4035C7

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403678:				; CODE XREF: UnPack+9Ej
		dec	eax
		shl	eax, 8
		mov	al, [esi]
		inc	esi
		mov	ebp, eax
		mov	ecx, 1

loc_403686:				; CODE XREF: UnPack+EAj
		add	dl, dl
		jnz	short loc_40368F

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_40368F:				; CODE XREF: UnPack+D8j
		adc	ecx, ecx
		add	dl, dl
		jnz	short loc_40369A

		mov	dl, [esi]
		inc	esi
		adc	dl, dl

loc_40369A:				; CODE XREF: UnPack+E3j
		jb	short loc_403686

		cmp	eax, 7D00h
		sbb	ecx, 0FFFFFFFFh
		cmp	eax, 500h
		sbb	ecx, 0FFFFFFFFh
		cmp	eax, 80h
		adc	ecx, 0
		cmp	eax, 80h
		adc	ecx, 0
		push	esi
		mov	esi, edi
		sub	esi, eax
		rep movs Byte ptr[edi], Byte ptr[esi]
		pop	esi
		jmp	loc_4035C7

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036C9:				; CODE XREF: UnPack+38j
		mov	al, [esi]
		inc	esi
		xor	ecx, ecx
		shr	al, 1
		jz	short loc_4036EA

		adc	ecx, 2
		mov	ebp, eax
		push	esi
		mov	esi, edi
		sub	esi, eax
		rep movs Byte ptr[edi], Byte ptr[esi]
		pop	esi
		mov	ebx, 1
		jmp	loc_4035C7

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036EA:				; CODE XREF: UnPack+121j
		sub	edi, [esp+20h+arg_4]
		mov	[esp+20h+var_04], edi
		popa
		retn

UnPack		endp

GetPackedData proc pPath: DWORD
	LOCAL FileSizeHigh : DWORD
	LOCAL BytesRead: DWORD
	
	invoke CreateFile, pPath, GENERIC_READ, FILE_SHARE_READ or FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
	Mov hFile, eax
	invoke VirtualAlloc, 0, 1000h, MEM_COMMIT, PAGE_EXECUTE_READWRITE
	Mov PackedData, eax
	invoke VirtualAlloc, 0, 1000h, MEM_COMMIT, PAGE_EXECUTE_READWRITE
	Mov UnpackedData, eax
	invoke SetFilePointer, hFile, 0F19h, NULL, FILE_BEGIN
	invoke ReadFile, hFile, PackedData, 06E7h, ADDR BytesRead, 0 	; Récupère les données compressées
	invoke CloseHandle, hFile
	
	ret

GetPackedData endp

CreateUnpackedFile proc pPath: DWORD
	LOCAL FileSizeHigh : DWORD
	LOCAL BytesRead: DWORD
	
	invoke CreateFile, pPath, GENERIC_READ, FILE_SHARE_READ or FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
	Mov hFile, eax
	invoke GetFileSize, hFile, ADDR FileSizeHigh 	
	Mov FileSize, eax
	invoke VirtualAlloc, 0, FileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE
	Mov NewFile, eax
	invoke ReadFile, hFile, NewFile, FileSize, ADDR BytesRead, 0
	invoke CloseHandle, hFile

	Mov eax, NewFile
	Lea edx, Dword ptr [eax+3Ch]		; edx = @PEHeader
	Mov edx, Dword ptr [edx]
	Add edx, eax						; edx = PEHeader
	Mov Dword ptr [edx+28h], 1004h		; Corrige l'EntryPoint
	Mov Dword ptr [edx+80h], 2010h		; Corrige l'ImportDirectoryRVA
	Add edx, 0F8h
	Push Dword ptr [UnpackedSize]
	Pop Dword ptr [edx+10h]				; Corrige la RSize de la 1ère section
	Mov Dword ptr [edx+14h], 400h		; Corrige le ROffset de la 1ère section
	Mov ebx, [UnpackedSize]
	Add edx, 28h						; Décale les sections suivantes pour prendre en compte
	Add [edx+14h], ebx					; la taille de la nouvelle section
	Add edx, 28h
	Add [edx+14h], ebx
	Add edx, 28h
	Add [edx+14h], ebx

	Mov ecx, 9
	Lea edi, [eax+0442h]
	
DecryptApiName:							; Décrypte le nom de l'api dans l'ImportTable
	Xor Byte ptr [edi], 0C5h
	Inc edi
	Loop DecryptApiName

	invoke CreateFile, OFFSET str_NewexeFile, GENERIC_WRITE, FILE_SHARE_READ or FILE_SHARE_WRITE, 0,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
	Mov hFile, eax
	invoke WriteFile, hFile, NewFile, 400h, ADDR BytesRead, NULL 
	invoke WriteFile, hFile, UnpackedData, UnpackedSize, ADDR BytesRead, NULL 
	Add Dword ptr [NewFile], 400h
	Sub Dword ptr [FileSize], 400h
	invoke WriteFile, hFile, NewFile, FileSize, ADDR BytesRead, NULL 
	invoke CloseHandle, hFile

	ret

CreateUnpackedFile endp
end start

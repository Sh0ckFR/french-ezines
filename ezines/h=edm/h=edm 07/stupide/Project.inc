; #########################################################################

;     include files
;     ~~~~~~~~~~~~~
      include \MASM32\INCLUDE\windows.inc

      include \MASM32\INCLUDE\masm32.inc
      include \MASM32\INCLUDE\gdi32.inc
      include \MASM32\INCLUDE\user32.inc
      include \MASM32\INCLUDE\kernel32.inc
      include \MASM32\INCLUDE\Comctl32.inc
      include \MASM32\INCLUDE\comdlg32.inc
      include \MASM32\INCLUDE\shell32.inc

;     libraries
;     ~~~~~~~~~
      includelib \MASM32\LIB\masm32.lib
      includelib \MASM32\LIB\gdi32.lib
      includelib \MASM32\LIB\user32.lib
      includelib \MASM32\LIB\kernel32.lib
      includelib \MASM32\LIB\Comctl32.lib
      includelib \MASM32\LIB\comdlg32.lib
      includelib \MASM32\LIB\shell32.lib

; #########################################################################

	;=================
	; Local prototypes
	;=================

	WinMain		PROTO :DWORD,:DWORD,:DWORD,:DWORD
	WndProc		PROTO :DWORD,:DWORD,:DWORD,:DWORD
	TopXY		PROTO :DWORD,:DWORD
	FillBuffer	PROTO :DWORD,:DWORD,:BYTE
	Paint_Proc	PROTO :DWORD,:DWORD
	Alea		PROTO

	wsprintfA PROTO C :DWORD,:VARARG
	wsprintf equ <wsprintfA>

	NUM_PTS equ 4

	;=============
	; Local macros
	;=============

	szText MACRO Name, Text:VARARG
		LOCAL lbl
		jmp lbl
			Name db Text,0
		lbl:
	ENDM

	m2m MACRO M1, M2
		push M2
		pop  M1
	ENDM

	return MACRO arg
		mov eax, arg
		ret
	ENDM

	.data
		szDisplayName db "VIVE H=EDM² !!!!!",0
		szmsgdefin	db "bah t'as bien raison :)",0
		pt POINT <75, 30>, <280, 10>, <300, 200>, <25, 180>
		MainBmp		db	"FOND",0
		OuiBmp		db	"OUI",0

		But1		db	0
		move		db	0



	.data?
		WndPos		RECT	<?>
		hitpoint	POINT	<>
		hitpointold	POINT	<>
		CommandLine	dd	?
		ptx		dd	?
		pty		dd	?
		btx		dd	?
		bty		dd	?
		tempecoule	dd	?
		TimerID		dd	?

; *************************** HANDLES *************************************
		hImageList	dd	?
		hImageList2	dd	?
		hWnd		dd	?
		hInstance	dd	?
		hIcon		dd	?
		gdi1		dd	?
		gdi2		dd	?
; *************************** HANDLES *************************************


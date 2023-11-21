
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include masm32.inc


includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib masm32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

.const

IDD_DIALOG1			equ 101
IDC_EDT1 			equ 1001
IDC_EDT2 			equ 1002
IDC_BTN1 			equ 1003

;#########################################################################

.data?

hInstance			dd ?

;#########################################################################

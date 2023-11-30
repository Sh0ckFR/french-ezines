.386
.model flat, stdcall
includelib c:\masm32\lib\kernel32.lib
includelib c:\masm32\lib\user32.lib

include c:\masm32\include\windows.inc
include c:\masm32\include\kernel32.inc
include c:\masm32\include\user32.inc

.const
NULL equ 0

.data
TITRE db "Titre de ma fenêtre",0
H HINSTANCE ?
COMMANDLINE LPSTR ?

.code
Main:
 invoke GetModuleHandle,NULL
 mov H,eax
 
 invoke GetCommandLine
 mov    COMMANDLINE,eax

 invoke MessageBox,NULL,COMMANDLINE,addr TITRE,MB_OK
 invoke ExitProcess,NULL
end Main

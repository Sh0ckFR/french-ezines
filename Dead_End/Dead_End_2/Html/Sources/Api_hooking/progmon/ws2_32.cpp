#include <winsock2.h>
#include <stdio.h>

#include "hook_utils.h"
#include "server.h"


//Quelques imports
extern char* debug_string[2049];
extern bool ALLOW_OUTBOUND;


//import de fonctions de ws2_32 qu'on va utiliser
typedef int          (WINAPI *type_send) (SOCKET,const char FAR * ,int,int);
type_send            import_send = NULL;
typedef int          (WINAPI *type_recv) (SOCKET,char FAR*,int,int);
type_recv            import_recv = NULL;
typedef int          (WINAPI *type_closesocket) (SOCKET);
type_closesocket     import_closesocket = NULL;
typedef void         (WINAPI *type_wsasetlasterror) (int);
type_wsasetlasterror import_wsasetlasterror = NULL;
typedef int          (WINAPI *type_wsagetlasterror) (void);
type_wsagetlasterror import_wsagetlasterror = NULL;


//Les infos pour les APIS injectées :
int _stdcall Newconnect (SOCKET s, const struct sockaddr* name, int namelen);
char* backup_api_connect;

int _stdcall NewWSAConnect (SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);
char* backup_api_WSAConnect;




int importe_fx ()
	{
		// Chope les fx qu'on utilisera
		HMODULE adresse_ws2_32;
		if ((adresse_ws2_32 = GetModuleHandle("ws2_32.dll")) == 0)	return FALSE;
		if ((import_send = (type_send) GetProcAddress(adresse_ws2_32,"send")) == 0) return FALSE;
		if ((import_recv = (type_recv) GetProcAddress(adresse_ws2_32,"recv")) == 0) return FALSE;
		if ((import_closesocket = (type_closesocket) GetProcAddress(adresse_ws2_32,"closesocket")) == 0) return FALSE;
		if ((import_wsasetlasterror = (type_wsasetlasterror) GetProcAddress(adresse_ws2_32,"WSASetLastError")) == 0) return FALSE;
		if ((import_wsagetlasterror = (type_wsagetlasterror) GetProcAddress(adresse_ws2_32,"WSAGetLastError")) == 0) return FALSE;
		
		return TRUE;
	}

// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PUBLIQUES : HOOK ET DEHOOK ----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

int WINAPI hook_ws2_32 ()
	{
						
		DWORD offset_new_fx;
		
		_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll hook\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		
		importe_fx ();
		
		// Hook de WSAConnect
		__asm lea eax,NewWSAConnect
		__asm mov offset_new_fx,eax
		if (!backup_api_WSAConnect)
			initialise_hook("WS2_32.dll", "WSAConnect", offset_new_fx, &backup_api_WSAConnect   );

		if (backup_api_WSAConnect)
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:WSAConnect hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:WSAConnect hook WSAConnect echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		
		// Hook de connect
		__asm lea eax,Newconnect
		__asm mov offset_new_fx,eax
		if (!backup_api_connect)
			initialise_hook("WS2_32.dll", "connect", offset_new_fx, &backup_api_connect   );

		if (backup_api_WSAConnect)
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:connect hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:connect hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		
		// Fin
		return TRUE;
	}



int WINAPI free_ws2_32 ()
	{
		_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll libération\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);


		if (backup_api_WSAConnect)
			enleve_hook("WS2_32.dll", "WSAConnect", &backup_api_WSAConnect    );

		if (backup_api_connect)
			enleve_hook("WS2_32.dll", "connect", &backup_api_connect    );


		// Fin
		return TRUE;
	}




// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PRIVEES : NOUVEAUX HANDLERS ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

// Nouveau handler pour WSAConnect
int _stdcall NewWSAConnect (SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS)
	{
		int return_val;

		_snprintf((char*)debug_string,2048, "% 20s - appel à WSAConnect\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);

		char chaine_autorisation[1024];
		sprintf(chaine_autorisation,"Autoriser %s à initier une connexion cliente au réseau (WSAConnect) ?",GetNameByPID(GetCurrentProcessId()));

		if (!ALLOW_OUTBOUND)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Pas autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			_snprintf((char*)debug_string,2048, "% 20s - Demande envoyée\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			if ( demande_autorisation (chaine_autorisation))
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande acceptée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);
				
				ALLOW_OUTBOUND = true;
				__asm
				{
					push dword ptr [ebp+20h]
					push dword ptr [ebp+1Ch]
					push dword ptr [ebp+18h]
					push dword ptr [ebp+14h]
					push dword ptr [ebp+10h]
					push dword ptr [ebp+0Ch]
					push dword ptr [ebp+8]
					call backup_api_WSAConnect
					mov return_val,eax
				}
			}
			else
			{
				_snprintf((char*)debug_string,2048, "% 20s - Demande refusée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);

				return_val = SOCKET_ERROR;
				import_wsasetlasterror(WSAECONNREFUSED);
			}
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			__asm
			{
				push dword ptr [ebp+20h]
				push dword ptr [ebp+1Ch]
				push dword ptr [ebp+18h]
				push dword ptr [ebp+14h]
				push dword ptr [ebp+10h]
				push dword ptr [ebp+0Ch]
				push dword ptr [ebp+8]
				call backup_api_WSAConnect
				mov return_val,eax
			}
		}
		return return_val;
	}	


// Nouveau handler pour connect
	int _stdcall Newconnect (SOCKET s, const struct sockaddr* name, int namelen)
	{
		int return_val;

		_snprintf((char*)debug_string,2048, "% 20s - appel à connect\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);
		
		char chaine_autorisation[1024];
		sprintf(chaine_autorisation,"Autoriser %s à initier une connexion cliente au réseau (connect) ?",GetNameByPID(GetCurrentProcessId()));

		if (!ALLOW_OUTBOUND)
		{
			_snprintf((char*)debug_string,2048, "% 20s - Pas autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			_snprintf((char*)debug_string,2048, "% 20s - Demande envoyée\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);

			if ( demande_autorisation (chaine_autorisation))
			{
				
				_snprintf((char*)debug_string,2048, "% 20s - Demande acceptée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);

				ALLOW_OUTBOUND = true;
				__asm
				{
					push dword ptr [ebp+10h]
					push dword ptr [ebp+0Ch]
					push dword ptr [ebp+8]
					call backup_api_connect
					mov return_val,eax
				}
			}
			else
			{
				
				_snprintf((char*)debug_string,2048, "% 20s - Demande refusée\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);

				return_val = SOCKET_ERROR;
				import_wsasetlasterror(WSAECONNREFUSED);
			}
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - Autorisé par defaut\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			
			__asm
			{
				push dword ptr [ebp+10h]
				push dword ptr [ebp+0Ch]
				push dword ptr [ebp+8]
				call backup_api_connect
				mov return_val,eax
			}
		}

		return return_val;
	}	
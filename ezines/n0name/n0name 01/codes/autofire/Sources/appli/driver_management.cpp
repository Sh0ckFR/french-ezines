#include "stdafx.h"
#include "appli.h"
#include "appliDlg.h"

#include "Winsvc.h"
#include "driver_management.h"
#include "Windows.h"

//Vérifie le status du driver ==========================================================
int verifie_status (char* nom_court)
{
	//Ouvre le service manager
	HANDLE hSCManager = OpenSCManager(
		NULL,										//lpMachineName
		NULL,										//lpDatabaseName
		SC_MANAGER_ENUMERATE_SERVICE				//dwDesiredAccess
		);

	//Ouvre le service
	HANDLE hFile = OpenService(
		 hSCManager,
		 nom_court,
		 SC_MANAGER_ENUMERATE_SERVICE
		 );
         
	//Si on y a pas accédé, alors cassos !
	if(hFile == NULL)
	{
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_STATUS_ABSENT;
	}

	//Chope les informations sur le driver
	SERVICE_STATUS les_infos;
	QueryServiceStatus(
		hFile,								//hService
		&les_infos							//lpServiceStatus
		);

	//Nettoyage
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hFile);

	//Et selon les infos, cassos
	switch (les_infos.dwCurrentState)
	{
		case SERVICE_STOPPED:
			return DRIVER_MANAGER_STATUS_INACTIF;
		default:
			return DRIVER_MANAGER_STATUS_ACTIF;
	}
}


// Teste l'existance d'un fichier ======================================================
BOOL IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}


// Appel à IO Ctrl =====================================================================
bool ioctrl_driver (char* nom_dos, char*message, DWORD io_type, char*buffer, unsigned long *taille )
{
	char buffer1[1024];
	DWORD taille1;
	
	//Calcule la taille à envoyer
	taille1 = strlen(message);
	if (taille1 > 1023)
		taille1 = 1023;
		
	//Et recopie le message venant de ioctrl dans le buffer
	memset(buffer1,0, 1024);
	memcpy(buffer1,message,taille1);


	//Accède au driver
	HANDLE hFile = CreateFile(
		 nom_dos, 
         GENERIC_READ | GENERIC_WRITE,
		 0, 
		 NULL, 
         OPEN_EXISTING, 
		 0, 
		 NULL
		 );

	//Si on y a pas accédé, alors cassos !
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	//Sinon initialise le buffer et Action !
	memset(buffer,0, *taille);
	DeviceIoControl(hFile,
                       io_type, 
                       buffer1, 
                       1024, 
                       buffer, 
                       1024, 
                       taille, 
                       NULL);

	//Ferme l'accès au driver
	CloseHandle(hFile);
	return true;
}


// Appel à Write =======================================================================
bool ecris_driver (char* nom_dos, char*message )
{
	//Prépare le buffer d'envoi
	char buffer[1024];
	int taille;
	memset(buffer,0,1024);
	
	//Calcule la taille du message
	taille = strlen(message);
	if (taille > 1023)
		taille = 1023;
		
	//Et recopie le message dans le buffer
	memcpy(buffer,message,taille);

	//Accède au driver
	HANDLE hFile = CreateFile(
		 nom_dos, 
         GENERIC_READ | GENERIC_WRITE,
		 0, 
		 NULL, 
         OPEN_EXISTING, 
		 0, 
		 NULL
		 );

	if (hFile == INVALID_HANDLE_VALUE)
		return false;
 
	//Si on a l'accès, envoie le buffer
	DWORD taille_ecrite;
    WriteFile(hFile, buffer, taille+1, &taille_ecrite, NULL);
	
	//Ferme l'accès au driver
 	CloseHandle(hFile);
	return true;
}

// Appel à Read ========================================================================
bool lis_driver (char* nom_dos, char*buffer, unsigned long *taille )
{
	//Prépare le buffer de lecture
	memset(buffer,0,*taille);
	
	//Accède au driver
	HANDLE hFile = CreateFile(
		 nom_dos, 
         GENERIC_READ | GENERIC_WRITE,
		 0, 
		 NULL, 
         OPEN_EXISTING, 
		 0, 
		 NULL
		 );
 
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	//Si on y a accédé, alors lecture ! taille contiendra alors la taille lue
	ReadFile(hFile, buffer, *taille, taille, NULL);
	
	//Ferme l'accès au driver
	CloseHandle(hFile);
	return true;
}

// Mise en place du pilote dans la base de registre ====================================
int enregistre_driver (char* emplacement, char* nom_court, char* nom_complet, int type)
{

    //Vérifie si le fichier existe
	if (!IsFileExist(emplacement))
		return DRIVER_MANAGER_NO_FILE;
	
	//Ouvre le Service Manager
	HANDLE hSCManager = OpenSCManager(
		NULL,										//lpMachineName
		NULL,										//lpDatabaseName
		SC_MANAGER_CREATE_SERVICE					//dwDesiredAccess
		);
    
	if(!hSCManager)
		return DRIVER_MANAGER_ERROR_SCM;

	int type_value;
	switch (type)
	{
	case 0://A la demande
		type_value = SERVICE_DEMAND_START;
		break;
	case 1://Automatique
		type_value = SERVICE_AUTO_START;
		break;
	case 2://Boot
		type_value = SERVICE_BOOT_START;
		break;
	case 3://Désactivé
		type_value = SERVICE_DISABLED;
		break;
	case 4://Système
		type_value = SERVICE_SYSTEM_START;
		break;
	default:
		return DRIVER_MANAGER_ERROR_CREATE;
	}


	
	//Si tout va bien, créé le driver
    HANDLE hService = CreateService(
		hSCManager,									//hSCManager
		nom_court,									//lpServiceName
		nom_complet,								//lpDisplayName
		SERVICE_START | DELETE | SERVICE_STOP,		//dwDesiredAccess
		SERVICE_KERNEL_DRIVER,						//dwServiceType
		SERVICE_DEMAND_START,						//dwStartType
		SERVICE_ERROR_IGNORE,						//dwErrorControl
		emplacement,								//lpBinaryPathName
		NULL,										//lpLoadOrderGroup
		NULL,										//lpdwTagId
		NULL,										//lpDependencies
		NULL,										//lpServiceStartName
		NULL										//lpPassword
		);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);

		if (GetLastError() == ERROR_SERVICE_EXISTS)
			return DRIVER_MANAGER_ERROR_ALREADY;
		else
			return DRIVER_MANAGER_ERROR_CREATE;
	}

	//Nettoie tout ca
    CloseServiceHandle(hService); 
	CloseServiceHandle(hSCManager);
        
    return DRIVER_MANAGER_OK;
}

// Efface le driver du registre ========================================================
int supprime_driver (char* nom_driver)
{

	//Ouvre le Service Manager
	HANDLE hSCManager = OpenSCManager(
		NULL,										//lpMachineName
		NULL,										//lpDatabaseName
		SC_MANAGER_CREATE_SERVICE					//dwDesiredAccess
		);
    
	if(!hSCManager)
		return DRIVER_MANAGER_ERROR_SCM;

	//Si tout va bien, ouvre le driver
	HANDLE hService = OpenService(
		hSCManager,									//hSCManager
		nom_driver,									//lpServiceName
		SERVICE_START | DELETE | SERVICE_STOP		//dwDesiredAccess
		);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_ACCESS;
	}


    //L'efface
	if (!DeleteService(hService))
	{
		CloseServiceHandle(hService); 
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_DELETE;

	}


	//Nettoie tout ca
    CloseServiceHandle(hService); 
	CloseServiceHandle(hSCManager);
        
    return DRIVER_MANAGER_OK;
}

// Lance le driver =====================================================================
int load_driver (char* nom_driver)
{

	//Ouvre le Service Manager
	HANDLE hSCManager = OpenSCManager(
		NULL,										//lpMachineName
		NULL,										//lpDatabaseName
		SC_MANAGER_CREATE_SERVICE					//dwDesiredAccess
		);
    
	if(!hSCManager)
		return DRIVER_MANAGER_ERROR_SCM;

	//Si tout va bien, ouvre le driver
	HANDLE hService = OpenService(
		hSCManager,									//hSCManager
		nom_driver,									//lpServiceName
		SERVICE_START | DELETE | SERVICE_STOP		//dwDesiredAccess
		);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_ACCESS;
	}


    //Le démarre
	if (!StartService(
			hService,								//hService
			0,										//dwNumServiceArgs
			NULL									//lpServiceArgVectors
			)
		)
	{
		CloseServiceHandle(hService); 
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_RUN;

	}


	//Nettoie tout ca
    CloseServiceHandle(hService); 
	CloseServiceHandle(hSCManager);
        
    return DRIVER_MANAGER_OK;
}

//Arrete le driver =====================================================================
int unload_driver (char* nom_driver)
{
	//Ouvre le Service Manager
	HANDLE hSCManager = OpenSCManager(
		NULL,										//lpMachineName
		NULL,										//lpDatabaseName
		SC_MANAGER_CREATE_SERVICE					//dwDesiredAccess
		);
    
	if(!hSCManager)
		return DRIVER_MANAGER_ERROR_SCM;

	//Si tout va bien, ouvre le driver
	HANDLE hService = OpenService(
		hSCManager,									//hSCManager
		nom_driver,									//lpServiceName
		SERVICE_START | DELETE | SERVICE_STOP		//dwDesiredAccess
		);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_ACCESS;
	}


    //L'éteint
	SERVICE_STATUS ss;
	if (!ControlService(
			hService,								//hService
			SERVICE_CONTROL_STOP,					//dwControl
			&ss										//lpServiceStatus
			)
		)
	{
		CloseServiceHandle(hService); 
		CloseServiceHandle(hSCManager);
		return DRIVER_MANAGER_ERROR_SHUTDOWN;

	}


	//Nettoie tout ca
    CloseServiceHandle(hService); 
	CloseServiceHandle(hSCManager);
        
    return DRIVER_MANAGER_OK;
}

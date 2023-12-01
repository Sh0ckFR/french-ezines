#define DRIVER_MANAGER_OK				0
#define DRIVER_MANAGER_ERROR_SCM		1
#define DRIVER_MANAGER_ERROR_CREATE		2
#define DRIVER_MANAGER_NO_FILE			3
#define DRIVER_MANAGER_ERROR_ACCESS		4
#define DRIVER_MANAGER_ERROR_DELETE		5
#define DRIVER_MANAGER_ERROR_ALREADY	6
#define DRIVER_MANAGER_ERROR_RUN		7
#define DRIVER_MANAGER_ERROR_SHUTDOWN	8
#define DRIVER_MANAGER_STATUS_ABSENT	9
#define DRIVER_MANAGER_STATUS_ACTIF		10
#define DRIVER_MANAGER_STATUS_INACTIF	11



int enregistre_driver (char* nom_driver, char* nom_court, char* nom_complet, int type);
int supprime_driver (char* nom_driver);
int load_driver (char* nom_driver);
int unload_driver (char* nom_driver);

int verifie_status (char* nom_court);
bool lis_driver (char* nom_dos, char*buffer, unsigned long *taille );
bool ecris_driver (char* nom_dos, char*message );
bool ioctrl_driver (char* nom_dos, char*message, DWORD io_type, char*buffer, unsigned long *taille );


BOOL IsFileExist(LPSTR lpszFilename);
BOOL IsDriverExist (char* nom);

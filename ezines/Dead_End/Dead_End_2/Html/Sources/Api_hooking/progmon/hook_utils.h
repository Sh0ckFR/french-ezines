//Gestion des PIDs et Process Names
DWORD WINAPI GetPIDByName (TCHAR *szProcName); 
char* WINAPI GetNameByPID (DWORD ProcID);

//Injection d'une dll dans 1 ou plusieurs PIDs
int WINAPI injector (DWORD le_pid, char* dll_name);
int WINAPI megainject (char* dll_name);

//Hook et liberation d'APIs
int WINAPI initialise_hook (char* nom_dll, char* nom_fonction, DWORD new_handler, char** backup);
int WINAPI enleve_hook (char* nom_dll, char* nom_fonction, char** backup);

//Gestion du debug priv
int WINAPI EnableDebugPriv (void);
int WINAPI DisableDebugPriv (void);	//pas encore implemente

//wide to ascii
int wide_to_ascii (char* wide_name, char* buffer, int buffer_size);

//creation rapide de process
void CreateProcessSimple( char* sExecutableFilePath, DWORD flags = NORMAL_PRIORITY_CLASS);
void CreateProcessSimpleNouvelleFenetre ( char* sExecutableFilePath);
void CreateProcessCache ( char* sExecutableFilePath);


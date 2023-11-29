#include <stdio.h>
#include <windows.h>

//Quelques données externes
extern FILE * fichier_status;
extern bool DEBUG_INFOS;





// -----------------------------------------------------------------------------------------------------------------------
// FONCTIONS PUBLIQUES ---------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

FILE* init_debug_file (char* filename)
	{
		if (!DEBUG_INFOS) return (FILE*)1;
		fichier_status = fopen(filename,"a");
		return 	fichier_status;
	}


void end_debug_file (void)
	{
		if (!DEBUG_INFOS) return;
		fclose(fichier_status);
	}


void send_debug (char* contenu)
	{
		if (!DEBUG_INFOS) return;
		fprintf(fichier_status,contenu);
		fflush(fichier_status);
	}

int demande_autorisation (char* message)
{
	if ( MessageBox(NULL, message, "Alerte réseau", MB_YESNO) == IDYES)
		return TRUE;
	else
		return FALSE;
}

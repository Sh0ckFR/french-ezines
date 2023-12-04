//tolwin_kernel.h
#define	DWORD	unsigned long
#define	BOOL	unsigned long
#define	WORD	unsigned int
#define	UINT	unsigned int
#define	BYTE	unsigned char


#include "ntddk.h"
#include "ntstrsafe.h"



// TYPES UTILISES ===============================================================================
	// Le device extension
	typedef struct {
		PDEVICE_OBJECT pMouseOldTopOfStack;
		BOOL autofire;
		int pending;
		PCHAR buffer;
		PIRP MouseIrp;
		BOOL latch;
		BOOL terminate_thread;
		HANDLE thread_handle;
		
	}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

	// Les données d'un IRP souris
	typedef struct {
		USHORT UnitId;
		USHORT flags;
		union {
			ULONG Buttons;
			struct {
				USHORT ButtonFlags;
				USHORT ButtonData;
			};
		};
		ULONG RawButtons;
		long LastX;
		long LastY;
		ULONG ExtraInformation;
	} MOUSE_INPUT_DATA, *PMOUSE_INPUT_DATA;



// PROTOTYPES DES FONCTIONS ====================================================================
	// Utilitaires
	UINT IsStringTerminatedAndSize	(PCHAR pString, UINT uiLength);
	
	//Driver
	VOID	 Unload					(IN PDRIVER_OBJECT  pDriverObject);
	
	//Device
	NTSTATUS PasTouche 				( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp );
	NTSTATUS FiltreLecture 				( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp );
	NTSTATUS ConfigurationDriver 		( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp );
	NTSTATUS CompletionRoutine		( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp, IN PVOID Context);


	void the_thread (IN PVOID pcontext);




//KeGetCurrentIrql()
/*#define PASSIVE_LEVEL            0      // Passive release level
#define LOW_LEVEL                0      // Lowest interrupt level
#define APC_LEVEL                1      // APC interrupt level
#define DISPATCH_LEVEL           2      // Dispatcher level
#define CMC_LEVEL                3      // Correctable machine check level
#define DEVICE_LEVEL_BASE        4      // 4 - 11 - Device IRQLs
#define PC_LEVEL                12      // Performance Counter IRQL
#define IPI_LEVEL               14      // IPI IRQL
#define CLOCK_LEVEL             13      // Clock Timer IRQL
#define POWER_LEVEL             15      // Power failure level
#define PROFILE_LEVEL           15      // Profiling level
#define HIGH_LEVEL              15      // Highest interrupt level
*/

// DRIVER ENTRY POINT ===========================================================================
NTSTATUS DriverEntry(IN PDRIVER_OBJECT  pDriverObject, IN PUNICODE_STRING  pRegistryPath)
{
	//Variables regroupées sinon bug de compilation à cause des DbgPrint
		//Divers
		NTSTATUS status = STATUS_UNSUCCESSFUL;
		int mj_function;
		//Création du device
		PDEVICE_OBJECT pMouseDeviceObject;
		PDEVICE_EXTENSION pDeviceExtension; 
		//Attache du device au stack souris
		CCHAR NomDriverStack[] = "\\Device\\PointerClass0";
		STRING NomDriverStackString;
		UNICODE_STRING uNomDriverStack;

		LARGE_INTEGER offset;



	//Le code de DriverEntry
		//Petit texte de debut d'exécution
		DbgPrint("\r\n\r\ntest_driver> Execution de DriverEntry\r\n");
		DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());


		//Initialise les irp majeures sur la fonction de dispatch de base
		DbgPrint("test_driver> Initialisation des fonctions majeures du driver filtrant\r\n");
		for (mj_function = 0; mj_function < IRP_MJ_MAXIMUM_FUNCTION; mj_function++)
			pDriverObject->MajorFunction[mj_function] = PasTouche;

		//Règle les irps qui auront un traitement spécifiques, les veinardes
		pDriverObject->MajorFunction[IRP_MJ_READ] = FiltreLecture;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ConfigurationDriver;
		pDriverObject->DriverUnload = Unload;

		//Création du device
		DbgPrint("test_driver> Creation du device de filtrage\r\n");
		status = IoCreateDevice (pDriverObject,sizeof(DEVICE_EXTENSION),NULL,FILE_DEVICE_MOUSE,0,TRUE,&pMouseDeviceObject);
		if (status != STATUS_SUCCESS)
		{
			//Si erreur alors cassos
			DbgPrint("test_driver> Erreur lors de la creation du device\r\n");
			return status;
		}
		else
			DbgPrint("test_driver> Device cree avec succes\r\n");

		//Initialisation du device extension : mise à zéro et utilisation d'un pointeur plus pratique à manipuler
		DbgPrint("test_driver> Initialisation du device de filtrage : mise a zero de la ram device extension\r\n");
		RtlZeroMemory(pMouseDeviceObject->DeviceExtension, sizeof(DEVICE_EXTENSION));
		pDeviceExtension = (DEVICE_EXTENSION*)pMouseDeviceObject->DeviceExtension;

		//Attache le device a la premiere device stack de classe souris
		DbgPrint("test_driver> Attache le device de filtrage sur %s\r\n",NomDriverStack);
		RtlInitAnsiString(&NomDriverStackString,NomDriverStack);
		RtlAnsiStringToUnicodeString(&uNomDriverStack,&NomDriverStackString,TRUE);
		status = IoAttachDevice(pMouseDeviceObject, &uNomDriverStack,&pDeviceExtension->pMouseOldTopOfStack);
		RtlFreeUnicodeString(&uNomDriverStack);
		if (status != STATUS_SUCCESS)
		{
			//Si erreur alors efface le device et cassos
			DbgPrint("test_driver> Erreur lors de l'insertion du device sur %s\r\n",NomDriverStack);
			IoDeleteDevice (pMouseDeviceObject);
			return status;
		}
		else
			DbgPrint("test_driver> Device insere avec succes sur %s\r\n",NomDriverStack);

		//Paramétrage de notre device filtrant notamment au moyen de valeurs provenant du driver souris original
		DbgPrint("test_driver> Parametrage du device\r\n");
		pMouseDeviceObject->Flags |= DO_BUFFERED_IO; //(pDeviceExtension->pMouseOldTopOfStack->Flags & DO_BUFFERED_IO)
		pMouseDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
		pMouseDeviceObject->DeviceType = pDeviceExtension->pMouseOldTopOfStack->DeviceType;
		pMouseDeviceObject->Characteristics = pDeviceExtension->pMouseOldTopOfStack->Characteristics;


/*
		//Lance le thread systeme
		pDeviceExtension->terminate_thread = FALSE;
		status = PsCreateSystemThread (
			&pDeviceExtension->thread_handle,
			(ACCESS_MASK)0,
			NULL,
			(HANDLE)0,
			NULL,
			the_thread,			//Adresse du thread
			pDeviceExtension		//Pagametre
			);
		if (status != STATUS_SUCCESS)
		{
			//Si erreur alors efface le device et cassos
			DbgPrint("Erreur lors de ca creation du thread\r\n");
			IoDetachDevice(pDeviceExtension->pMouseOldTopOfStack);
			IoDeleteDevice (pMouseDeviceObject);
			return status;
		}
		else
			DbgPrint("Thread cree avec succes\r\n");


		//Alloue le buffer pour l'IRP
		pDeviceExtension->buffer = ExAllocatePoolWithTag (NonPagedPool, sizeof(MOUSE_INPUT_DATA), 'Mous');
		if (pDeviceExtension->buffer == NULL)
		{
			//Si erreur alors detache et efface le device et cassos
			DbgPrint("Erreur lors de l'allocation du buffer\r\n");
			IoDetachDevice(pDeviceExtension->pMouseOldTopOfStack);
			IoDeleteDevice (pMouseDeviceObject);
			return status;
		}
		else
			DbgPrint("Buffer alloue avec succes\r\n");

		//Créé l'irp
		offset.QuadPart = 0;
		pDeviceExtension->MouseIrp = IoBuildAsynchronousFsdRequest( IRP_MJ_READ, pDeviceExtension->pMouseOldTopOfStack, pDeviceExtension->buffer, sizeof(MOUSE_INPUT_DATA), &offset, NULL);
		if (pDeviceExtension->MouseIrp == NULL)
		{
			//Si erreur alors detache et efface le device et cassos
			DbgPrint("Erreur lors de la creation de l'irp souris\r\n");
			IoDetachDevice(pDeviceExtension->pMouseOldTopOfStack);
			IoDeleteDevice (pMouseDeviceObject);
			return status;
		}
		else
			DbgPrint("Irp souris creee avec succes\r\n");
*/

		//Fin du driver entry
		return status;
}





// FONCTION DE DECHARGEMENT =====================================================================
VOID Unload(IN PDRIVER_OBJECT  pDriverObject)
{    
	PDEVICE_EXTENSION pDeviceExtension; 

	DbgPrint("test_driver> Execution de FilterUnload\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());
//	__asm int 3
	
	pDeviceExtension = pDriverObject->DeviceObject->DeviceExtension;
	DbgPrint("test_driver> pending : %i\r\n",pDeviceExtension->pending );

	do 	DbgPrint("test_driver> Wait pending\r\n");
		while (pDeviceExtension->pending > 0);

	//Séparation
//	__asm int 3
//	IoDetachDevice(pDeviceExtension->pMouseOldTopOfStack);

	//Efface le device
//	__asm int 3
//	IoDeleteDevice(pDriverObject->DeviceObject);

}


// QUELQUES FONCTIONS QUI APPELLENT L'ANCIEN DRIVER ====================================================
NTSTATUS ConfigurationDriver ( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp )
{
	PDEVICE_EXTENSION pDeviceExtension; 

	DbgPrint("test_driver> Execution de ConfigurationDriver\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());

	pDeviceExtension = pDeviceObject->DeviceExtension;

    	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(pDeviceExtension->pMouseOldTopOfStack, Irp);
}


NTSTATUS PasTouche ( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp )
{
	PDEVICE_EXTENSION pDeviceExtension; 

	DbgPrint("test_driver> Execution de PasTouche\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());

	
	pDeviceExtension = pDeviceObject->DeviceExtension;

    	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(pDeviceExtension->pMouseOldTopOfStack, Irp);
}




// LA FONCTION DE LECTURE ET SON CALLBACK ============================================================

NTSTATUS FiltreLecture ( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp )
{
	PDEVICE_EXTENSION pDeviceExtension; 
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	NTSTATUS status = STATUS_UNSUCCESSFUL;


	DbgPrint("test_driver> Execution de FiltreLecture\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());


	pDeviceExtension = pDeviceObject->DeviceExtension;

	//Chope les positions dans l'irp stack
   	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
   	IoCopyCurrentIrpStackLocationToNext(Irp);

	//Mise en place de la completion routine
	DbgPrint("test_driver> Regle CompletionRoutine et down the stack\r\n");
	IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) CompletionRoutine, NULL, TRUE, TRUE, TRUE);

	//Appel au driver d'origine
	pDeviceExtension->pending++;
	status = IoCallDriver(pDeviceExtension->pMouseOldTopOfStack, Irp);
	pDeviceExtension->pending--;

	return status;
}





NTSTATUS CompletionRoutine(PDEVICE_OBJECT pDeviceObject, PIRP Irp, PVOID Context)
{
    	PMOUSE_INPUT_DATA pMouseData;
	PDEVICE_EXTENSION pDeviceExtension; 

	DbgPrint("test_driver> Execution de CompletionRoutine\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());

	pMouseData = (PMOUSE_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;
	pDeviceExtension = pDeviceObject->DeviceExtension;


	//Teste pour l'autofire : si appel d'origine et que le bouton 1 est en jeu alors... ...
	if (pMouseData->UnitId == 0)
	{
		if (pMouseData->Buttons & 1)
			{
				pDeviceExtension->autofire = TRUE;
				pDeviceExtension->latch=TRUE;
			 	DbgPrint("test_driver> Autofire on\r\n");
			}
			else if (pMouseData->Buttons & 2)
			{
				pDeviceExtension->autofire = FALSE;
			    	DbgPrint("test_driver> Autofire off\r\n");
			}
	}

	if (pDeviceExtension->autofire == TRUE)
	{
		if (pDeviceExtension->latch == TRUE)
		{
			//Ici je passe le bouton à 1
			pMouseData->Buttons &= 0xFFFD;
			pMouseData->Buttons |= 0x0001;
			pDeviceExtension->latch = FALSE;
		}
		else
		{
			//Ici je passe le bouton à 2
			pMouseData->Buttons &= 0xFFFE;
			pMouseData->Buttons |= 0x0002;
			pDeviceExtension->latch = TRUE;
		}
	}

	if (Irp->PendingReturned)
		IoMarkIrpPending(Irp);
	
	return Irp->IoStatus.Status;
}



void the_thread (IN PVOID pcontext)
{
	PDEVICE_EXTENSION pDeviceExtension = (PDEVICE_EXTENSION) pcontext;
	PMOUSE_INPUT_DATA pMouseData;
	PIO_STACK_LOCATION nextstack;

	DbgPrint("test_driver> Execution de void the_thread\r\n");
	DbgPrint("test_driver> Irql : %i\r\n",KeGetCurrentIrql());

	pMouseData = pDeviceExtension->MouseIrp->AssociatedIrp.SystemBuffer;
	nextstack = IoGetNextIrpStackLocation(pDeviceExtension->MouseIrp);
	nextstack->MajorFunction = IRP_MJ_READ;
	nextstack->Parameters.Read.Length = sizeof(MOUSE_INPUT_DATA);


while (1)
{
	if (pDeviceExtension->autofire == TRUE)
	{
		if (pDeviceExtension->latch == TRUE)
		{
			RtlZeroMemory(pMouseData, sizeof(MOUSE_INPUT_DATA));
			pMouseData->UnitId = 1;
			pMouseData->flags = 1;
			pMouseData->Buttons = 1;
			pMouseData->RawButtons = 0;
			pMouseData->LastX = 0;
			pMouseData->LastY = 0;
			pMouseData->ExtraInformation = 0;
		//	pDeviceExtension->MouseIrp->IoStatus.Status = STATUS_SUCCESS;
			//pDeviceExtension->MouseIrp->IoStatus.Information = sizeof(MOUSE_INPUT_DATA);
			if (pDeviceExtension->pending == 0) IoCallDriver(pDeviceExtension->pMouseOldTopOfStack, pDeviceExtension->MouseIrp);
		
			pDeviceExtension->latch = FALSE;
		 	DbgPrint("test_driver> Fire !!!\r\n");
		}
		else
		{
			RtlZeroMemory(pMouseData, sizeof(MOUSE_INPUT_DATA));
			pMouseData->UnitId = 1;
			pMouseData->flags = 1;
			pMouseData->Buttons = 2;
			pMouseData->RawButtons = 0;
			pMouseData->LastX = 0;
			pMouseData->LastY = 0;
			pMouseData->ExtraInformation = 0;
			//pDeviceExtension->MouseIrp->IoStatus.Status = STATUS_SUCCESS;
			//pDeviceExtension->MouseIrp->IoStatus.Information = sizeof(MOUSE_INPUT_DATA);
			if (pDeviceExtension->pending == 0) IoCallDriver(pDeviceExtension->pMouseOldTopOfStack, pDeviceExtension->MouseIrp);
		
			pDeviceExtension->latch = TRUE;
		}
	}
}

}


// QUELQUES FONCTIONS UTILES ====================================================================
UINT IsStringTerminatedAndSize(PCHAR pString, UINT uiLength)
{
    UINT uiIndex = 0;

    while(uiIndex < uiLength)
    {
        if(pString[uiIndex] == '\0')
        {
            return uiIndex+1;
        }
        else
        {
           uiIndex++;
        }
    }

    return 0;
}


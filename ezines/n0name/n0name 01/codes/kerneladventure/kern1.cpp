NTSTATUS analyse (HANDLE hFichierStatus)
{
	char buffer[1024];

	DWORD processor_control_region;
	DWORD KdVersionBlock;
	DWORD PsActiveProcessHead;
	DWORD eprocess;
	DWORD ethread;
	unsigned int ActiveThreads;
	unsigned int i;
		
	unsigned int  UniqueProcessId;
	unsigned int  UniqueThreadId;
	char* ImageFileName;
	
	//Partie 1 : se place sur le premier processus
	__asm {
		mov eax, dword ptr fs:[0x1C]			//KPCR -> SelfPCR
		mov processor_control_region,eax
		mov eax,[eax+0x34]					//KPCR -> KdVersionBlock
		mov KdVersionBlock,eax
		mov eax,[eax+0x78]					//KDDEBUGGER_DATA32 -> PsActiveProcessHead
		mov PsActiveProcessHead,eax
		mov eax,[eax+0x0]					//PsActiveProcessHead -> Flink
		sub eax,0x88							//(eprocess -> ActiveProcessLink) - eprocess begin
		mov eprocess,eax
		}



	RtlStringCbPrintfA(buffer,1024,"Processor Control Region (KPCR) : %#08X\n",processor_control_region);
	ecris_fichier_log (hFichierStatus, buffer);

	 RtlStringCbPrintfA(buffer,1024,"KdVersionBlock : %#08X\n",KdVersionBlock);
	ecris_fichier_log (hFichierStatus, buffer);

	RtlStringCbPrintfA(buffer,1024,"PsActiveProcessHead : %#08X\n",PsActiveProcessHead);
	ecris_fichier_log (hFichierStatus, buffer);

	RtlStringCbPrintfA(buffer,1024,"Processus de tête : %#08X\n",eprocess);
	ecris_fichier_log (hFichierStatus, buffer);

	do {
		//Récupère les valeurs importantes du processus
		__asm {
			mov eax,eprocess
			mov eax,[eax+0x084]				//eprocess -> UniqueProcessId
			mov UniqueProcessId,eax	
	
			mov eax,eprocess
			add eax,0x174					//eprocess -> ImageFileName
			mov ImageFileName,eax

			mov eax,eprocess
			mov eax,[eax+0x1a0]				//eprocess -> ActiveThreads
			mov ActiveThreads,eax

			mov eax,eprocess
			mov eax,[eax+0x050]				//eprocess -> ThreadListHead
			sub eax,0x1b0					//(ethread -> ThreadListEntry) - ethread begin
			mov ethread,eax
			}

		//Affiche les infos du processus
		RtlStringCbPrintfA(buffer,1024,"\nProcessus %s : PID %u, %u thread(s)\n",ImageFileName,UniqueProcessId,ActiveThreads);
		ecris_fichier_log (hFichierStatus, buffer);

		//Parcours les threads
		for (i=0;i<ActiveThreads;i++)
		{
			__asm {
				mov eax,ethread
				mov eax,[eax+0x1f0]			//ethread -> UniqueThread
				mov UniqueThreadId, eax

				mov eax,ethread
				mov eax,[eax+0x1b0]			//ethread -> UniqueThread
				sub eax,0x1b0				//(ethread -> ThreadListEntry) - ethread begin
				mov ethread, eax
				}

			//Affiche les infos du thread
			RtlStringCbPrintfA(buffer,1024,"     TID % 5u\n",UniqueThreadId);
			ecris_fichier_log (hFichierStatus, buffer);
		}

		//Passe au processus suivant
		__asm{
			mov eax,eprocess
			mov eax,[eax+0x088]
			sub eax,0x88						//(eprocess -> ActiveProcessLink) - eprocess begin
			mov eprocess,eax
			}
	
	}while ((eprocess+0x88) != PsActiveProcessHead);  //(eprocess -> ActiveProcessLink) != PsActiveProcessHead

	
	return STATUS_SUCCESS;
}
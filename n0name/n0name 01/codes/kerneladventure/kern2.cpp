	//Se place sur le premier module
		__asm {
		mov eax, KdVersionBlock
		mov eax,[eax+0x70]					//KDDEBUGGER_DATA32 -> PsLoadedModulesList
		mov PsLoadedModuleList,eax
		mov eax,[eax+0x0]					//PsLoadedModuleList -> Flink
		mov module,eax
		}

	do {
		//Chope les infos
		__asm{
		mov eax,module
		mov eax,[eax+0x018]					//base
		mov base,eax

		mov eax,module
		mov eax,[eax+0x01c]					//driver_start
		mov driver_start,eax

		mov eax,module
		mov eax,[eax+0x030]					//driver_name
		mov driver_name,eax

		mov eax,module
		mov eax,[eax+0x0]					//flink
		mov module,eax
			}

		//Affiche les infos du module
		RtlStringCbPrintfA(buffer,1024,"module %S : base %#08x, start %#08x\n",driver_name,base,driver_start);
		ecris_fichier_log (hFichierStatus, buffer);


		}while (module != PsLoadedModuleList );





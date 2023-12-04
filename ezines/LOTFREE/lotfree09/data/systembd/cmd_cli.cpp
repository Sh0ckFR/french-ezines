/* LOTFREE 9
 * Client pour la backdoor Windows locale
 * Get NT System privileges
 */
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>

#define BUFSIZE 512

int _tmain(int argc, TCHAR *argv[])
{
    HANDLE hPipe;
    char message[500]; // buffer pour la saisie a la console (stdin)
	TCHAR wmessage[500]; // buffer de conversion en widechars
    TCHAR  chBuf[BUFSIZE]; // Buffer de lecture sur le pipe
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	DWORD err = 0;
    LPTSTR lpszPipename = _T("\\\\.\\pipe\\mynamedpipe");
	size_t convertedChars = 0;

    // Try to open a named pipe; wait for it, if necessary.
    while(1)
    {
        hPipe = CreateFile(
            lpszPipename,   // pipe name
            GENERIC_READ |  // read and write access
            GENERIC_WRITE,
            0,              // no sharing
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,              // default attributes
            NULL);          // no template file

        // Break if the pipe handle is valid.

        if(hPipe != INVALID_HANDLE_VALUE) break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs.

		err = GetLastError();
        
		if(err == ERROR_ACCESS_DENIED)
		{
			_tprintf(_T("Acces refuse!\n"));
			return -1;
		}
		else if(err != ERROR_PIPE_BUSY)
        {
            _tprintf( _T("Could not open pipe. GLE=%lu\n"), GetLastError());
            return -1;
        }

        // All pipe instances are busy, so wait for 20 seconds.
        if(!WaitNamedPipe(lpszPipename, 20000))
        {
            _tprintf(_T("Could not open pipe: 20 second wait timed out."));
            return -1;
        }
    } // fin while accès pipe

    // The pipe connected; change to message-read mode.
    dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle
        &dwMode,  // new pipe mode
        NULL,     // don't set maximum bytes
        NULL);    // don't set maximum time
    if (!fSuccess)
    {
        _tprintf( _T("SetNamedPipeHandleState failed. GLE=%lu\n"), GetLastError() );
        return -1;
    }

	memset((void *)&chBuf, 0, sizeof(TCHAR) * BUFSIZE);

	//  On lit la banniere d'accueil
    fSuccess = ReadFile(
                   hPipe,    // pipe handle
                   chBuf,    // buffer to receive reply
                   BUFSIZE * sizeof(TCHAR),  // size of buffer
                   &cbRead,  // number of bytes read
                   NULL);    // not overlapped
	chBuf[cbRead / sizeof(TCHAR)] = '\0';
	_tprintf(_T("%s\n"), chBuf);

    while(1)
    {
        // Send a message to the pipe server.
		memset(message, 0, 499);
        fgets(message, 499, stdin);
        cbToWrite = strlen(message);
		mbstowcs_s(&convertedChars, wmessage, cbToWrite, message, _TRUNCATE);
        if (strncmp(message, "QUIT", 4) == 0) break;

        fSuccess = WriteFile(
            hPipe,                  // pipe handle
            wmessage, //lpvMessage,             // message
            convertedChars * sizeof(TCHAR), //cbToWrite + 1,          // message length
            &cbWritten,             // bytes written
            NULL);                  // not overlapped

        if (!fSuccess)
        {
            _tprintf(_T("WriteFile to pipe failed. GLE=%lu\n"), GetLastError() );
            return -1;
        }

		memset((void *)&chBuf, 0, sizeof(TCHAR) * BUFSIZE);
        do
        {
            // Read from the pipe.
            fSuccess = ReadFile(
                hPipe,    // pipe handle
                chBuf,    // buffer to receive reply
                BUFSIZE * sizeof(TCHAR),  // size of buffer
                &cbRead,  // number of bytes read
                NULL);    // not overlapped
             chBuf[cbRead / sizeof(TCHAR)] = '\0';

            if (!fSuccess && GetLastError() != ERROR_MORE_DATA) break;

            _tprintf(_T("%s\n"), chBuf);
        } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA

        if(!fSuccess)
        {
            _tprintf(_T("ReadFile from pipe failed. GLE=%d\n"), (int)GetLastError());
            return -1;
        }
    }

    CloseHandle(hPipe);
    return 0;
}

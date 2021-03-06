// patrao1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

HANDLE	hEvent;
TCHAR cmd[80];

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// Event
	hEvent = CreateEvent(NULL, true, false, TEXT("evento"));

	if (hEvent == NULL) {
		_tprintf(TEXT("CreateEvent error: %d\n"), GetLastError());
		return 1;
	}

	_tprintf(TEXT("cmd: \"folga\" ou \"avisar\"\n"));
	do {
		_tprintf(TEXT("#: "));
		_fgetts(cmd, 80, stdin);
		if (_tcsncmp(cmd, TEXT("avisar"), 6) == 0) {
			SetEvent(hEvent);
			_tprintf(TEXT("\n15s"));
			Sleep(15000);
			ResetEvent(hEvent);
		}
	} while (_tcsncmp(cmd, TEXT("folga"), 5) != 0);

	_tprintf(TEXT("\nfim"));
	CloseHandle(hEvent);
	return 0;
}
// patrao.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

HANDLE	hSem;
TCHAR cmd[80];

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// Semaphore
	hSem = CreateSemaphore(NULL, 0, 2, TEXT("boss"));

	if (hSem == NULL) {
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}

	_fgetts(cmd, 80, stdin);
	
	if (_tcsncmp(cmd, TEXT("folga"), 5) == 0)
		_tprintf(TEXT("faz qq coisa.."));

	if (_tcsncmp(cmd, TEXT("avisar"), 6) == 0)
		ReleaseSemaphore(hSem, 2, NULL);

	CloseHandle(hSem);
	
	//_tprintf(TEXT("\nfim"));
	return 0;
}
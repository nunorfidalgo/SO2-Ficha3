// empregado.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

HANDLE	hSem;

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// Semaphore
	hSem = OpenSemaphore(SYNCHRONIZE|SEMAPHORE_MODIFY_STATE, true, TEXT("boss"));
	if (hSem == NULL) {
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}

	WaitForSingleObject(hSem, INFINITE);
	_tprintf(TEXT("Inicio do trabalho...(%d)\n"), GetCurrentProcessId());
	Sleep(10000);
	_tprintf(TEXT("Fim do dia! (%d)\n"), GetCurrentProcessId());
	ReleaseSemaphore(hSem, 1, NULL);
	CloseHandle(hSem);
	return 0;
}
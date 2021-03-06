// empregado1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

HANDLE	hEvent;

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// Event
	hEvent = OpenEvent(SYNCHRONIZE, true, TEXT("evento"));
	if (hEvent == NULL) {
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}

	WaitForSingleObject(hEvent, INFINITE);
	_tprintf(TEXT("Inicio do trabalho...(%d)\n"), GetCurrentProcessId());
	Sleep(5000);
	_tprintf(TEXT("Trablhar 5 seg"), GetCurrentProcessId());
	_tprintf(TEXT("Fim do dia! (%d)\n"), GetCurrentProcessId());
	ReleaseSemaphore(hEvent, 1, NULL);
	CloseHandle(hEvent);
	return 0;
}
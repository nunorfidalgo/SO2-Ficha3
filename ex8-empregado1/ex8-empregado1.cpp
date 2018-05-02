// não é o produtor/consumidor.. mas  é parecido...
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define TAM 80

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	HANDLE	hEvent;
	TCHAR *cmd;

	HANDLE hMap;

	LARGE_INTEGER t;
	t.QuadPart = TAM * sizeof(TCHAR);

	//hFile = CreateFile(TEXT("dados.txt"), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READONLY, t.HighPart, t.LowPart, TEXT("shm"));
	cmd = (TCHAR *) MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, (SIZE_T)t.QuadPart);

	// Event
	hEvent = OpenEvent(SYNCHRONIZE, true, TEXT("evento"));
	if (hEvent == NULL) {
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}
	do {
		WaitForSingleObject(hEvent, INFINITE);
		_tprintf(TEXT("Li '%s' (%d bytes)\n"), cmd, _tcslen(cmd) + 1);

	} while (_tcsncmp(cmd, TEXT("fim"), 3) != 0);

	CloseHandle(hEvent);
	// Fechar ficheiro
	//CloseHandle(hFile);
	UnmapViewOfFile(cmd);
	CloseHandle(hMap);

	return 0;
}
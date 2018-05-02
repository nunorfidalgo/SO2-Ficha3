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

	HANDLE hFile, hMap;

	// Abrir ficheiro... (RW)
	hFile = CreateFile(TEXT("memdados.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	//hMap = CreateFileMapping(INVALID_HANDLE_VALUE, ..., TAM * sizeof(TCHAT));
	//hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, TAM * sizeof(TCHAR), NULL);
	LARGE_INTEGER t;
	t.QuadPart = TAM * sizeof(TCHAR);
	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, t.HighPart, t.LowPart, TEXT("shm"));
	//cmd = (TCHAR *) MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, TAM * sizeof(TCHAR));
	cmd = (TCHAR *) MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, (SIZE_T) t.QuadPart);

	// Event
	hEvent = CreateEvent(NULL, true, false, TEXT("evento"));
	if (hEvent == NULL) {
		_tprintf(TEXT("CreateEvent error: %d\n"), GetLastError());
		return 1;
	}
	do {
		_tprintf(TEXT("CMD: "));
		_fgetts(cmd, TAM, stdin);
		cmd[_tcslen(cmd) - 1] = TEXT('\0');
		_tprintf(TEXT("Esccrevi %d bytes: '%s'\n"), _tcslen(cmd)+1, cmd);
		SetEvent(hEvent);
		ResetEvent(hEvent);
	} while (_tcsncmp(cmd, TEXT("fim"), 3) != 0);

	CloseHandle(hEvent);
	// Fechar ficheiro
	CloseHandle(hFile);
	UnmapViewOfFile(cmd);
	CloseHandle(hMap);

	_tprintf(TEXT("\nfim"));
	return 0;
}
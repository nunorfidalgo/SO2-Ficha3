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
	TCHAR cmd[TAM];

	HANDLE hFile;
	DWORD nBytes;

	// Abrir ficheiro... (RW)
	hFile = CreateFile(TEXT("dados.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

		// Escrever no ficheiro...
		WriteFile(hFile, cmd, (_tcslen(cmd) + 1) * sizeof(TCHAR), &nBytes, NULL);

		_tprintf(TEXT("Esccrevi %d bytes: '%s'\n"), nBytes, cmd);

		SetEvent(hEvent);
		ResetEvent(hEvent);
	} while (_tcsncmp(cmd, TEXT("fim"), 3) != 0);

	CloseHandle(hEvent);
	
	// Fechar ficheiro
	CloseHandle(hFile);

	_tprintf(TEXT("\nfim"));
	return 0;
}
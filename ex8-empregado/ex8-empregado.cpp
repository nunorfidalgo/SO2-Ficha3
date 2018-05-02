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

	// Abrir ficheiro... (R)
	hFile = CreateFile(TEXT("dados.txt"), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Event
	hEvent = OpenEvent(SYNCHRONIZE, true, TEXT("evento"));
	if (hEvent == NULL) {
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}
	do {
		WaitForSingleObject(hEvent, INFINITE);

		// Ler ficheiro...
		ReadFile(hFile, cmd, TAM * sizeof(TCHAR), &nBytes, NULL);

		_tprintf(TEXT("Li '%s' (%d bytes)\n"), cmd, nBytes);

	} while (_tcsncmp(cmd, TEXT("fim"), 3) != 0);

	CloseHandle(hEvent);

	// Fechar ficheiro
	CloseHandle(hFile);

	return 0;
}
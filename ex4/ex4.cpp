// ex4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/* NOTAS:
$ ex4.exe 4 50 data.bin -> num_threads num ficheiro
// Semaphore
hSem = CreateSemaphore(NULL, 0, 10, TEXT("Sem"));

*/

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

DWORD WINAPI Contar(LPVOID param);

typedef struct {
	int nt, num;
} PARAM;

HANDLE	hSem;
int nsearch;
TCHAR *fname;

int total = 0;

int _tmain(int argc, LPTSTR argv[]) {
	int i, nthread, nP, tam;
	FILE	*f;

	DWORD	*threatId;
	HANDLE	*hT;
	PARAM	*p;

	LARGE_INTEGER t;
	HANDLE hTimer;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	if (argc != 4) {
		_tprintf(TEXT("\nusage: nT nP <ficheiro>, nT -> numero threads, nP -> numero a pesquisar..."));
		return 1;
	}

	nthread = _ttoi(argv[1]);
	nP = _ttoi(argv[2]);

	fname = argv[3];
	_tprintf(TEXT("Nome ficheiro: %s"), fname);

	// saber tamanho do ficheiro
	if ((_tfopen_s(&f, argv[3], TEXT("rb"))) != 0) {
		_tprintf(TEXT("\nErro ao abrir ficheiro..."));
		return 1;
	}
	fseek(f, 0, SEEK_END);
	tam = ftell(f) / sizeof(int);
	fclose(f);
	_tprintf(TEXT("\nTamanho do ficheiro: %d bytes"), tam);

	// tempo
	_tprintf(TEXT("\nVou esperar 5 segundos..."));
	t.QuadPart = 5 * 1000000;
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(hTimer, &t, 0, NULL, NULL, 0);
	//...
	WaitForSingleObject(hTimer, INFINITE);

	threatId = (DWORD *)malloc(nthread * sizeof(DWORD));
	hT = (HANDLE *)malloc(nthread * sizeof(HANDLE));
	p = (PARAM *)malloc(nthread * sizeof(PARAM));

	// Semaphore
	hSem = CreateSemaphore(NULL, 0, 10, TEXT("Sem"));
	if (hSem == NULL){
		_tprintf(TEXT("CreateSemaphore error: %d\n"), GetLastError());
		return 1;
	}

	for (i = 0; i < nthread; i++) {
		p[i].nt = i;
		p[i].num = tam / nthread;

		hT[i] = CreateThread(NULL, 0, Contar, (LPVOID)&p[i], 0, &threatId[i]);
		if (hT[i] != NULL) {
			_tprintf(TEXT("\nLancei uma thread com id %d"), threatId[i]);
		}
	}

	WaitForMultipleObjects(nthread, hT, TRUE, INFINITE);
	CloseHandle(hSem);
	free(threatId);
	free(hT);

	_tprintf(TEXT("\n\nTotal: %d\n"), total);
	return 0;
}

/* ----------------------------------------------------- */
/* "Thread" - Funcao associada à Thread */
/* ----------------------------------------------------- */
DWORD WINAPI Contar(LPVOID param) {
	int i, pos, n;
	PARAM *y;
	FILE *f;
	y = (PARAM *)param;

	pos = y->nt * y->num * sizeof(int); //bytes

	_tprintf(TEXT(" - [T%d] a começar em %d bytes (#%d)"), y->nt, pos, y->num);
	Sleep(100);

	_tfopen_s(&f, fname, TEXT("rb"));
	fseek(f, pos, SEEK_SET);

	for (i = 0; i < y->num; i++) {
		fread(&n, sizeof(int), 1, f);
		if (n == nsearch) {
			WaitForSingleObject(hSem, INFINITE);
			total++;
			ReleaseSemaphore(hSem, 1, NULL);
		}
	}
	fclose(f);
	return 0;
}
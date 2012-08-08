
#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <malloc.h>
#include <stdio.h>
#include <TlHelp32.h>

#include "Injection.h"

BOOL InjectLibrary(HANDLE hProcess, char *fnDll)
{
	BOOL success = FALSE;
	HANDLE hThread = NULL;
	char *fnRemote = NULL;
	FARPROC procLoadLibraryA = NULL;

	size_t lenFilename = strlen(fnDll) + 1;

	/* Allocate space in the remote process */
	fnRemote = (char *) VirtualAllocEx(hProcess, NULL, lenFilename, MEM_COMMIT, PAGE_READWRITE);
	
	if(fnRemote)
	{
		/* Write the filename to the remote process. */
		if(WriteProcessMemory(hProcess, fnRemote, fnDll, lenFilename, NULL))
		{
			/* Get the address of the LoadLibraryA function */
			procLoadLibraryA = GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");
			hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) procLoadLibraryA, fnRemote, 0, NULL);
			if(hThread)
			{
				WaitForSingleObject(hThread, INFINITE);
				success = TRUE;
			}
		}
		VirtualFreeEx(hProcess, fnRemote, 0, MEM_RELEASE);
	}

	return success;
}

BOOL EjectLibrary(HANDLE hProcess, char *fnDll)
{
	BOOL success = FALSE;
	HANDLE hSnapshot = NULL;
	HANDLE hThread = NULL;
	FARPROC procFreeLibrary = NULL;
	int dwProcessId = GetProcessId(hProcess);

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	
	if(hSnapshot)
	{
		MODULEENTRY32 me = { sizeof(me) };
		BOOL isFound = FALSE;
		BOOL isMoreMods = Module32First(hSnapshot, &me);

		while(isMoreMods && !isFound)
		{
			isMoreMods = Module32Next(hSnapshot, &me);

			printf("Checking %s [0x%p]\n", me.szModule, me.modBaseAddr);
			isFound = (_strcmpi(me.szModule, fnDll) == 0 || _strcmpi(me.szExePath, fnDll) == 0);
		}
		printf("\nFound library: %s [0x%p]\n\n", me.szModule, me.modBaseAddr);

		if(isFound)
		{
			/* Get the address of the FreeLibrary function */
			procFreeLibrary = GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");
			printf("Freeing library at 0x%p\n", me.modBaseAddr);
			hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) procFreeLibrary, me.modBaseAddr, 0, NULL);
			if(hThread)
			{
				WaitForSingleObject(hThread, INFINITE);
				success = TRUE;
			}
		}

		CloseHandle(hSnapshot);	
	}

	return success;
}

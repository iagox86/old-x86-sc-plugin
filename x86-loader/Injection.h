#ifndef __INJECTION_H_
#define __INJECTION_H_

/* This is a simple implementation of .dll injection using CreateRemoteThread(). The structure
 * is taken from Jeffrey Richter's Programming Applications in Microsoft Windows, but it's 
 * almost entirely been re-written. */

#include <windows.h>

BOOL InjectLibrary(HANDLE hProcess, char *fnDll);
BOOL EjectLibrary(HANDLE hProcess, char *fnDll);

#endif
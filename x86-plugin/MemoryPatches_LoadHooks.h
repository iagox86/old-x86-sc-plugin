#ifndef __MEMORY_PATCHES_LOAD_HOOKS_H__
#define __MEMORY_PATCHES_LOAD_HOOKS_H__

#include <windows.h>

/** This file contains patches for load functions, and possibly, in the future, other library
 * functions. These are generally functions that are safe to patch immediately after loading 
 * the .dll -- that is, functions that, can't be detected and functions in default .dll's. */

void patches_addloadpatches();
void patches_removeloadpatches();

void __stdcall patchfunc_loadlibrary(char *library);
void __stdcall patchfunc_freelibrary(HMODULE library);
void __stdcall patchfunc_getprocaddress(HMODULE hModule, char *name);
void __stdcall patchfunc_getmodulehandlea(char *ModuleName);

#endif
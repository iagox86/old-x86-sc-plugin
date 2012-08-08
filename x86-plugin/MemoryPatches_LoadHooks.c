#include "Logging.h"
#include "MemoryPatch.h"
#include "MemoryPatches_StarcraftHooks.h"
#include "MemoryPatches_WinsockHooks.h"
#include "StarcraftAddresses.h"
#include "StarcraftState.h"

#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <malloc.h>
#include <stdio.h>
#include <TlHelp32.h>


#include "MemoryPatches_LoadHooks.h"

static t_memorypatch *load_library           = NULL;
static t_memorypatch *free_library           = NULL;
static t_memorypatch *getprocaddress         = NULL;
static t_memorypatch *getmodulehandlea       = NULL;

static BOOL display_system_calls = FALSE; /* Enable for debugging */

/* This prevents infinite recursion. */
static BOOL in_getprocaddress   = FALSE;
static BOOL in_getmodulehandlea = FALSE;

HMODULE handle_checkrevision = NULL;
HMODULE handle_extrawork = NULL;

static void patch_load_library()
{
	HMODULE addrKernel32 = GetModuleHandle("kernel32");
	FARPROC addrLoadLibraryA = GetProcAddress(addrKernel32, "LoadLibraryA");
	
	log_add("Patching LoadLibraryA()");
	if(!load_library)
	{
		load_library = mp_initialize_useful(addrLoadLibraryA, (void*) patchfunc_loadlibrary, 5);
		mp_add_memoryoffset_parameter(load_library, EBP, 8);
	}
	mp_apply(load_library);
}

static void unpatch_load_library()
{
	log_add("Unpatching LoadLibraryA()");
	if(load_library)
		mp_remove(load_library);
}

static void patch_free_library()
{
	HMODULE addrKernel32 = GetModuleHandle("kernel32");
	FARPROC addrFreeLibrary = GetProcAddress(addrKernel32, "FreeLibrary");

	log_add("Patching FreeLibrary()");
	if(!free_library)
	{
		free_library = mp_initialize_useful(addrFreeLibrary, patchfunc_freelibrary, 5);
		mp_add_memoryoffset_parameter(free_library, EBP, 8);
	}
	mp_apply(free_library);
}

static void unpatch_free_library()
{
	log_add("Unpatching FreeLibrary()");
	if(free_library)
		mp_remove(free_library);
}

static void patch_getprocaddress()
{
	HMODULE addrKernel32 = GetModuleHandle("kernel32");
	FARPROC addrGetProcAddress = GetProcAddress(addrKernel32, "GetProcAddress");

	log_add("Patching GetProcAddress()");
	if(!getprocaddress)
	{
		getprocaddress = mp_initialize_useful(addrGetProcAddress, (void *) patchfunc_getprocaddress, 5);
		mp_set_return_register(getprocaddress, EAX); /* ECX doesn't work here, for some reason.. */
		mp_add_memoryoffset_parameter(getprocaddress, EBP, 8);
		mp_add_memoryoffset_parameter(getprocaddress, EBP, 12);
	}
	mp_apply(getprocaddress);
}

static void unpatch_getprocaddress()
{
	log_add("Unpatching GetProcAddress()");
	if(getprocaddress)
		mp_remove(getprocaddress);
}

static void patch_getmodulehandlea()
{
	HMODULE addrKernel32 = GetModuleHandle("kernel32");
	FARPROC addrGetModuleHandleA = GetProcAddress(addrKernel32, "GetModuleHandleA");

	log_add("Patching GetModuleHandleA()");
	if(!getmodulehandlea)
	{
		getmodulehandlea = mp_initialize_useful(addrGetModuleHandleA, (void *) patchfunc_getmodulehandlea, 5);
		mp_set_return_register(getmodulehandlea, ECX);
		mp_add_memoryoffset_parameter(getmodulehandlea, EBP, 8);
	}
	mp_apply(getmodulehandlea);
}

static void unpatch_getmodulehandlea()
{
	log_add("Unpatching GetModuleHandleA()");
	if(getmodulehandlea)
		mp_remove(getmodulehandlea);
}

void patches_addloadpatches()
{
	log_add("Adding initial hooks");
	log_indent();
	patch_load_library();
	patch_free_library();
	/* One of these two seems to break the debugger, so no dice on them. */
	//patch_getprocaddress();
	//patch_getmodulehandlea();
	log_undent();
}

void patches_removeloadpatches()
{
	log_add("Removing load hooks.");
	log_indent();
	unpatch_load_library();
	unpatch_free_library();
	//unpatch_getprocaddress();
	//unpatch_getmodulehandlea();
	log_undent();
}

BOOL getModuleName(BYTE *address, char *buffer, int buffer_len)
{
	HANDLE hSnapshot = NULL;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(GetCurrentProcess()));
	
	if(hSnapshot)
	{
		MODULEENTRY32 me = { sizeof(me) };
		BOOL isMoreMods = Module32First(hSnapshot, &me);

		while(isMoreMods)
		{
			isMoreMods = Module32Next(hSnapshot, &me);

			if(me.modBaseAddr == address)
			{
				strcpy_s(buffer, buffer_len, me.szExePath);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void __stdcall patchfunc_loadlibrary(char *library)
{
	static int inside = FALSE; /* Protect from infinite recursion */

	if(display_system_calls)
		log_add("Called: LoadLibraryA(\"%s\")", library);

	if(!inside)
	{
		inside = TRUE;

		if(state_get() == SC_NEW && strstr(library, "ws2_32"))
		{
			state_set(SC_STARTINGCONNECT);
			patches_addwinsockpatches();
		}
		else if(state_get() == SC_STARTINGCONNECT && strstr(library, "IX86"))
		{
			handle_checkrevision = LoadLibrary(library); /* Save the address. */

			patches_removestarcraftpatches();
			state_set(SC_CHECKREVISION_HAPPENING);
		}
		else if(state_get() == SC_CHECKREVISION_OVER && strstr(library, "IX86"))
		{
			handle_extrawork = LoadLibrary(library); /* Save the address. */

			patches_removestarcraftpatches(); /* Remove patches for EXTRAWORK (just in case) */
			state_set(SC_EXTRAWORK_HAPPENING);
		}

		inside = FALSE;
	}
}

void __stdcall patchfunc_freelibrary(HMODULE library)
{
	static int inside = FALSE; /* Protect from infinite recursion */

	if(!inside)
	{
		inside = TRUE;

		if(display_system_calls)
		{
			char buffer[256];
			getModuleName((byte*)library, buffer, 255);
			log_add("Called: FreeLibrary(0x%p) [%s]", library, buffer);
		}

		if(state_get() == SC_CHECKREVISION_HAPPENING && library == handle_checkrevision)
		{
			FreeLibrary(handle_checkrevision);
			handle_checkrevision = NULL;

			state_set(SC_CHECKREVISION_OVER);
			patches_addstarcraftpatches(); /* Add the Starcraft patches after CHECKREVISION */
		}
		else if(state_get() == SC_EXTRAWORK_HAPPENING && library == handle_extrawork)
		{
			FreeLibrary(handle_extrawork);
			handle_extrawork = NULL;

			state_set(SC_EXTRAWORK_OVER);
			patches_addstarcraftpatches(); /* Re-add patches (they were removed for EXTRAWORK) */
		}
		else if(state_get() == SC_EXTRAWORK_OVER && library == GetModuleHandle("Battle.snp"))
		{
			state_set(SC_NEW);

			patches_removestarcraftpatches();
		}

		if(library == GetModuleHandle("ws2_32.dll"))
		{
			patches_removewinsockpatches();

			if(state_get() == SC_EXTRAWORK_OVER)
			{
				state_set(SC_NEW);
				patches_removestarcraftpatches();
			}
		}

		inside = FALSE;
	}
}

void __stdcall patchfunc_getprocaddress(HMODULE hModule, char *name)
{
	if(!in_getprocaddress)
	{
		in_getprocaddress = TRUE;
		if(display_system_calls)
			log_add("Called: GetProcAddress -- looking for %p!%s", hModule, name);
		in_getprocaddress = FALSE;
	}
}

void __stdcall patchfunc_getmodulehandlea(char *ModuleName)
{
	if(!in_getmodulehandlea)
	{
		in_getmodulehandlea = TRUE;
		if(display_system_calls)
			log_add("Called: GetModuleHandleA(%s)", ModuleName);
		in_getmodulehandlea = FALSE;
	}
}
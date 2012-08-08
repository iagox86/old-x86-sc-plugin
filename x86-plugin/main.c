
#include "Logging.h"
#include "MemoryPatches_LoadHooks.h"
#include "MemoryPatches_StarcraftHooks.h"
#include "MemoryPatches_WinsockHooks.h"
#include "StarcraftAddresses.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		log_clear();
		log_add("Dll Attached.");
		/* Note: changing the resolution breaks the checksum, so don't enable these if you want
		 * to connect to Battle.net. */
//		WriteProcessMemory(GetCurrentProcess(), x_resolution, "\x00\x05", 2, NULL);
//		WriteProcessMemory(GetCurrentProcess(), y_resolution, "\x00\x04", 2, NULL);
		
		patches_addloadpatches();
		
		if(GetModuleHandle("battle.snp"))
			patches_addstarcraftpatches();

		if(GetModuleHandle("ws2_32.dll"))
			patches_addwinsockpatches();

		break;

	case DLL_PROCESS_DETACH:
		log_add("");
		log_add("Dll Detaching.");
		log_indent();

		patches_removeloadpatches();
		
		if(GetModuleHandle("battle.snp"))
			patches_removestarcraftpatches();

		if(GetModuleHandle("ws2_32.dll"))
			patches_removewinsockpatches();

		log_undent();
		log_add("Dll Detached.");


		break;
	}
    return TRUE;
}

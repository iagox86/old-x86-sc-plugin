#ifndef __MEMORY_PATCHES_WINSOCK_HOOKS_H__
#define __MEMORY_PATCHES_WINSOCK_HOOKS_H__

/** Patches in this function are for Winsock functions, and are applied when ws2_32.dll is 
 * loaded and removed when ws2_32.dll is unloaded. Strictly speaking, this isn't especially
 * useful for Starcraft, since I hook the processing functions anyways, but this is meant to 
 * be more of a generic demonstration. 
 *
 * In addition to seeing the data, it can also be manipulated since all the function parameters
 * are pointers. */

void patches_addwinsockpatches();
void patches_removewinsockpatches();


void __stdcall patchfunc_send(SOCKET *ptr_s, char **ptr_buf, int *ptr_len, int *ptr_flags);
void __stdcall patchfunc_sendto(SOCKET *ptr_s, char **ptr_buf, int *ptr_len, int *ptr_flags, void **to_ptr, int *tolen_ptr);
#endif
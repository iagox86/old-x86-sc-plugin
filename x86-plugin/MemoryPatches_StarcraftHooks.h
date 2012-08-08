#ifndef __MEMORY_PATCHES_STARCRAFT_HOOKS_H__
#define __MEMORY_PATCHES_STARCRAFT_HOOKS_H__

/** Patches in this function should be explicitly for Starcraft code. These patches are 
 * applied as soon as the ExtraWork .dll is unloaded, and they stick around until Battle.snp
 * or ws2_32.dll are unloaded. This prevents them from being detected by the version-checker. */

void patches_addstarcraftpatches();
void patches_removestarcraftpatches();


void __stdcall patchfunc_outgoinggamemessage(char *message);
BOOL __stdcall patchfunc_outgoingchannelmessage(int code, char *message);

BOOL __stdcall patchfunc_outgoingpregamemessage(char *message);
BOOL __stdcall patchfunc_outgoingingamemessage(char *message);

void __stdcall patchfunc_incomingchannelmessage(char *data, int length);

/** These are generic patch functions I use if I need to check a variable or two. */
void __stdcall patchfunc_display_one_parameter(int arg1);
void __stdcall patchfunc_display_two_parameters(int arg1, int arg2);
void __stdcall patchfunc_display_three_parameters(int arg1, int arg2, int arg3);
void __stdcall patchfunc_display_four_parameters(int arg1, int arg2, int arg3, int arg4);
void __stdcall patchfunc_display_five_parameters(int arg1, int arg2, int arg3, int arg4, int arg5);
void __stdcall patchfunc_display_six_parameters(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
void __stdcall patchfunc_display_seven_parameters(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);

#endif
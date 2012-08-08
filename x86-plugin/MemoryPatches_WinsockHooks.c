#include "Logging.h"
#include "MemoryPatch.h"
#include "StarcraftAddresses.h"

#include "MemoryPatches_WinsockHooks.h"

static t_memorypatch *send_patch             = NULL;
static t_memorypatch *sendto_patch           = NULL;

static BOOL display_winsock = TRUE;

static void patch_send()
{
	HMODULE addrWS2_32   = GetModuleHandle("ws2_32.dll");
	FARPROC addrSend     = GetProcAddress(addrWS2_32, "send");

	log_add("Patching send()");
	if(!send_patch)
	{
		send_patch = mp_initialize_useful(addrSend, (void*) patchfunc_send, 5);
		mp_add_pointeroffset_parameter(send_patch, EBP, ECX, 0x08);
		mp_add_pointeroffset_parameter(send_patch, EBP, ECX, 0x0c);
		mp_add_pointeroffset_parameter(send_patch, EBP, ECX, 0x10);
		mp_add_pointeroffset_parameter(send_patch, EBP, ECX, 0x14);
	}
	mp_apply(send_patch);
}

static void unpatch_send()
{
	log_add("Unpatching send()");
	if(send_patch)
		mp_remove(send_patch);
}


static void patch_sendto()
{
	HMODULE addrWS2_32   = GetModuleHandle("ws2_32.dll");
	FARPROC addrSendTo   = GetProcAddress(addrWS2_32, "sendto");

	log_add("Patching sendto()");
	if(!sendto_patch)
	{
		sendto_patch = mp_initialize_useful(addrSendTo, (void*) patchfunc_sendto, 5);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x08);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x0c);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x10);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x14);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x18);
		mp_add_pointeroffset_parameter(sendto_patch, EBP, ECX, 0x1c);
	}
	mp_apply(sendto_patch);
}

static void unpatch_sendto()
{
	log_add("Unpatching sendto()");
	if(sendto_patch)
		mp_remove(sendto_patch);
}

void patches_addwinsockpatches()
{
	log_add("Adding Winsock patches.");
	log_indent();
	patch_send();
	patch_sendto();
	log_undent();
}

void patches_removewinsockpatches()
{
	log_add("Removing Winsock patches.");
	log_indent();
	unpatch_send();
	unpatch_sendto();
	log_undent();
}

void __stdcall patchfunc_send(SOCKET *ptr_s, char **ptr_buf, int *ptr_len, int *ptr_flags)
{
	SOCKET s  = *ptr_s;
	char *buf = *ptr_buf;
	int len   = *ptr_len;
	static BOOL inside = FALSE;

	if(!inside)
	{
		inside = TRUE;

		if(display_winsock)
			log_add("Socket %d: sending %3d bytes, starting with %02x %02x %02x %02x.", s, len, buf[0] & 0x00FF, buf[1] & 0x00FF, buf[2] & 0x00FF, buf[3] & 0x00FF);	

//		if(len > 1)
//		{
//			if(buf[1] == 0x0E)
//			{
//				send_tcp(0x0E, "/join test\x00", 11);
//				display_game(10000, "This is a test?");
//			}
//		}

		inside = FALSE;
	}
}


void __stdcall patchfunc_sendto(SOCKET *ptr_s, char **ptr_buf, int *ptr_len, int *ptr_flags, void **to_ptr, int *tolen_ptr)
{
	SOCKET s = *ptr_s;
	char *buf = *ptr_buf;
	int len = *ptr_len;
	int flags = *ptr_flags;
	void *to = *to_ptr;

	if(display_winsock)
		log_add("Socket %d: sending [udp] %3d bytes, starting with %02x %02x %02x %02x.", s, len, buf[0] & 0x00FF, buf[1] & 0x00FF, buf[2] & 0x00FF, buf[3] & 0x00FF);	
}

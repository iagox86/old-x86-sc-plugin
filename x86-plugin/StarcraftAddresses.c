#include "Buffer.h"
#include "Logging.h"

#include "StarcraftAddresses.h"


void send_tcp(int packet_code, char *data, int len)
{
	//log_add("Sending out TCP packet 0x%02x (%d bytes)", packet_code & 0x000000FF, len);

	/* This sends out a tcp packet to Battle.net. For some reason, the length of the tcp 
	 * packet is sent in ebx, so that has to be preserved. */
	__asm
	{
		push    ebx      ; Preserve ebx
		push    data
		push    packet_code

		mov     ebx, len  ; For some reason, the length value is passed in ebx

		call    addr_function_send_tcp
		pop     ebx       ; Restore ebx
	}
}

BOOL send_command_from_game(char *command)
{
	int retval;

	__asm
	{
		push command
		call addr_function_outgoing_command
		mov retval, eax
	}

	return retval;
}

void display_game_raw(int ms_duration, int colour, char *message)
{
	int time_end = ms_duration + GetTickCount();

	__asm
	{
		push 0
		push time_end
		push colour
		mov eax, message
		call addr_function_display_game_message
	}
}

void display_channel_raw(int type, int flags, int ping, char *username, char *message)
{
	char *data;
	int len;

	t_buffer *buffer = buffer_initialize();
	buffer_insert_dword(buffer, type);
	buffer_insert_dword(buffer, flags);
	buffer_insert_dword(buffer, ping);
	buffer_insert_dword(buffer, 0x00000000);
	buffer_insert_dword(buffer, 0x00000000);
	buffer_insert_dword(buffer, 0x00000000);
	buffer_insert_ntstring(buffer, username);
	buffer_insert_ntstring(buffer, message);

	data = buffer_get_cstring(buffer);
	len  = buffer_get_length(buffer);
	

	__asm
	{
		mov eax, data
		mov ecx, len
		call addr_function_display_channel_message
	}

	buffer_destroy(buffer);
}

/** Getting the player colour is nasty -- I have to go through three translation tables! 
 * I don't know what the programmers were thinking here... 
 * 
 * Here is the original code (warning: it's UGLY):
 *   .text:0048C6DB E8 20 DA 00 00                    call    sub_49A100      ; Takes eax as a parameter, likely the player number
 *   .text:0048C6E0 0F B6 C0                          movzx   eax, al
 *   .text:0048C6E3 0F B6 80 9E 0D 58+                movzx   eax, byte ptr dword_580D9E[eax]
 *   .text:0048C6EA 83 C0 ED                          add     eax, -19
 *   .text:0048C6ED 3D A6 00 00 00                    cmp     eax, 0A6h
 *   .text:0048C6F2 77 68                             ja      short loc_48C75C
 *   .text:0048C6F4 0F B6 88 A4 C8 48+                movzx   ecx, ds:byte_48C8A4[eax]
 *   .text:0048C6FB FF 24 8D 64 C8 48+                jmp     ds:off_48C864[ecx*4]
 */
//char get_player_colour(int player)
//{
//	char colour_value;
//	char *jumpdest;
//
//	__asm
//	{
//		mov eax, player
//		call addr_function_colour_translator
//		mov colour_value, al
//	}
//
//	colour_value = addr_array_colour_translator_1[colour_value & 0x000000FF] - 19;
//	colour_value = addr_array_colour_translator_2[colour_value & 0x000000FF];
//	jumpdest = (char *) addr_array_colour_translator_3[colour_value & 0x000000FF];
//
//	return jumpdest[3];
//}
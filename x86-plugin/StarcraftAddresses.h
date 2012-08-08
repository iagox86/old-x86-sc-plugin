#ifndef __STARCRAFT_ADDRESSES_H__
#define __STARCRAFT_ADDRESSES_H__

#include <windows.h>

/** This function defines the addresses of a variety of addresses within Starcraft.exe and
 * Battle.snp. I realize it's bad practice to list a module's address directly, but that 
 * isn't terribly important here. 
 * 
 * Addresses here are for the original Starcraft.exe (STAR), not Brood War; however, the 
 * addresses might be the same, I don't remember. 
 * 
 * All functions that directly call a Starcraft function can be found in the corresponding
 * .c file, and I tried to list the way in which I found the address (where possible), which
 * should help with re-discovering it after an update. */


/** The address of the function that displays messages on the screen. 
 * Found by searching for "%s: %s":
 *  .text:0048C840 6A 00                             push    0               ; char
 *  .text:0048C842 FF 15 C4 D0 4F 00                 call    ds:GetTickCount
 *  .text:0048C848 8B 4D FC                          mov     ecx, dword ptr [ebp+ColorMaybe]
 *  .text:0048C84B 05 58 1B 00 00                    add     eax, 7000
 *  .text:0048C850 50                                push    eax             ; time_to_stop
 *  .text:0048C851 51                                push    ecx             ; char
 *  .text:0048C852 8D 85 A4 FE FF FF                 lea     eax, [ebp+display_buffer]
 *  .text:0048C858 E8 C3 F8 FF FF                    call    DisplayMessage
 */
static void *addr_function_display_game_message = (void *) 0x0048C9D0; // 1.15.2

/** This is identical to addr_patch_incoming_channel_message (below). */
static void *addr_function_display_channel_message = (void *) 0x19028CF0; // 1.15.2

/** This can be found within the connection sequence:
.text:1902E09D                 push    edx             ; buffer
.text:1902E09E                 push    50h             ; packet_code
.text:1902E0A0                 mov     ebx, eax
.text:1902E0A2                 call    sub_190298A0    ; ebx = the length
*/
static void *addr_function_send_tcp = (void *) 0x190298A0; // 1.15.2

/* This address was found by searching for the string "squelch" and going up to the top
 * of the function it was found in: 
.text:0047FC60 55                                      push    ebp
.text:0047FC61 8B EC                                   mov     ebp, esp
.text:0047FC63 81 EC 08 01 00 00                       sub     esp, 108h
.text:0047FC69 57                                      push    edi
.text:0047FC6A 8B 7D 08                                mov     edi, [ebp+arg_0]
.text:0047FC6D 80 3F 2F                                cmp     byte ptr [edi], 2Fh
.text:0047FC70 74 09                                   jz      short loc_47FC7B
.text:0047FC72 33 C0                                   xor     eax, eax
.text:0047FC74 5F                                      pop     edi
.text:0047FC75 8B E5                                   mov     esp, ebp
.text:0047FC77 5D                                      pop     ebp
.text:0047FC78 C2 04 00                                retn    4

 * The '/' comparison clinched it. This patches over the first 9 bytes in that function, 
 * up to the end of the "sub esp" line. */
static void *addr_function_outgoing_command = (void*) 0x0047FC60; // 1.15.2

/** This function translates a player number to a code for the corresponding colour. The
 * process of changing the returned value into a colour code is still tricky, but this
 * is the first step. This can be found in the big switch in the function containing 
 * "%s: %s". */
//static void *addr_function_colour_translator = (void *) 0x0049A100;

/** These three arrays are also used in the colour translator. The first and second change
 * the number to another number, and the final one is a jump table for a switch(). */
//static char *addr_array_colour_translator_1 = (char *) 0x580D9E;
//static char *addr_array_colour_translator_2 = (char *) 0x48C8A4;
//static int **addr_array_colour_translator_3 = (int **) 0x48C864;


/* Found by going one back from addr_function_outgoing_command and overwriting the "call" line.
.text:00470BD0 55                                      push    ebp
.text:00470BD1 8B EC                                   mov     ebp, esp
.text:00470BD3 83 EC 7C                                sub     esp, 7Ch
.text:00470BD6 57                                      push    edi
.text:00470BD7 E8 84 F0 00 00                          call    sub_47FC60 **********
.text:00470BDC 85 C0                                   test    eax, eax
.text:00470BDE 75 54                                   jnz     short loc_470C34
.text:00470BE0 8B C7                                   mov     eax, edi
.text:00470BE2 8D 50 01                                lea     edx, [eax+1]
*/
static void *addr_patch_outgoing_pregame_message = (void *) 0x00470BD7; // pregame 1.15.2

/* Found the same way as pregame message (this one will be the bigger value) */
static void *addr_patch_outgoing_ingame_message  = (void *) 0x004C2097; // ingame 1.15.2

/* This address was found by searching for the string "/whisper %s %s", and looking for the 
 * function it was sent to:
.text:19025507                 push    offset aWhisperSS ; "/whisper %s %s"
.text:1902550C                 push    edx             ; char *
.text:1902550D                 mov     byte ptr [eax], 0
.text:19025510                 call    _sprintf
.text:19025515                 add     esp, 10h
.text:19025518                 lea     eax, [ebp+var_280]
.text:1902551E                 call    sub_1902AF80                                                 

   Then, within that function, find the line that calls send_tcp(), and patch over that:
.text:1902AFE3                 push    edx             ; buffer
.text:1902AFE4                 mov     byte ptr [ebp+eax+buffer], bl
.text:1902AFEB                 push    0Eh             ; packet_code
.text:1902AFED                 mov     ebx, eax
.text:1902AFEF                 call    send_tcp        ; ebx = the length ********
.text:1902AFF4                 pop     ebx
.text:1902AFF5                 mov     esp, ebp
.text:1902AFF7                 pop     ebp

 */
static void *addr_patch_outgoing_channel_message = (void *) 0x1902AFEF; // 1.15.2

/* This address is the beginning of the function that contains "<From %s> %s":
.text:19028CF0                 push    ebp
.text:19028CF1                 mov     ebp, esp
.text:19028CF3                 sub     esp, 240h
.text:19028CF9                 test    eax, eax
...
.text:19028E7C                 lea     eax, [ebp+var_130]
.text:19028E82                 push    eax
.text:19028E83                 push    esi
.text:19028E84                 push    offset aFromSS  ; "<From: %s> %s"
.text:19028E89                 jmp     short loc_19028E98
 */
static void *addr_patch_incoming_channel_message = (void *) 0x19028CF0; // 1.15.2

/** Found with TSearch and autohack, should be easy to re-find it. */
//static int *addr_var_minerals = (int*)  0x0057E0B8;

/** Same as minerals. */
//static int *addr_var_gas      = (int*)  0x0057E0E8;

/** I found this near the use of "%s: %s". */
//static unsigned char *addr_var_players = (unsigned char *) 0x0057DEC8;

/** I found this by looking for a function that requires a hWnd parameter (IsIconic worked). */
static HWND *addr_var_hwnd = (HWND *) 0x0051BFB0; // 1.15.2

/** I found this near the "%s: %s" deal. */
//static int *MyPlayerNumber = (int *) 0x00511684;

/** .text:0041DA2B 6A 08                                   push    8
 *  .text:0041DA2D 68 E0 01 00 00                          push    1E0h
 *  .text:0041DA32 68 80 02 00 00                          push    280h
 *  .text:0041DA37 50                                      push    eax
 *  .text:0041DA38 FF 51 54                                call    dword ptr [ecx+54h]
 *  .text:0041DA3B 85 C0                                   test    eax, eax
 *  .text:0041DA3D 74 3C                                   jz      short loc_41DA7B
 *  .text:0041DA3F 8B 35 68 E3 4F 00                       mov     esi, ds:GetSystemMetrics
 */
static int *x_resolution = (int*)0x0041DA2E; // 1.15.2
static int *y_resolution = (int*)0x0041DA33; // 1.15.2



/** Sends a TCP packet to the Battle.net server. The packet doesn't include the 4-byte 
 * header, and neither does the length. */
void send_tcp(int packet_code, char *data, int length);

/** Sends a command to the Battle.net server. */
BOOL send_command_from_game(char *command);

/* Display a message to a user during a game. The message is shown for ms_duration in
 * milliseconds, although there's an upper limit that I haven't timed. The message is
 * shown in the specified colour, a good one is SC_PALEBLUE. Finally, the message is
 * displayed. */
void display_game_raw(int ms_duration, int colour, char *message);

/* Display any EID_ packet with all parameters. */
void display_channel_raw(int type, int flags, int ping, char *username, char *message);

/* The conversion from the player number to colour is nasty, and is performed by this function. */
char get_player_colour(int player);

#endif
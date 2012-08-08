#ifndef __BUFFER_H__
#define __BUFFER_H__

#define BUFFER_INITIAL_LENGTH 16 

/* This is a classic buffer implementation. Bytes, words, dwords, and strings can be added,
 * and it grows as necessary. The length doubles each time, because I hear that's what's cool. 
 * The buffer clears old data every time it grows, and erases itself when it's destroyed. There's
 * no guarantee that data from the buffer doesn't linger, though, so the usual precaution of 
 * storing nothing important should be taken. 
 * 
 * I'm not really sure why I re-implemented this, since it's been done 100 times, but it was
 * only ever supposed to be a demonstration. */

typedef struct
{
	char *data;
	int current_length;
	int max_length;
} t_buffer;

t_buffer *buffer_initialize();
void buffer_destroy(t_buffer *buffer);

/* Ensure capacity in the buffer. This can be used to stop the buffer from expanding by 
 * pre-allocating all memory. */
void buffer_allocate(t_buffer *buffer, int amount);

/* Insert a single byte into the buffer. */
t_buffer *buffer_insert_byte(t_buffer *buffer, int byte);

/* Insert 2-bytes, little endian, into the buffer. */
t_buffer *buffer_insert_word(t_buffer *buffer, int word);
/* Insert 4-bytes, little endian, into the buffer. */
t_buffer *buffer_insert_dword(t_buffer *buffer, int dword);
/* Insert a null teriminated string, including the terminator, into the buffer. Note that this can be tricked with an 
 * integer overflow, which results in adding nothing to the string, which is safe. */
t_buffer *buffer_insert_ntstring(t_buffer *buffer, char *string);
/* Insert a specific number of bytes, from an array, into the buffer. */
t_buffer *buffer_insert_bytes(t_buffer *buffer, char *bytes, int count);
/* Insert the contents of one buffer into another. */
t_buffer *buffer_insert_buffer(t_buffer *buffer, t_buffer *src);

/* After calling this, you should stop inserting until you're done with the string. Otherwise, 
 * the string may change. */
char *buffer_get_cstring(t_buffer *buffer);

/* Get the length. */
int buffer_get_length(t_buffer *buffer);

/* Get the buffer as a human-redable string. This string must be free()'d. */
char *buffer_to_string(t_buffer *buffer);

/* Get the address of the tip of the buffer. This is useful if assembly is being built and
 * a relative operation (like a call) is required. */
char *buffer_gettipaddress(t_buffer *buffer);

/* Erase the buffer. The data that was in the buffer is also erased. */
void buffer_clear(t_buffer *buffer);

/* Display the bytes in the buffer. This isn't pretty, but it's just designed for debugging so it isn't 
 * meant to be. */
void buffer_print(t_buffer *buffer);

#endif
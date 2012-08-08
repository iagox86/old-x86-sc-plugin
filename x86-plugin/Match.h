/** Match.h
 * by iago
 * Sept. 10/2003
 *
 * These are basic functions for pattern-matching and replacing. */

#ifndef __MATCH_H__
#define __MATCH_H__

#include <ctype.h>
#include <windows.h>

/* Check if the string name matches the pattern mask. I believe the implementation was given
 * to me by Skywing. */
int match(const char *mask, const char *name);

/* Do a really lousy, ineffecient match. For strings less than a few hundred characters, 
 * this works very fast. */
BOOL match_replace(const char *Find, const char *Replace, char *String, size_t maxlen);

/* Call match_replace() until the string stops changing. Seriously. If it's obvious that an
 * infinite loop is going on, this will end early. */
BOOL match_replaceall(const char *Find, const char *ReplaceChar, char *String, size_t maxlen);


#endif

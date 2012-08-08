#ifndef __LOGGING_H__
#define __LOGGING_H__

/* This is a simple logging module, that writes entries to a logfile. */

#define LOGFILE "log.txt"
#define LOGMAX 1024

/* Add text to the log (printf-style formatting) */
void __cdecl log_add(const char *text, ...);

/* Indent text until log_undent() is called. */
void __cdecl log_indent();

/* Remove indentation from the text. */
void __cdecl log_undent();

/* Clear the logfile. */
void __cdecl log_clear();

#endif
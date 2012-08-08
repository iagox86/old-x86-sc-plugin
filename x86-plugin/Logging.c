#include "Logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

static int indentation = 1;

void __cdecl log_add(const char *text, ...)
{
    char buffer[LOGMAX + 1];
	FILE *file;

    va_list argptr;
    va_start(argptr, text);

	memset(buffer, 0, LOGMAX);
    _vsnprintf_s(buffer, LOGMAX, LOGMAX, text, argptr);

	fopen_s(&file, LOGFILE, "a");
    
    if(file != NULL)
    {
		char *spaces = malloc(indentation * 2 + 1);
		time_t currtime = time(NULL);
		struct tm Time;
		localtime_s(&Time, &currtime);

		memset(spaces, 0, indentation * 2 + 1);
		memset(spaces, ' ', indentation * 2);
		fprintf(file, "[%02d/%02d/%04d %02d:%02d:%02d] %s%s\n", Time.tm_mon + 1, Time.tm_mday, Time.tm_year + 1900, Time.tm_hour, Time.tm_min, Time.tm_sec, spaces, buffer);
		fclose(file);
		free(spaces);
    }
}

void __cdecl log_indent()
{
	indentation++;
}

void __cdecl log_undent()
{
	indentation--;
	if(indentation < 1)
		indentation = 1;
}

void __cdecl log_clear()
{
	FILE *file;
	fopen_s(&file, LOGFILE, "w");
	if(file)
		fclose(file);
}
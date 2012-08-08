#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Injection.h"
#include "getopt.h"

typedef enum
{
	OPTION_INJECT = 1,
	OPTION_EJECT  = 2,
	OPTION_QUIT   = 9,
} t_options;

/** This prints the error message, then ends the program. */
void error(char *message)
{
	fprintf(stderr, "Fatal Error: %s\nTerminated.\n", message);
	fprintf(stderr, "Code: 0x%x\n\n", GetLastError());

	system("pause");
	exit(1);
}

/** This creates the new process, and fills in the two HANDLEs with the handles of the newly 
 * created process. This also suspends the process, ResumeThread()  has to be called on hThread
 * to get it going.
 *  lpAppPath: The full path to the application. 
 *  lpCmdLine: The commandline. First parameter has to be the program name, or this has to be NULL.
 *  hThread:   The created thread is put in here. 
 *  hProcess:  The created process is put in here. 
 */
void CreateProcessEx ( char *lpAppPath, char *lpCmdLine, char *lpWorkingDirectory, HANDLE *hThread, HANDLE *hProcess) 
{
    STARTUPINFO         startupInfo;
    PROCESS_INFORMATION processInformation;    

	printf("Attempting to load the program: %s\n", lpAppPath);
	if(lpCmdLine)
		printf(" -> Command Line: %s\n", lpCmdLine);
	else
		lpCmdLine = "";

	/* Initialize the STARTUPINFO structure. */
    ZeroMemory( &startupInfo, sizeof( STARTUPINFO ));
    startupInfo.cb = sizeof( STARTUPINFO );

	/* Initialize the PROCESS_INFORMATION structure. */
    ZeroMemory( &processInformation, sizeof( PROCESS_INFORMATION ));

	/* Create the actual process with an overly-complicated CreateProcess function. */
	if(!CreateProcess(lpAppPath, lpCmdLine, 0, 0, FALSE, CREATE_NEW_CONSOLE | CREATE_SUSPENDED, 0, lpWorkingDirectory, &startupInfo, &processInformation))
		error("Failed to create the process");

	*hThread = processInformation.hThread;
    *hProcess = processInformation.hProcess;

	printf("Successfully created the process!\n\n");
}

/** This function overwrites the first bytes of a function with the specified bytes, 
 * with the intention of disabling a function. The first few bytes can be replaced with
 * a "ret" or a "mov eax, xxx / ret". This will be used to disable certain functions 
 * like SetSecurityInfo() and IsDebuggerPresent(). This will also load the .dll file 
 * that the function is contained in, if it's not already loaded. 
 * 
 * Note that you can actually do *anything* here, since this is arbitrary machine code,
 * but that the change will not be undone until the program ends. 
 *
 * Note that certain .dll functions may cause unpredictable results. For example, if
 * you try disabling a function in battle.snp, Battle.net won't connect. Most normal 
 * .dll functions should be ok, though. 
 * 
 *  hProcess:  The process to modify the data. 
 *  function:  The name of the function, such as "SetSecurityInfo"
 *  dll:       The name of the .dll file, such as "advapi32.dll". Has to be just the name, not the path. 
 *  code:      The code to replace the opening bytes with, see below for some examples
 *  length:    The number of bytes to replace (should be the same as the length of code)
 * 
 * Here are the most common ways to return early (be sure you match the function's 
 * real return size!):
 *  ret:    c3
 *  ret xx: c2 xx xx
 *
 * And here are some ways to set the value that's returned (be sure to follow with
 *  c3 or c2 xx xx:
 *
 * return 1:  b8 01 00 00 00 (mov eax, 1)
 * return 1:  6a 01 58       (push 1 / pop eax)
 * return 1+: 83 c8 01       (or eax, 1)
 * return 0:  33 c0
 * return -1: b8 ff ff ff ff (mov eax, -1)
 * return -1: 33 c0 83 e8 01 (xor eax, eax / sub eax, 1)
 * return -1: 83 c8 ff       (or eax, -1)
 * 
 * Here are some examples:
 *  return 1: "\x6a\x01\x58\xc3"
 *  return 0: "\x33\xc0\xc3"
 */
void DisableFunction(HANDLE hProcess, char *function, char *dll, char *code, int length)
{
	HMODULE addrModule = NULL;
	FARPROC addrFunction = NULL;

	printf(" -> Disabling %s!%s\n", dll, function);

	/* Try and get the module handle without loading it. */
	addrModule = GetModuleHandle(dll);
	if(!addrModule)
	{
		printf("    -> .dll doesn't seem to be loaded, attempting to load.\n");
		/* The module wasn't found, so we probably have to load it. This line loads it (into the current process). */
		addrModule = LoadLibrary(dll);
		if(!addrModule)
		{
			/* Apparently the library couldn't be loaded. */
			printf("    -> ERROR! Couldn't find library: %s\n", dll);
			return;
		}
	}
	printf("    -> Found %s: %p\n", dll, addrModule);

	/* Get the address of the function within the .dll file. */
	addrFunction = GetProcAddress(addrModule, function);
	if(!addrFunction)
	{
		printf("    -> ERROR! Couldn't find function: %s\n", function);
		return;
	}

	printf("    -> Found %s: %p\n", function, addrFunction);
	printf("    -> Attempting to overwrite with %d bytes of code\n", length);
	
	/* Attempt to write to the foreign process. */
	if(!WriteProcessMemory(hProcess, addrFunction, code, length, NULL))
	{
		printf("    -> Memory write failed, attempting to load the .dll into the process\n");
		/* If the write failed, it likely means that the .dll isn't loaded yet, so load it into the foreign process. */
		if(!InjectLibrary(hProcess, dll))
		{
			printf("    -> ERROR! Couldn't load the dll: %s\n", dll);
			return;
		}

		/* Try writing again. Hopefully it'll work this time. */
		if(!WriteProcessMemory(hProcess, addrFunction, code, length, NULL))
		{
			printf("    -> ERROR! Still couldn't disable the function!\n");
			return;
		}
	}
	printf("    -> Function %s!%s disabled successfully!\n", dll, function);
}

/** This function will probably be extended in the future to include more and more security functions. 
 * This prevents certain functions from running. These functions can prevent and detect things like 
 * debuggers and other tools. */
void DisableSecurity(HANDLE hProcess)
{
	DisableFunction(hProcess, "SetSecurityInfo",   "advapi32.dll", "\xc2\x1c\x00", 3);
	DisableFunction(hProcess, "IsDebuggerPresent", "kernel32.dll", "\x33\xc0\xc3", 3);
	//DisableFunction(hProcess, "GetThreadContext",  "kernel32.dll", "\x33\xc0\xc2\x08\x00", 5);
}

/** This function disables file deletion functions, such as DeleteFileA and DeleteFileW. */
void DisableDelete(HANDLE hProcess)
{
	DisableFunction(hProcess, "DeleteFileA", "kernel32.dll", "\x6a\x01\x58\xc2\x04\x00", 6);
	DisableFunction(hProcess, "DeleteFileW", "kernel32.dll", "\x6a\x01\x58\xc2\x04\x00", 6);
}

/** Print out the command-line usage. */
void usage()
{
	error("Proper usage:                                                 \n\
		                                                                 \n\
 x86-loader.exe [-s <0/1>] [-p <0/1>] [-c <commandline>] [-d <dll>]      \n\
                [-w <directory>]  <program>                              \n\
                                                                         \n\
   -s                                                                    \n\
   When -s is set to its default value of 1, certain security functions  \n\
   are disabled, which prevents the program from protecting itself from  \n\
   other tools, such as debuggers. Currently, the following security     \n\
   functions are disabled:                                               \n\
   - advapi32.dll!SetSecurityInfo()                                      \n\
   - kernel32.dll!IsDebuggerPresent()                                    \n\
                                                                         \n\
   -p																	 \n\
   If -p is set (it is disabled by default), files are 'preserved' (in   \n\
   other words, the DeleteFileA function is disabled.                    \n\
																		 \n\
   -c                                                                    \n\
   The -c parameter provides a commandline to pass to the program. The   \n\
   first parameter must always be the name of the program (or            \n\
   something), the first real parameter passed to the program is the     \n\
   second parameter on -c. Use quotation marks to give more than one     \n\
   parameter.                                                            \n\
                                                                         \n\
   -d                                                                    \n\
   If -d is set, the given dll will be loaded into the program's address \n\
   space immediately after the program is loaded.                        \n\
                                                                         \n\
   -w                                                                    \n\
   Specifies the working directory. If this isn't set, the working       \n\
   directory from the loader will be inherited.                          \n\
                                                                         \n\
   <program>                                                             \n\
   This is the program that will be run by this loader.                  \n\
   ");
}

int main(int argc, char *argv[], char **env)
{
	HANDLE hThread;
	HANDLE hProcess;
 
	char *dll               = NULL;
	BOOL disable_security   = TRUE;
	BOOL disable_delete     = FALSE;
	char *commandline       = NULL;
	char *working_directory = NULL;
	char *program           = NULL;
	int c;
	t_options option;

    opterr = 0;

	/* This big loop and switch() parse the command-line parameters. */
	while ((c = getopt (argc, argv, "s:p:c:d:w:")) != -1)
	{
		switch (c)
        {
        case 's':
             disable_security = atoi(optarg);
             break;

		case 'p':
			disable_delete = atoi(optarg);
			break;

        case 'c':
            commandline = optarg;
            break;

        case 'd':
            dll = optarg;
            break;

		case 'w':
			working_directory = optarg;
			break;

        case '?':
             fprintf (stderr, "Unknown option '-%c'.\n", optopt);
			 usage();
             return 1;

        default:
			fprintf(stderr, "Unrecognized option (programmer's fault!): '%c'.\n", optopt);
            return 1;
        }
	}
	if(optind >= argc)
		usage();
	program = argv[optind];

	/* The process is always created. */
	CreateProcessEx(program, commandline, working_directory, &hThread, &hProcess);

	/* If they opt to disable the security functions, do it. */
	if(disable_security)
		DisableSecurity(hProcess);

	/* If they opt to disable the delete functions, do it. */
	if(disable_delete)
		DisableDelete(hProcess);

	/* If they provided a .dll file, inject it. */
	if(dll)
	{
		printf("\nInjecting .dll file: %s\n", dll);
		if(InjectLibrary(hProcess, dll))
			printf("Dll injected successfully!\n\n\n");
		else
			printf("Dll injection failed!\n\n\n");
	}

	/* Resume the thread. */
	ResumeThread(hThread);

	if(dll)
	{
		do
		{
			printf("%d) Inject the .dll\n", OPTION_INJECT);
			printf("%d) Eject the .dll\n", OPTION_EJECT);
			printf("%d) Quit\n", OPTION_QUIT);
			printf("\n");
			printf("Please make a selection --> ");
			scanf_s("%d", &option);

			switch(option)
			{
			case OPTION_INJECT:
				if(InjectLibrary(hProcess, dll))
					printf("Library successfully injected!\n\n");
				else
					printf("Library injection failed!\n\n");

				break;

			case OPTION_EJECT:
				if(EjectLibrary(hProcess, dll))
					printf("Library successfully ejected!\n\n");
				else
					printf("Library ejection failed!\n\n");
				
				break;

			case OPTION_QUIT:
				printf("Bye!\n\n");
				break;

			default:
				printf("Unknown option!\n\n");
			}
		} while(option != OPTION_QUIT);
	}

	return 0;
}
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "par_run.h"
#include "commandlinereader.h"

#define MAX_INPUT_SIZE 2048
#define MAX_ARGC 7

int main(int argc, char* argv[])
{
	
        char input[MAX_INPUT_SIZE];
		char* command_plus_argv[MAX_ARGC+1];
		char* command;
		char** argv_child;
		int numchildren;

    for (;;)
	{	
        memset(command_plus_argv, 0,MAX_ARGC+1);
   
		putchar('$'); putchar(' ');
	
		fgets(input, MAX_INPUT_SIZE, stdin);
	
		commandlinereader(input, command_plus_argv, MAX_ARGC);
		
		command = command_plus_argv[0];
		argv_child = &command_plus_argv[1];

        if (command == NULL)
            continue;

		if (!strcmp(command, "exit"))
		{
			while (numchildren < 0)
				wait(NULL), numchildren--; 

			break;
		}
	
		else
			par_run(command, argv_child);
			putchar('\n');
	}

	return EXIT_SUCCESS;
}
	

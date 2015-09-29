#include <stdio.h>
#include <string.h>

#include "par-run.h"
#include "divide_input.h"

#define MAX_INPUT_SIZE 2048

int main(int argc, char* argc[])
{
	for (;;)
	{	char input[MAX_INPUT_SIZE];
		char** command_plus_argv;
		char* command;
		char** argv_child;
		int numchildren;

		putchar('$'); putchar(' ');
	
		fgets(input, MAX_INPUT_SIZE, stdin);
	
		command_plus_argv = divide_input(input, MAX_ARGC);
		
		command = command_plus_argv[0];
		argv_child = &command_plus_argv[1];

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
	

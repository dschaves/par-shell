
#include <stdio.h>
#include <string.h>

#include "par-run.h"

#define MAX_INPUT_SIZE 2048
#define MAX_ARGC 5

int main(int argc, char* argc[])
{
	for (;;)
	{	char input[MAX_INPUT_SIZE];
		char** command_plus_argv;
		char* command;
		char** argv_child;

		putchar('$'); putchar(' ');
	
		fgets(input, MAX_INPUT_SIZE, stdin);
	
		command_plus_argv = divide_input(input, MAX_ARGC);
		
		command = command_plus_argv[0];
		argv_child = &command_plus_argv[1];

		if (!strcmp(command, "exit"))
			break;
	
		else
			par_run(command, argv_child);
			putchar('\n');
	}

	return EXIT_SUCCESS;
}

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#include "par_run.h"
#include "commandlinereader.h"

#define MAX_ARGC 7

int main(int argc, char* argv[])
{
	
    char* input = NULL; // for use in getline

    size_t input_size = 0;

    char* argv_child[MAX_ARGC+2];

    int commands_given = 0;

    for (;;)
	{
        
        printf("        <<[%d] PAR-SHELL NOW ACCEPTING INPUT>>\n", commands_given);	

        getline(&input, &input_size, stdin);   

        int argc_child = commandlinereader(input, argv_child, MAX_ARGC);

        if (argv_child[0] == NULL)
        {
               puts("(got nothing)");         
               continue;
        }

        else if (argc_child == -1)
        {
            printf("Too many arguments. Maximum is %d.\n", MAX_ARGC);            
            continue;
        }

	    else if (!strcmp(argv_child[0], "exit"))
	    {
            while (errno != ECHILD)
                wait(NULL);
            break;
	    }   

        else
		    par_run(argv_child), ++commands_given;
    }

    return 0;
}


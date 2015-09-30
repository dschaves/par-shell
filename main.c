#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "par_run.h"
#include "commandlinereader.h"

#define MAX_ARGC 7

int main(int argc, char* argv[])
{
	
    char* input = NULL; // for use in getline
    size_t input_size = 0;

    char* argv_child[MAX_ARGC+2];

    int commands_given = 0;

    int numchildren = 0;

    for (;;)
	{	
        memset(input, 0, input_size);
        memset(argv_child, 0, sizeof(argv_child));
        
        printf("        <<[%d] PAR-SHELL NOW ACCEPTING INPUT>>\n", commands_given);	   
        
        getline(&input, &input_size, stdin); 	

        int argc_child = commandlinereader(input, argv_child, MAX_ARGC);

        if (argv_child[0] == NULL || argc_child == -1)
            continue;

	    if (!strcmp(argv_child[0], "exit"))
	    {
		    while (numchildren > 0)
			    wait(NULL), numchildren--;

		    break;
	    }   

        else
		    par_run(argv_child), numchildren++;

        ++commands_given;

    }

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "par_run.h"
#include "commandlinereader.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGC 7

int main(int argc, char* argv[])
{
	
    char input[MAX_INPUT_SIZE];
	char* argv_child[MAX_ARGC+2];
    int numchildren = 0;

    for (;;)
	{	
        
        memset(input, '\0', sizeof(input));
        memset(argv_child, '\0', sizeof(argv_child));
   
		putchar('$'); putchar(' ');
	
		fgets(input, (int) sizeof(input), stdin);
	
		commandlinereader(input, argv_child, MAX_ARGC);

        if (argv_child[0] == NULL)
            continue;

	    if (!strcmp(argv_child[0], "exit"))
	    {
		    while (numchildren > 0)
			    wait(NULL), numchildren--;

		    break;
	    }   

	    else if (numchildren < 10) //prevent forkbombing
		    par_run(argv_child), numchildren++;

    }

    return EXIT_SUCCESS;
}


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "par_run.h"

void par_run(char* argv_child[])
{

    pid_t pid = fork();
	
	if (pid == 0) // is child
	{	
        execv(argv_child[0], argv_child);

        // next instruction will not be reached if exec is successful
        perror("par-shell: exec failed");
        exit(1);
    }
    
    else return;
}


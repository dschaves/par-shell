#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "par_run.h"

void par_run(char* argv_child[])
{

    pid_t pid = fork();
	
	if (pid == 0) /* is child */
	{	
        execv(argv_child[0], argv_child);
        // next instruction should never be reached
        perror("EXEC FAILED");
        exit(1);
    }
/*
    int i;
    char* string;
    for (i = 0; (string = argv[i]); i++) puts(string); */

}

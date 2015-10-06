#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "par_run.h"

pid_t par_run(char* argv_child[])
{

    pid_t pid = fork(); //parent returns, child becomes desired process
	
	if (pid == 0) // is child
	{	
        /* we first try executing the command as a directory. If that does not work,
		 * execute as a command in PATH.
		 */
		printf("%s",argv_child[0]);
		execv(argv_child[0], argv_child); //arg 1: command; arg2: name and arguments
		execvp(argv_child[0], argv_child);

        // next instruction will not be reached if exec is successful
        perror("par-shell: exec failed");
        exit(1);
    }
    
    else return pid; //is parent
}

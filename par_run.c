#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "par_run.h"


int par_run(char **argVector){
	int pid = fork();
	if (pid == -1){
		perror("par-shell: unable to fork");
		exit(EXIT_FAILURE);			
	}
	if (pid == 0){
		execv(argVector[0], argVector);
		perror("par-shell: exec failed");
    	exit(-1);
	}
	return pid;
}

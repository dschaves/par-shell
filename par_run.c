#include <unistd.h>
#include "par_run.h"

void par_run(char command[], char* argv[])
{
	pid_t pid = fork();
	
	if (pid == 0) /* is child */
		execv(command, argv);

	else return;

}

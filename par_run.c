#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "par_run.h"
#include "main.h" // children_list, children_count
#include "list.h"


void par_run(char* argVector[]) 
{

	pid_t pid = fork();

	if (pid == -1)  //parent. if child isn't created
		perror("par-shell: unable to fork");		

	else if (pid == 0) // is child
	{
		execv(argVector[0], argVector);
		// next line shouldn't be reached
		perror("par-shell: exec failed");
		exit(1);
	}

	else
	{
		pthread_mutex_lock(&main_mutex);
		insert_new_process(children_list, pid, time(NULL));
		children_count +=1;
		pthread_mutex_unlock(&main_mutex);
	}

}

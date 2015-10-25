#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "par_run.h" // self
#include "main.h" // children_list, children_count
#include "list.h" // insert_new_process


void par_run(char* argVector[]) 
{
	
	sem_wait(can_exec);
	pid_t pid = fork();

	if (pid == -1)  //parent. if child isn't created
		perror("par-shell: unable to fork");		

	else if (pid == 0) // is child
	{
		execv(argVector[0], argVector);
		// next line shouldn't be reached
		perror("par-shell: exec failed");
		exit(EXIT_FAILURE);
	}

	else // is parent
	{ 
		IN_MAIN_MUTEX 
		(
			insert_new_process(children_list, pid, time(NULL));
			children_count +=1;
		)

		sem_post(can_wait);
	}

}


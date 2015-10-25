#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <unistd.h> // sleep()
#include <pthread.h> // mutex()

#include "list.h" // 
#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self


void* monitor(void* _)
{

	time_t endtime;

	int pid, children_waiting, exit_called_l;

	for(;;)
	{	
		if (atomic_get_exit_called()) pthread_exit(NULL);

		sem_wait(&can_wait); // XXX

		/* In this next part:
		*
		*		1 - We wait for the waited_children-th child to finish
		*		2 - From function wait we get pid
		*		3 - From function time we get the time child took to finish
		*		4 - Update children_list with pid and endtime for this child. 
		*		Check all necessary errors.
		*/

		pid = wait(NULL);		// XXX

		sem_post(can_exec); 	// XXX

		endtime = time(NULL); 
	
		if (endtime == -1) 
			perror("par-shell: [ERROR] couldn't get finish time for child");

		IN_MAIN_MUTEX
		(

			if (pid == -1)
				perror("par-shell: [ERROR] couldn't wait on child");  

			else 
				++waited_children;

			update_terminated_process(children_list, pid, endtime); //XXX: updates here
		)

	}

	return NULL;
}

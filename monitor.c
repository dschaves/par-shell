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
		pthread_mutex_lock(&main_mutex);
		children_waiting = (waited_children < children_count);
		exit_called_l = exit_called;
		pthread_mutex_unlock(&main_mutex);

		if (children_waiting) 
		{
	   
			/* In this next part:
			*
			*		1 - We wait for the waited_children-th child to finish
			*		2 - From function wait we get pid
			*		3 - From function time we get the time child took to finish
			*		4 - Update children_list with pid and endtime for this child. 
			*		X - Check all necessary errors
			*/


			pid = wait(NULL);		// XXX: waits here
			endtime = time(NULL); 
		
			if (endtime == -1) 
				perror("par-shell: [ERROR] couldn't get finish time for child");

			pthread_mutex_lock(&main_mutex);

			if (pid == -1)
				perror("par-shell: [ERROR] couldn't wait on child");  

			else 
				waited_children += 1;

			update_terminated_process(children_list, pid, endtime); //XXX: updates here

			pthread_mutex_unlock(&main_mutex);
		}

		else if (exit_called_l == 1)
				break;

		else sleep(1); //sleep if no processes are waiting to be waited.

	}

	return NULL;
}

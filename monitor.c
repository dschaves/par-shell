#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <pthread.h> 

#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self

void* monitor(void* _)
{

	time_t endtime;

	pid_t pid;

	for(;;)
	{	
		pthread_mutex_lock(&children_mutex);

		if (atomic_get_exit_called() && !wait_slot_avaliable()) 
		/* Only breaks if exit has been input AND there are no more children to be waited on. */
		{
			pthread_mutex_unlock(&children_mutex);
			break;
		}

		while (!wait_slot_avaliable()) 
			pthread_cond_wait(&can_wait, &children_mutex);

		pid = wait(NULL); // XXX: waits for next child to terminate

		time(&endtime);

		if (endtime == -1) 
			perror("par-shell: [ERROR] couldn't get finish time for child");

		if (pid == -1)
			perror("par-shell: [ERROR] couldn't wait on child");  

		else
		{
			atomic_update_terminated_process(pid, endtime);
			inc_waited_children();
			pthread_cond_signal(&can_fork);	
		}

		pthread_mutex_unlock(&children_mutex);
	}
	return NULL;
}

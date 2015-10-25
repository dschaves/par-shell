#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <semaphore.h> 

#include "list.h" // 
#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self


void* monitor(void* _)
{

	time_t endtime;

	int pid;

	for(;;)
	{	
		if (atomic_get_exit_called()) break;

		sem_wait(&can_wait); // XXX

		pid = wait(NULL);		// XXX

		sem_post(&can_fork); 	// XXX

		endtime = time(NULL); 
	
		if (endtime == -1) 
			perror("par-shell: [ERROR] couldn't get finish time for child");

		if (pid == -1)
			perror("par-shell: [ERROR] couldn't wait on child");  

		else
		{
			atomic_inc_waited_children();
			atomic_update_terminated_process(pid, endtime);
		}

	}

	return NULL;
}

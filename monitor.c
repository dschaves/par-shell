#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <pthread.h> 
#include <signal.h>

#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self

static int exit_called = 0;

static void handle_SIGTERM(int signo) 
{ 
	exit_called = 1; 
	pthread_cond_signal(&can_wait);
}

void* monitor(void* _)
{
	signal(SIGTERM, handle_SIGTERM);

	time_t endtime;

	pid_t pid;

	for(;;)
	{	
		pthread_mutex_lock(&children_mutex);

		while (!wait_slot_avaliable())
		{ 
			if (exit_called) goto exit;
				 
			else pthread_cond_wait(&can_wait, &children_mutex);
		}

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
	}
		exit: return NULL;
}





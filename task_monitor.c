#include <time.h> // time
#include <stdio.h> // perror
#include <sys/wait.h> // wait
#include <unistd.h> // sleep 
#include <pthread.h> // mutex

#include "task_monitor.h"
#include "main.h"

void* task_monitor(void* _main_status)
{

	time_t start_time, finish_time;

	int pid, status, children_count, exit_called, waited_children = 0;

	int* children_pid_arr, *children_status_arr;

	time_t *children_time_arr;

	struct main_status* main_status =  _main_status;

	pthread_mutex_t* main_mutex = main_status->mutex;

	for(;;)
	{
		pthread_mutex_lock(main_mutex);

		exit_called = main_status->exit_called;	
		children_count = main_status->children_count;
		children_pid_arr = main_status->children_pid_arr;
		children_status_arr = main_status->children_status_arr;
		children_time_arr = main_status->children_time_arr;

		pthread_mutex_unlock(main_mutex);

		if (children_count > waited_children) 
		{

		   
			/* In this next part:
			*
			*		1 - We wait for the waited_children-th child to finish
			*		2 - From function wait we get pid and status
			*		3 - From function time we get time child took to finish
			*		4 - Save pid, status and time into array for later printing
			*		5 - Check all necessary errors
			*/

			start_time = time(NULL);

			pid = wait(&status);	// waits here

			finish_time = time(NULL) - start_time;

			if (pid == -1)
			{
				perror("par-shell: [ERROR] couldn't wait on child"); 
				status = -1;
				goto failure;
			}
		
			if (finish_time < 0)
			{	
				perror("par-shell: [ERROR] couldn't get finish time for child");
				finish_time = -1;		
			}

			pthread_mutex_lock(main_mutex);
			 
			children_pid_arr[waited_children] = pid;
			children_status_arr[waited_children] = WEXITSTATUS(status); 
			children_time_arr[waited_children] = finish_time;

			pthread_mutex_unlock(main_mutex);

			waited_children += 1;


		}

		else if (exit_called == 1)
				goto success;

		else sleep(1); //sleep if no processes are waiting to be waited.

	}

	success: return NULL; //FIXME: este return nao faz sentido
	failure: return NULL; //FIXME: ditto
}

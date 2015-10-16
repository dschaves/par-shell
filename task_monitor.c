#include <time.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "task_monitor.h"
#include "main.h"

void* task_monitor(void* _datav_children)
{

	int pid;

	int start_time;

	int finish_time;

	int status;

	int waited_children = 0;

	int** datav_children = (int**) _datav_children;

	for(;;)
	{
		
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
			 
			datav_children[waited_children][0] = pid;
			datav_children[waited_children][1] = status; 
			datav_children[waited_children][2] = finish_time;

			waited_children += 1;

			if (exit_called == 1 && children_count == waited_children)
			// then we're done here
				goto success;

		}

		else sleep(1); //sleep if no processes are waiting to be waited.

	}

	success: return NULL; //FIXME: este return nao faz sentido
	failure: return NULL; //FIXME: ditto
}


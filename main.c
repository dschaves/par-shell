#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "commandlinereader.h"
#include "par_run.h"
#include "task_monitor.h"
#include "main.h"

//int a;

#define INPUT_SIZE 7

int main(int argc, char* argv[]) 
{

	char* argv_child[INPUT_SIZE];

	int numtokens;

	pthread_t monitor;

	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);

	struct main_status main_status = {0, 0, &mutex};

	pthread_create(&monitor, NULL, task_monitor, &main_status);
	puts("<< PAR-SHELL READY >>"); 

	for(;;) // breaks upon "exit" input
	{

		numtokens = readLineArguments(argv_child, INPUT_SIZE);

		if (numtokens == -1) // attempts to read again if there's an error
		{ 
			perror("par-shell: readLineArguments failed");
			continue; 
		} 

		else if (numtokens == 0) //nothing input
		{	
			continue;
 		}

		else if (!strcmp(argv_child[0], "exit")) 
		{
			pthread_mutex_lock(&mutex);

			main_status.exit_called += 1;

			pthread_mutex_unlock(&mutex);

			pthread_join(monitor, NULL);

			int i;
			for (i = 0; i < main_status.children_count; i++) // prints all pids, return values, and execution times of all children
				printf("Process %d finished with status %d. Execution time: %d\n", 
				main_status.children_pid_arr[i], main_status.children_status_arr[i], (int) main_status.children_time_arr[i]);

			break;
		}

		else if (par_run(argv_child) != -1) 
		{
			pthread_mutex_lock(&mutex);
			main_status.children_count += 1; // new child created here
			pthread_mutex_unlock(&mutex);
		}
	}

	return 0;	
}


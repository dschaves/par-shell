#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "commandlinereader.h"
#include "par_run.h"
#include "task_monitor.h"
#include "list.h"
#include "main.h"



#define INPUT_SIZE 7

int children_count = 0;
int exit_called = 0;
list_t* children_list;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) 
{
	children_list = lst_new(); 

	char* argv_child[INPUT_SIZE];
	int numtokens;

	pthread_t monitor;
	pthread_create(&monitor, NULL, task_monitor, NULL);
	
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
			exit_called += 1;
			pthread_mutex_unlock(&mutex);

			pthread_join(monitor, NULL);

			lst_print(children_list); 
			lst_destroy(children_list);
			break;
		}


		else
		{
			int pid = par_run(argv_child);
			if (pid != -1)
			{
			pthread_mutex_lock(&mutex);
				children_count++;
				insert_new_process(children_list, pid, time(NULL));
			pthread_mutex_unlock(&mutex);
			}
		} 
	}

	return 0;	
}


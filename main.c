#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "commandlinereader.h"
#include "par_run.h"
#include "task_monitor.h"

#define INPUT_SIZE 7
#define MAX_CHILDREN 8192

int exit_called;
int children_count;

int main(int argc, char* argv[]) 
{

	char* argv_child[INPUT_SIZE];

	int datav_children[MAX_CHILDREN][3];	

	pthread_t monitor;

	pthread_create(&monitor, NULL, task_monitor, datav_children);

	puts("<< PAR-SHELL READY >>"); 

	for(;;) // breaks upon "exit" input
	{
	

		int numtokens = readLineArguments(argv_child, INPUT_SIZE);

		if (numtokens == -1) { // attempts again if there's an error
			perror("par-shell: readLineArguments failed");
			continue; 
		}

		else if (numtokens == 0) //nothing input
			continue;

		else if (!strcmp(argv_child[0], "exit")) 
		{

			exit_called = 1;
			pthread_join(monitor, NULL);
			int i;
			for (i = 0; i < children_count; i++) // prints all pids and return values of children
				printf("Process %d finished with status %d.\n", datav_children[i][0], datav_children[i][1]);
			break;
		}

		else if (par_run(argv_child) != -1) children_count += 1;
	}

	return 0;	
}


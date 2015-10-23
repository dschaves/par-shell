#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "commandlinereader.h"
#include "par_run.h"
#include "monitor.h"
#include "list.h"
#include "main.h"

#define INPUT_SIZE 7

/* HERE: Define and initialize all global variables declared in main.h
	children_list is initialized inside main since involves funciton call. 
*/
unsigned int children_count = 0;
unsigned int waited_children = 0;
int exit_called = 0;
pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER;
list_t* children_list;


int main(int argc, char* argv[]) 
{
	children_list = lst_new(); // see above

	char* argv_child[INPUT_SIZE]; // argv passed to forked child.

	pthread_t thread_monitor;	

	if(pthread_create(&thread_monitor, NULL, monitor, NULL)) // multi-threading starts here
		perror("par-shell: Couldn't create monitoring thread. Will not be able to wait for children or monitor.");


	puts("<< PAR-SHELL READY >>"); 

	for(;;) // breaks upon "exit" input
	{

		switch (readLineArguments(argv_child, INPUT_SIZE)) // Consegui!
		{
			case -1: perror("par-shell: couldn't read input");
			case 0: continue;
		}

		if (!strcmp(argv_child[0], "exit")) // user asks to exit
		{
			pthread_mutex_lock(&main_mutex);
			exit_called = 1;
			pthread_mutex_unlock(&main_mutex);

			if(pthread_join(thread_monitor, NULL))
				perror("par-shell: couldn't join with monitor thread");

			lst_print(children_list); 
			lst_destroy(children_list);

			break;
		}


		else par_run(argv_child);

	}

	return 0;	
}


#include <stdio.h> // getline()
#include <string.h> // strcmp()
#include <pthread.h> // all
#include <semaphore.h> // sem_init()

#include "commandlinereader.h"
#include "par_run.h"
#include "monitor.h"
#include "list.h"
#include "main.h"

#define CHILD_ARGV_SIZE 20
#define MAXPAR 7

//*********** BEGIN GLOBAL VARIABLES ***********/

sem_t can_wait; // semaphore for being able to wait on children; initialized in main
sem_t can_fork; // semaphore for being able to fork children; initialized in main

static unsigned int children_count = 0; // counts all children successfully forked
static unsigned int waited_children = 0; // counts all children succesffully waited on

static bool exit_called = false; // true after user inputs enter

static pthread_mutex_t main_mutex; // initialized in main

static list_t* children_list; // initialized in main

//*********** END GLOBAL VARIABLES ***********/


#define IN_MAIN_MUTEX(INSTRUCTIONS) \
    { \
		if (pthread_mutex_lock(&main_mutex)) perror("Couldn't lock mutex. Can't continue. Aborting."), exit(EXIT_FAILURE); \
    	INSTRUCTIONS \
		if (pthread_mutex_unlock(&main_mutex)) perror("Couldn't unlock mutex. Can't continue. Aborting."), exit(EXIT_FAILURE); \
	}

void atomic_insert_new_process(int pid, time_t starttime)
{ 
	IN_MAIN_MUTEX(insert_new_process(children_list, pid, starttime);) 
}

void atomic_update_terminated_process(int pid, time_t endtime)
{ 
	IN_MAIN_MUTEX(update_terminated_process(children_list, pid, endtime);)
}

void atomic_inc_children_count(void)
{
	IN_MAIN_MUTEX(++children_count;)
}

void atomic_inc_waited_children(void)
{
	IN_MAIN_MUTEX(++waited_children;)
}

bool atomic_get_exit_called(void)
{
	bool exit_called_l;	
	IN_MAIN_MUTEX(exit_called_l = exit_called;)	
	return exit_called_l;
}

void atomic_set_exit_called(bool b)
{
	IN_MAIN_MUTEX(exit_called = b;)
}





int main(int argc, char* argv[]) 
{	
	children_list = lst_new(); // see above

	pthread_mutex_init(&main_mutex, NULL);

	char* argv_child[CHILD_ARGV_SIZE]; // argv passed to forked child. 

	sem_init(&can_fork, 0, MAXPAR);	// semaphore, mode, count 

	sem_init(&can_wait, 0, 0); //ditto

	pthread_t thread_monitor;

	if(pthread_create(&thread_monitor, NULL, monitor, NULL)) // multi-threading starts here
		perror("par-shell: Couldn't create monitoring thread. Will not be able to monitor and wait for children.");


	puts("<< PAR-SHELL READY; accepting input at any time! >>"); 

	for(;;) // breaks upon "exit" input
	{

		switch (readLineArguments(argv_child, CHILD_ARGV_SIZE)) 
		// switch mais lindo do mundo. Nao me interessa que diz o Rafael!
		{
			case -1: perror("par-shell: couldn't read input");
			case 0: continue;
		}

		if (!strcmp(argv_child[0], "exit")) // user asks to exit
		{
			atomic_set_exit_called(true);

			if (pthread_join(thread_monitor, NULL))
				perror("par-shell: couldn't join with monitor thread");

			break;
		}

		else par_run(argv_child);
	}

	lst_print(children_list); 
	lst_destroy(children_list);		
	pthread_mutex_destroy(&main_mutex);
	sem_destroy(&can_fork);
	sem_destroy(&can_wait);

	return EXIT_SUCCESS;
	
}

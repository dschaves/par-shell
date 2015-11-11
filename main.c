#include <stdio.h> // getline()
#include <string.h> // strcmp()
#include <pthread.h> // all

#include "commandlinereader.h"
#include "par_run.h"
#include "monitor.h"
#include "list.h"
#include "main.h"

#define CHILD_ARGV_SIZE 7
#define BUFFER_SIZE 512

//*********** BEGIN GLOBAL VARIABLES ***********/

static list_t* children_list; // initialized in main

static pthread_t thread_monitor;

static bool exit_called = false; // true after user inputs enter

static unsigned int forked_children = 0; // counts all children successfully forked
static unsigned int waited_children = 0; // counts all children succesffully waited on

const unsigned int MAXPAR = 8; 

static pthread_mutex_t exit_called_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t children_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t can_fork = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_wait = PTHREAD_COND_INITIALIZER;


//*********** END GLOBAL VARIABLES ***********/

/* The following is an API with setters and getters
 * for the static global variables defined above.
 * Mutual exclusion is guaranteed.
 */

void atomic_insert_new_process(int pid, time_t starttime)
{ 
	pthread_mutex_lock(&list_mutex);	
	insert_new_process(children_list, pid, starttime); 
	pthread_mutex_unlock(&list_mutex);	
}

void atomic_update_terminated_process(int pid, time_t endtime)
{ 
	
	pthread_mutex_lock(&list_mutex);
	update_terminated_process(children_list, pid, endtime);
	pthread_mutex_unlock(&list_mutex);
}

bool atomic_get_exit_called(void)
{
	pthread_mutex_lock(&exit_called_mutex);
	bool exit_called_l = exit_called;
	pthread_mutex_unlock(&exit_called_mutex);
	return exit_called_l;
}

static void atomic_set_exit_called(bool b)
{
	pthread_mutex_lock(&exit_called_mutex);	
	exit_called = b;
	pthread_mutex_unlock(&exit_called_mutex);	
}

/** WARNING:
  * ALL FOLLOWING INLINE FUNCTIONS MAY ONLY BE CALLED 
  *	IF CALLING FUNCTION HAS LOCKED MUTEX "CHILDREN_MUTEX" BEFORE CALLING.*/ 

inline bool fork_slot_avaliable(void) 
{ return forked_children - waited_children <= MAXPAR; }

inline bool wait_slot_avaliable(void)
{ return forked_children != waited_children; }

inline void inc_waited_children(void) { ++waited_children; }

inline void inc_forked_children(void) { ++forked_children; }






int main(int argc, char* argv[]) 
{	
	children_list = lst_new(); // initialized here due to function call

	char input[BUFFER_SIZE];

	char* argv_child[CHILD_ARGV_SIZE]; // argv passed to forked child. 

	if (pthread_create(&thread_monitor, NULL, monitor, NULL)) // multi-threading starts here
		perror("par-shell: Couldn't create monitoring thread. Will not be able to monitor and wait for children.");

	printf("Par-shell now ready. Does not wait for jobs to exit!\n>>> "); 

	for(;;) // breaks upon "exit" input
	{

		switch (readLineArguments(argv_child, CHILD_ARGV_SIZE, input, BUFFER_SIZE)) 
		{
			case -1: puts("par-shell: couldn't read input. Retrying.");
			case 0: continue;
		}

		if (!strcmp(argv_child[0], "exit")) { //user asks to exit	

			atomic_set_exit_called(true); 

			if (pthread_join(thread_monitor, NULL))
				perror("par-shell: couldn't join with monitor thread");

			break;
		
		} else par_run(argv_child);

	}

	//cleanup
	lst_print(children_list); 

	lst_destroy(children_list);
		
	pthread_mutex_destroy(&exit_called_mutex);
	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&children_mutex);

	pthread_cond_destroy(&can_fork);
	pthread_cond_destroy(&can_wait);

	return EXIT_SUCCESS;	
}

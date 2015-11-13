#include <stdio.h> // getline()
#include <string.h> // strcmp()
#include <pthread.h> // all
#include <signal.h>

#include "commandlinereader.h"
#include "par_run.h"
#include "monitor.h"
#include "list.h"
#include "main.h"

#define CHILD_ARGV_SIZE 7
#define BUFFER_SIZE 128

//*********** BEGIN GLOBAL VARIABLES ***********/
static list_t* children_list; // initialized in main
static unsigned int forked_children = 0; // counts all children successfully forked
static unsigned int waited_children = 0; // counts all children succesffully waited on
static const unsigned int MAXPAR = 8; 
static bool exit_called = false;
static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t exit_called_mutex = PTHREAD_MUTEX_INITIALIZER;
// End file-global variables

// Begin global variables
unsigned int iteration_count = 0;
time_t total_time = 0;
pthread_mutex_t children_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_fork = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_wait = PTHREAD_COND_INITIALIZER;
//*********** END GLOBAL VARIABLES ***********/

/* Atomic getters and setters. 
 * Mutual exclusion is guaranteed.
 */

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

time_t atomic_get_process_time(int pid)
{ 
	pthread_mutex_lock(&list_mutex);
	time_t process_time = get_process_time(children_list, pid);
	pthread_mutex_unlock(&list_mutex);
	return process_time;
}

/** WARNING:
  * ALL FOLLOWING INLINE FUNCTIONS MAY ONLY BE CALLED 
  *	IF CALLING FUNCTION -HAS NOT- LOCKED MUTEX "CHILDREN_MUTEX" BEFORE CALLING.*/ 

inline bool fork_slot_avaliable(void) 
{ return forked_children - waited_children < MAXPAR; }

inline bool wait_slot_avaliable(void)
{ return forked_children > waited_children; }

/** WARNING:
  * ALL FOLLOWING INLINE FUNCTIONS MAY ONLY BE CALLED 
  *	IF CALLING FUNCTION -HAS- LOCKED MUTEX "CHILDREN_MUTEX" BEFORE CALLING.*/ 

inline void atomic_inc_waited_children(void) 
{ 
	pthread_mutex_lock(&children_mutex);		
	++waited_children;
	pthread_mutex_unlock(&children_mutex);	
}

inline void atomic_inc_forked_children(void)
{ 
	pthread_mutex_lock(&children_mutex);
	++forked_children; 
	pthread_mutex_unlock(&children_mutex);	
}

int main(int argc, char* argv[]) 
{	
	children_list = lst_new(); // initialized here due to function call

	FILE* log_file = fopen("log.txt", "a+");

	char first_line[BUFFER_SIZE];
	char third_line[BUFFER_SIZE];
	unsigned int prev_iter = 0;
	unsigned int prev_time = 0;

	while (fgets(first_line, BUFFER_SIZE, log_file))
	{
		while (fgetc(log_file) != '\n');
		fgets(third_line, BUFFER_SIZE, log_file);
	}

	sscanf(first_line, "iteracao %u ", &prev_iter); 
	sscanf(third_line, "total execution time: %u", &prev_time); 
	
	total_time += prev_time;
	iteration_count += prev_iter;

	pthread_t thread_monitor;
	if (pthread_create(&thread_monitor, NULL, monitor, log_file)) // multi-threading starts here
		perror("par-shell: Couldn't create monitoring thread. Will not be able to monitor and wait for children.");

	char* argv_child[CHILD_ARGV_SIZE]; // argv passed to forked child. 
	char input[BUFFER_SIZE];

	printf("Par-shell now ready. Does not wait for jobs to exit!\n>>> "); 
	for(;;) // breaks upon "exit" input
	{

		switch (readLineArguments(argv_child, CHILD_ARGV_SIZE, input, BUFFER_SIZE)) 
		{
			case -1: fputs("\npar-shell: couldn't read input. Retrying.\n", stderr);
			case 0: continue;
		}

		if (!strcmp(argv_child[0], "exit")) break;	

		else par_run(argv_child);
	}

	atomic_set_exit_called(true);
	pthread_cond_signal(&can_wait);

	if (pthread_join(thread_monitor, NULL))
		perror("par-shell: couldn't join with monitor thread");

	lst_print(children_list); 
	fclose(log_file); 
	lst_destroy(children_list);
	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&children_mutex);
	pthread_cond_destroy(&can_fork);
	pthread_cond_destroy(&can_wait);

	return EXIT_SUCCESS;	
}

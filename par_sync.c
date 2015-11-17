#include <pthread.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "list.h"

#define EMSG_THREADCREATE "par-shell: Couldn't create monitoring thread. Will not be able to monitor and wait for children. Aborting."

static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t exit_called_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t children_counters_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t can_fork = PTHREAD_COND_INITIALIZER;
static pthread_cond_t can_wait = PTHREAD_COND_INITIALIZER;

static unsigned int forked_children = 0; // counts all children successfully forked
static unsigned int waited_children = 0; // counts all children succesffully waited on
static const unsigned int MAXPAR = 8; 

static bool exit_called = false;

static list_t* children_list;

bool exit_called(void)
{
	pthread_mutex_lock(&exit_called_mutex);
	bool exit_called_l = exit_called;
	pthread_mutex_unlock(&exit_called_mutex);
	return exit_called_l;
}

void set_exit_called(void)
{
	pthread_mutex_lock(&exit_called_mutex);	
	exit_called = b;
	pthread_mutex_unlock(&exit_called_mutex);	
	pthread_cond_signal(&can_wait) // XXX So the monitor thread can exit.
}

void signal_fork(int pid, time_t starttime)
{ 
	pthread_mutex_lock(&list_mutex);
	insert_new_process(children_list, pid, endtime);
	pthread_mutex_unlock(&list_mutex);

	pthread_mutex_lock(&list_mutex);	
	insert_new_process(children_list, pid, starttime); 
	pthread_mutex_unlock(&list_mutex);	
}

void signal_wait(int pid, time_t endtime)
{ 
	pthread_mutex_lock(&list_mutex);
	update_terminated_process(children_list, pid, endtime);
	pthread_mutex_unlock(&list_mutex);

	pthread_mutex_lock(&children_counters_mutex);		
	++waited_children;
	pthread_mutex_unlock(&children_counters_mutex);
}

time_t get_finish_time(int pid)
{ 
	pthread_mutex_lock(&list_mutex);
	time_t process_time = get_process_time(children_list, pid);
	pthread_mutex_unlock(&list_mutex);
	return process_time;
}

#define NO_FORK_SLOT (forked_children - waited_children < MAXPAR)
#define NO_WAIT_SLOT (forked_children > waited_children)

pid_t synced_wait(int* status) 
{
	pthread_mutex_lock(&children_mutex);

	while (NO_WAIT_SLOT) {
	
	        if (exit_called()) return -2;
	        printf("\n>>> "); // print a nice prompt
		pthread_cond_wait(&can_wait, &children_mutex);
	}
	pthread_mutex_unlock(&children_mutex);

	else return wait(status); // XXX: waits for next child to terminate
}

pid_t synced_fork(void)
{

	pthread_mutex_lock(&children_mutex);
				
	while (NO_FORK_SLOT) 
		pthread_cond_wait(&can_fork, &children_mutex);
	pthread_mutex_unlock(&children_mutex);	

	return fork();
}

void threading_init(list_t* _children_list)
{ 
        pthread_t par_wait;
        children_list = _children_list; 
        // pthread_create returns non-zero if it fails.
        if(pthread_create(&thread_, NULL, monitor, NULL)) {
       	        perror(EMSG_THREADCREATE)
       	        exit(1);
       	} // multi-threading starts here
}


void threading_cleanup(void)
{
        // pthread_join returns non-zero if it fails.
        
        if (pthread_join(thread_monitor, NULL))
	        perror("par-shell: couldn't join with monitor thread");
		
	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&children_mutex);
	pthread_mutex_destroy(&exit_called_mutex);
	pthread_cond_destroy(&can_fork);
	pthread_cond_destroy(&can_wait);
}

#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "list.h"
#include "par_wait.h"

#define EMSG_THREADCREATE "par-shell: Couldn't create waiting thread. Aborting."

static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t can_fork;
static sem_t can_wait;
static const unsigned int MAXPAR = 8; 
static list_t* children_list;
static pthread_t par_wait_thread;

void regist_fork(int pid, time_t starttime)
{ 
	pthread_mutex_lock(&list_mutex);
	insert_new_process(children_list, pid, starttime);
	pthread_mutex_unlock(&list_mutex);
	
	sem_post(&can_wait);	
}

void regist_wait(int pid, time_t endtime)
{ 
	pthread_mutex_lock(&list_mutex);
	update_terminated_process(children_list, pid, endtime);
	pthread_mutex_unlock(&list_mutex);
	
	sem_post(&can_fork);
}

time_t get_finish_time(int pid)
{ 
	pthread_mutex_lock(&list_mutex);
	time_t process_time = get_process_time(children_list, pid);
	pthread_mutex_unlock(&list_mutex);
	return process_time;
}

pid_t synced_wait(int* status) 
{
        sem_wait(&can_wait);
        return wait(status);
}

pid_t synced_fork(void)
{
        sem_wait(&can_fork);
        return fork();
}

void threading_init(list_t* _children_list)
{ 
        children_list = _children_list;
         
        sem_init(&can_wait, 0, 0);
        sem_init(&can_fork, 0, MAXPAR);
        
        if (pthread_create(&par_wait_thread, NULL, par_wait, NULL)) {
       	        perror(EMSG_THREADCREATE);
       	        exit(1);
       	} // multi-threading starts here ONLY
}

void threading_cleanup(void)
{
        // pthread_join returns non-zero if it fails.
        if (pthread_join(par_wait_thread, NULL))
	        perror("par-shell: couldn't join with monitor thread");
	
	int done;
	sem_getvalue(&can_wait, &done);
        while (!done) { 
                sem_wait(&can_wait);
                sem_getvalue(&can_wait, &done);
        }
                
        pthread_kill(SIGTERM, par_wait_thread);
        
	pthread_mutex_destroy(&list_mutex);
	sem_destroy(&can_fork);
	sem_destroy(&can_wait);
}

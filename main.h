#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <stdbool.h>
#define MAXPAR 8

/* can_fork: semaphore indicating being able to fork more children 
 * can_wait: semaphore indicating there are unwaited children
 */
//extern sem_t can_fork;		//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX legacy
extern pthread_cond_t fork_cond;	//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX declaracao da var condicao
extern int can_fork;			//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX declaracao da condicao em si
extern sem_t can_wait;
extern pthread_mutex_t fork_mutex;	//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX declaracao do mutex

/* The following is an atomic API with setters and getters
 * for the static global variables defined in main.c.
 * Mutual exclusion is guaranteed.
 */

/*increments total forked children*/
void atomic_inc_children_count(void);

/*increments total waited on children*/
void atomic_inc_waited_children(void);

/*checks whether exit was called*/
bool atomic_get_exit_called(void); 

/*new node into list containing info about all children*/
void atomic_insert_new_process(int pid, time_t starttime);

/*update endtime of terminated child with pid 'pid' */
void atomic_update_terminated_process(int pid, time_t endtime); 

#endif


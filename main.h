#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

extern sem_t can_exec;
extern sem_t can_wait;

/*  
 *  API atomica: Depois desta excelente ideia, o Daniel vai ter de admitir 
 *  que eu sou um excelente programador. XXX XXX XXX XXX XXX XXX
 */


void atomic_inc_children_count(void);
void atomic_inc_waited_children(void);
bool atomic_exit_called(void);

void atomic_insert_new_process(int pid, time_t starttime);
void atomic_update_terminated_process(int pid, time_t endtime);


#if 0
extern unsigned int children_count; // counts all successfully forked and execved children.
extern unsigned int waited_children; // counts all successfully waited on children.
extern int exit_called;	// boolean: >0 if user input exit, 0 if user has not yet.
extern list_t* children_list; // linked list saving data about all children.
extern pthread_mutex_t main_mutex; // mutex for accessing these variables. Must always be used when doing so.
extern sem_t children_sem;
#endif

#endif



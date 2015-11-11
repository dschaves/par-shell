#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <stdbool.h>

extern pthread_cond_t can_fork;
extern pthread_cond_t can_wait;

/** WARNING:
 *  below mutex must always be locked before... */
extern pthread_mutex_t children_mutex;
//...calling these functions:
inline void inc_waited_children(void);
inline void inc_forked_children(void);
inline bool fork_slot_avaliable(void);
inline bool wait_slot_avaliable(void);
// end WARNING

/* Unlike the ones above, all following functions automatically
 *  guarantee mutual exclusion when called. */

/*checks whether exit was called*/
bool atomic_get_exit_called(void); 

/*new node into list containing info about all children*/
void atomic_insert_new_process(int pid, time_t starttime);

/*update endtime of terminated child with pid 'pid' */
void atomic_update_terminated_process(int pid, time_t endtime); 

#endif


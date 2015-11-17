#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <stdbool.h>

inline void atomic_inc_waited_children(void);
inline void atomic_inc_forked_children(void);

bool exit_called(void);

void set_exit_called(void);

time_t get_finish_time(int pid);

pid_t par_wait(int* status);

pid_t synced_wait(int* status);

pid_t synced_fork(void); 

void threading_init(list_t*);

void threading_cleanup(void);

#endif


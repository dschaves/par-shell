#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <pthread.h>
#include "list.h"

// Global variables. I cringed too. But it's ok, it's better this way.
extern unsigned int children_count;
extern unsigned int waited_children;
extern int exit_called;
extern pthread_mutex_t main_mutex;
extern list_t* children_list;

#endif

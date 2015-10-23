#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <pthread.h>
#include "list.h"

// Global variables. I cringed too. But it's ok, it's better this way.

extern unsigned int children_count; // counts all successfully forked and execved children.
extern unsigned int waited_children; // counts all successfully waited on children.
extern int exit_called;	// boolean: >0 if user input exit, 0 if user has not yet.
extern list_t* children_list; // linked list saving data about all children.
extern pthread_mutex_t main_mutex; // mutex for accessing these variables. Must always be used when doing so.
#endif

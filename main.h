#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <pthread.h>
#include "list.h"

extern list_t* children_list;  
extern int children_count;
extern int exit_called;
extern pthread_mutex_t mutex;

#endif

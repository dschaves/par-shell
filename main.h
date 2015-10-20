#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H
#define MAX_CHILDREN 8192

struct main_status {

	int exit_called;
	int children_count;
	int children_pid_arr[MAX_CHILDREN];
	int children_status_arr[MAX_CHILDREN];
	time_t children_time_arr[MAX_CHILDREN];
 
};
#endif

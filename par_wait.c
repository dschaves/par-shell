#include <time.h> // time()
#include <stdio.h> // perror()

#include "par_wait.h" // self
#include "par_sync.h"

static unsigned int iteration_count = 0;
static unsigned time_t total_time = 0;

void* monitor(void*_)
{
	pid_t pid;
	time_t endtime;
	
	FILE* log_file = fopen("log.txt", "a+");
	
	while (pid != -2 && !exit_called()) { // pid == -2 signals exit has been called	

		pid = synced_wait(NULL);

		if ((endtime = time(NULL)) == -1)
		        perror("par-shell: [ERROR] couldn't get finish time for child.")
		
                switch (pid) {
                
                case -1: perror("par-shell: [ERROR] couldn't wait on child");  
                case -2: break;
                default:
                        signal_fork(pid, endtime); 
			save_log(log_file, get_finish_time(pid))
                }
	
	}
	
	fclose(log_file);
	 
	return NULL; 		
}


FILE* load_log_file(void)
{
	time_t prev_time = 0;
	time_t prev_iter = 0;
	
	char first_line[BUFFER_SIZE];
	char third_line[BUFFER_SIZE];

	while (fgets(first_line, BUFFER_SIZE, log_file)) {
	
		while (fgetc(log_file) != '\n');
		fgets(third_line, BUFFER_SIZE, log_file);
	}

	sscanf(first_line, "iteracao %u", &prev_iter); 
	sscanf(third_line, "total execution time: %u", &prev_time);
	
	total_time += prev_time;
	iteration_count += prev_iter;

        return log_file;
}



void save_log_file(FILE* log_file, time_t finish_time)
{       
        fprintf(log_file, "iteracao %d\npid: %d execution time: %d s\ntotal execution time: %d s\n", iteration_count, pid, (int) process_time, (int) total_time);
	fflush(log_file);
	++iteration_count;
	total_time += finish_time; 
}

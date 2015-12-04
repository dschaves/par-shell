#include <time.h> // time()
#include <stdio.h> // perror()

#include "par_wait.h" // self
#include "par_sync.h"

static unsigned int iteration_count;
static time_t total_time;
#define BUFFER_SIZE 128

// getters a la Java for the above static variables.
unsigned int get_iteration_count() {return iteration_count;}
time_t get_total_time() {return total_time;}

static void read_log_file(FILE* log_file)
{	
        printf("Reading log.txt: ");

	if (fgetc(log_file) == EOF) {
	
	        puts("Log empty.");
	        iteration_count = 0;
	        total_time = 0;
	        return;
	        
	} ungetc('i', log_file);
	
	time_t prev_time;
	unsigned int prev_iter;
	char first_line[BUFFER_SIZE];
	char third_line[BUFFER_SIZE];
	
	while (fgets(first_line, BUFFER_SIZE, log_file)) {
	
		while (fgetc(log_file) != '\n'); // discard second line.
		fgets(third_line, BUFFER_SIZE, log_file);
	}
	sscanf(first_line, "iteracao %u", &prev_iter); 
	sscanf(third_line, "total execution time: %u", (unsigned int*) &prev_time);
	
	total_time = prev_time;
	iteration_count = prev_iter+1;
	
	printf("Previous iterations: %d. Previous time: %d s\n", iteration_count, (int) prev_time);
}


static void save_log_file(FILE* log_file, pid_t pid, time_t finish_time)
{       
	total_time += finish_time; 
        fprintf(log_file, "iteracao %d\npid: %d execution time: %d s\ntotal execution time: %d s\n", iteration_count, pid, (int) finish_time, (int) total_time);
        ++iteration_count;
	fflush(log_file);
}


void* par_wait(void*_)
{
	pid_t pid;
	time_t endtime;
	
	FILE* log_file = fopen("log.txt", "a+");

        if (log_file == NULL) 
                perror("Couldn't read log.txt. Continuing without logging.");
	else 
	        read_log_file(log_file);
	
	for(;;) {

		pid = synced_wait(NULL);
		
		if ((endtime = time(NULL)) < 0)
		        perror("par-shell: couldn't get finish time for child");
		
                if (pid == -1) 
                        perror("par-shell: couldn't wait on child");
                        
                else {
                        regist_wait(pid, endtime); 
			save_log_file(log_file, pid, get_finish_time(pid));
                }
	}
	
	fclose(log_file);	 
	return NULL; 		
}

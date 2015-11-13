#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <pthread.h> 

#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self

void* monitor(void* _log_file)
{

	FILE* log_file = (FILE*) _log_file;	
	time_t endtime;
	pid_t pid;

	for(;;) {	

		pthread_mutex_lock(&children_mutex);

		while (!wait_slot_avaliable()) {

			if (atomic_get_exit_called()) {

				pthread_mutex_unlock(&children_mutex); 
				goto exit; 
			}
			
			pthread_cond_wait(&can_wait, &children_mutex);
		}		

		pthread_mutex_unlock(&children_mutex);


		pid = wait(NULL); // XXX: waits for next child to terminate

		time(&endtime);

		if (endtime == -1)
			perror("par-shell: [ERROR] couldn't get finish time for child");

		if (pid == -1)
			perror("par-shell: [ERROR] couldn't wait on child");  

		else {
			atomic_update_terminated_process(pid, endtime);
			atomic_inc_waited_children();

			pthread_cond_signal(&can_fork);

			unsigned int process_time = atomic_get_process_time(pid);

			total_time += process_time;	

			fprintf(log_file, "iteracao %d\npid: %d execution time: %d s\ntotal execution time: %d s\n", iteration_count++, pid, (int) process_time, (int) total_time);
			
		}

	}
		exit: return NULL;
}



#include <stdio.h> // getline()
#include <string.h> // strcmp(), puts()
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>

#include "list.h"
#include "par_sync.h"

#define MSG_PROMPT "Par-shell now ready. Does not wait for jobs to exit!"

#define BUFFER_SIZE 128
#define CHILD_ARGV_SIZE 7
#define EXIT_RETURN -2
#define EXIT_STRING "exit"
#define EXITGLOBAL_RETURN -3
#define EXITGLOBAL_STRING "exit-global"
#define eq_str(str1,str2) (!strcmp(str1,str2)) 

/* get_child_argv:
Reads up to 'vectorSize' space-separated arguments from the standard input
and saves them in the entries of the 'argVector' argument.
This function returns once enough arguments are read or the end of the line 
is reached

Arguments: 
 'argv' should be a vector of char* previously allocated with
 as many entries as 'argv_size'
 'vectorSize' is the size of the above vector. A vector of size N allows up to 
 N-1 arguments to be read; the entry after the last argument is set to NULL.
 'buffer' is a buffer with 'buffersize' bytes, which will be 
 used to hold the strings of each argument.  

Return value:
 The number of arguments that were read; -1 if some error occurred; 
 -2 if the first token input is "exit."
*/


void par_run(char* argVector[]) 
{
	pid_t pid = synced_fork();

	char filename[BUFFER_SIZE];
	
        switch (pid) {
                
                /* Is forked child: */  
                case 0: sprintf(filename, "par-shell-out-%d.txt", getpid());
			int output_file = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
			dup2(output_file, STDOUT_FILENO);
			close(output_file);

                        execv(argVector[0], argVector);

		        if (argVector[0][0] != '/') 
			execvp(argVector[0], argVector);

		        // next line only reached if execv fails.
		        perror("par-shell: exec failed");
		        _exit(EXIT_FAILURE); // better way of exiting.
		        
		/* Is parent (par-shell, main): */
                case -1: perror("par-shell: unable to fork"); break;
		default: regist_fork(pid, time(NULL));
	}
}

void accept_remotes(int _accept_pipe, char* child_argv[])
{
        static char read_from_path[BUFFER_SIZE];
        
        FILE* accept_pipe = fdopen(_accept_pipe, "r");
        
        if (fgets(read_from_path, child_argv, accept_pipe) < 0)
                perror("par-shell: couldn't accept connection");

        return read_from_path;
}

int main(void) 
{	 
	char* child_argv[CHILD_ARGV_SIZE];
        int accept_pipe = mkfifo("/tmp/par-shell", S_IWUSR | S_IRUSR);
        
        list_t* children_list = lst_new();
       
        threading_init(children_list); /** NOTE: initiates multi-threading. */

        puts(MSG_PROMPT);
	
	/** MAINLOOP:
	  * The program's main logic is dictated next. */
        //TODO!
        while(!exit_called()) {
	        listen_for_argv(fifo, child_argv);
	}
	
        threading_cleanup();
        lst_print(children_list);
	lst_destroy(children_list);

	return EXIT_SUCCESS;	
}

void decide_input(char* argv[])
{
        char* command = argv[0];
        
        if (eq_str(command, EXIT_STRING)) command_exit();
        else if (eq_str(command, EXITGLOBAL_STRING)) command_exitglobal();
        else if (eq_str(command, STAT_STRING)) command_stat();
        
        else par_run(argv);
}



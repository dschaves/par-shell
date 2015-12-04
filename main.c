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

static struct remote* remote_first = NULL;

struct remote {

        pid_t pid;
        int par_shell_out;
        struct remote* next;
};

/* get_child_argv:
Reads all whitespace-separated tokens from the string "command_line"
and saves them in the entries of the 'argv' argument.
This function stops reading when argv arguments are read or a NULL is reached.

Arguments: 
 'argv' should be a vector of char* previously allocated with
 as many entries as 'argv_size'. 
 'command_line' is a string with which will be divided via strtok
 to hold the strings of each argument.
  
 Warning: argv will solely contain pointers to indexes in 'command_line' so 
 'argv's entries are only valid if 'command_line' is in scope.

Return value:
The number of arguments that were read; -1 if any argument evals to false,
because we can't do anything like that.*/

int get_child_argv(char command_line[], char* argv[], size_t argv_size)
{
        char* token;			    // each read token from input. 
        const char delimiters[] =" \t\n";   // strtok-ending characters
        int i, argc;                        // array index; tokens found count
        
        if (!command_line || !argv || !argv_size) return -1;
     
	token = strtok(command_line, delimiters); // this token is the command 

        /* Preencher o vector argv com todos os tokens encontrados
         * ate ultrapassar o tamanho do vector ou chegar a um NULL. */
        for (argc = 0; argc < argv_size-1 && token != NULL; argc++) {
        
                argv[argc] = token;
                token = strtok(NULL, delimiters);
        } 
        
        /* Fill all remaining spaces with null terminators. */
        for (i = argc; i < argv_size; i++) argv[i] = NULL;

        return argc;
}

void par_run(char* argVector[]) 
{
	pid_t pid = synced_fork();
	
	if (!pid) { // Is forked child.

	        char filename[BUFFER_SIZE];
                /* First we redirect the child's output into a file. */
                sprintf(filename, "par-shell-out-%d.txt", getpid());
	        int output_file = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	        dup2(output_file, STDOUT_FILENO);
	        close(output_file);
                /* Then we exec the child. */
                execv(argVector[0], argVector);
                if (argVector[0][0] != '/') 
	                execvp(argVector[0], argVector);
                /* Next line only reached if execv fails. */
                perror("par-shell: exec failed");
                exit(EXIT_FAILURE); // better way of exiting.       
	}
	
	if (pid > 0) perror("par-shell: unable to fork");
	
	else regist_fork(pid, time(NULL));
	
}

int main() 
{	 
	/* TODO: pipes should be in current working dir. 
	   Use: get_current_dir_name(). But you have to make buffers etc.*/
	char* argv[CHILD_ARGV_SIZE];
	char* command_line = NULL;
	size_t command_size = 0;
        int par_shell_in = mkfifo("/tmp/par-shell-in", O_CREAT S_IRUSR);
        
        signal(SIGINT, exitglobal);
        list_t* children_list = lst_new();
        threading_init(children_list); /** NOTE: initiates multi-threading. */
	
	/* Main loop. The program's main logic is dictated next. */
        for(;;) {
                
                if (strtol(argv[0], NULL, 10)) stats(argv);
                else if (eq_str(argv[0], EXITGLOBAL_STRING)) exitglobal();
                else if (eq_str(argv[0], STATS_STRING)) acknowledge(par_shell_out);
                else par_run(argv);
        }
}

void acknowledge(char* argv[])
{
        (pid_t) pid = strtol(argv[0], NULL, 10);
        char* par_shell_out_path = argv[1];
        
        if (known_remote(pid)) stats(pid);
        
        else {
                struct remote* new = new_remote(pid, par_shell_out_path);
                insert_remote(new);
        }
}

struct remote* new_remote(pid_t pid, char* par_shell_out_path)
{
        struct remote_shell new = malloc(sizeof(struct remote_shell));
        new->pid = pid;
        new->par_shell_out = open(par_shell_out,)
        if (new->par_shell_out > 0) perror("couldn't open par-shell-out"), exit(1);

        return new_sh;
}

void insert_remote(struct remote* remote)
{
        remote->next = remote_first;
        remote_first = remote;
}

bool known_remote(pid_t target)
{
        struct remote* next;
        struct remote* this = remote_first;
       
        while (this) {
                next = this->next; 
                if (this->pid == target) return true;
                this = next;
        } return false;
}

void exitglobal(list_t* children_list)
{
        struct remote* next;
        struct remote* this = remote_first;
        
        while (this) {
                next = this->next; 
                kill(SIGTERM, this->pid);
                free(this);
                this = next;
        }
        
        threading_cleanup();
        lst_print(children_list);
	lst_destroy(children_list);
	exit(EXIT_SUCCESS);
}




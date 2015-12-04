#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128
#define CHILD_ARGV_SIZE 7
#define EMSG_INPUT "\npar-shell: couldn't read input"
#define EMSG_PIPE "\npar-shell: couldn't open pipe"

void input_error()
{
        free(input); 
        perror(EMSG_INPUT);
        exit(EXIT_FAILURE); 
}

void pipe_error()
{
        free(input); 
        perror(EMSG_PIPE);
        exit(EXIT_FAILURE); 
}

void print_stats(int par_shell_out, char buffer[], size_t buffer_size )
{   
        getline(&buffer, &buffer_size, fdopen(par_shell_out, "r")); // get number of iters
        printf("\nIterations: %s", buffer);
        getline(&buffer, &buffer_size, fdopen(par_shell_out, "r"));
        printf("\nTotal time: %s", buffer);
}

/* Buffer should be a malloc string! */
int regist_self(int par_shell_in, char par_shell_out_path[], char buffer[], size_t buffer_size)
{
        int par_shell_out = mkfifo(par_shell_out_path, 
        O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
        
        if (par_shell_out < 0) pipe_error();
        
        sprintf(buffer, "%d %s", getpid(), par_shell_out_path);
        
        if (write(par_shell_in, buffer, strlen(buffer)+1) < 0)
                pipe_error();
        
        return par_shell_out;
}       

int main()
{
        char* input = NULL; // for getline
        size_t size = 0; // for getline
        size_t input_len; 
        int par_shell_in;
        int par_shell_out;

        printf("\nPath for par-shell-in: ");
        if (getline(&input, &size, stdin) < 0) input_error();
        strtok(input, "\n"); // strip newline so this is a proper directory
        par_shell_in = open(input, O_WRONLY);
        if (par_shell_in < 0) pipe_error();
        
        printf("\nPath for par-shell-out: ");
        if (getline(&input, &size, stdin) < 0) input_error();
        strtok(input, "\n"); //strip newline
        par_shell_out = fopen(input, "r");
        if (!par_shell_out) goto pipe_error;

	for(;;) {
	
                printf("\n>>> ");
                input_len = getline(&input, &size, stdin);
                
                if (input_len < 0) perror(EMSG_INPUT);
                
                else if (input_len > 0) { 
                
                        if (strncmp("exit", input, 4)) break;
                        
                        write(par_shell_in, input, input_len+1); // XXX
                        
                        if (strncmp("stats", input, 5)) 
                                print_stats(par_shell_out);
                }
        }
}

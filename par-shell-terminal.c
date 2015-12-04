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
#define MSG_IN "\nPath for par-shell-in pipe (existing): "
#define MSG_OUT "\nPath for par-shell-out pipe (will be created): "

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

void get_stats(FILE* par_shell_out, FILE* par_shell_in, char buffer[], size_t buffer_size)
{   
        getline(&buffer, &buffer_size, fdopen(par_shell_out, "r")); // get number of iters
        printf("\nIterations: %s", buffer);
        getline(&buffer, &buffer_size, fdopen(par_shell_out, "r"));
        printf("\nTotal time: %s", buffer);
}

/* Buffer should be a malloc string! */
int regist_self(int par_shell_in, char par_shell_out_path[], char buffer[], size_t buffer_size)
{

        if (par_shell_out < 0) pipe_error();
        
        sprintf(buffer, "%d %s", getpid(), par_shell_out_path);
        
        if (write(par_shell_in, buffer, strlen(buffer)+1) < 0)
                pipe_error();
        
        return par_shell_out;
}       

FILE* get_par_shell_in(char path[], size_t size)
{
        printf(MSG_IN);
        
        if (getline(&path, &size, stdin) < 0) input_error();
        
        strtok(path, "\n"); // strip newline so this is a proper directory

        FILE* par_shell_in = fopen(input, "r");
        
        if (!par_shell_in) pipe_error();
        
        return par_shell_in;
}

int get_par_shell_out(char path[], size_t size )
{
        printf(MSG_OUT);
        
        if (getline(&path, &size, stdin) < 0) input_error();
        
        strtok(path, "\n"); //strip newline
        
        int fd = mkfifo(path, S_IRUSR | S_IWUSR);
        
        if (fd < 0) pipe_error();
        
        return fd;
}

int main()
{
        char* input = NULL; // for getline
        size_t size = 0; // for getline
        size_t input_len; 
        FILE* par_shell_in = get_par_shell_in(input);
        FILE* par_shell_out = get_par_shell_out(output);
        
	for(;;) {
	
                printf("\n>>> ");
                input_len = getline(&input, &size, stdin);
                
                if (input_len < 0) perror(EMSG_INPUT);
                
                else if (input_len > 0) { 
                
                        if (strncmp("exit", input, 4)) break;
                        
                        fputs(input, par_shell_in); // XXX
                        
                        if (strncmp("stats", input, 5)) 
                                get_stats(par_shell_out, par_shell_in, line, size);
                }
        }
        
        return EXIT_SUCCESS;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "par_sync.h"

#define BUFFER_SIZE 128
#define CHILD_ARGV_SIZE 7
#define EMSG_INPUT "\npar-shell: couldn't read input"
#define EMSG_PIPE "\npar-shell: couldn't open pipe"

void print_stats(FILE* par_shell_out)
{
        static char stats[128];
          
}

int main()
{
        char* input = NULL;
        size_t size = 0;
        size_t input_len;
        int par_shell_in;
        FILE* par_shell_out;

        printf("\nPath for par-shell-in: ");
        if (getline(&input, &size, stdin) < 0) goto input_error;
        strtok(input, "\n"); // strip newline so this is a proper directory
        par_shell_in = open(input, O_WRONLY);
        if (par_shell_in < 0) goto pipe_error;
        
        printf("\nPath for par-shell-out: ");
        if (getline(&input, &size, stdin) < 0) goto input_error;
        strtok(input, "\n"); //strip newline
        par_shell_out = fopen(input, "r");
        if (!par_shell_out) goto pipe_error;

	for(;;) {
	
                printf("\n>>> ");
                input_len = getline(&input, &size, stdin);
                
                if (input_len < 0) perror(EMSG_INPUT);
                
                else if (input_len > 0) { 
                
                        if (strncmp("exit", input, 4)) goto success; 
                        
                        write(par_shell_in, input, input_len+1);
                        
                        if (strncmp("stats", input, 5)) 
                                print_stats(par_shell_out);
                }
        }
        
        success: free(input); return EXIT_SUCCESS;
        input_error: free(input); perror(EMSG_INPUT); return EXIT_FAILURE;      
        pipe_error: free(input); perror(EMSG_PIPE); return EXIT_FAILURE;
}

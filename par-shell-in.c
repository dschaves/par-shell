#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "par_sync.h"

#define BUFFER_SIZE 128
#define CHILD_ARGV_SIZE 7
#define PIPE_DIR /tmp/par-shell
#define EMSG_INPUT "\npar-shell: couldn't read input. Retrying.\n"

int get_child_argv(char* argv[], size_t argv_size)
{
        char* token;				        /* each read token from input. */
        const char delimiters[] =" \t\n";		/* caracteres que acabam strtok */
        int i, argc;                        		/* array index and
                                                         * the number of arguments found.*/
        static char input[BUFFER_SIZE];
        
        fflush(stdout);
        if (!fgets(input, BUFFER_SIZE, stdin)) return 0;
     
        /* Preencher o vector tokens com todos os tokens encontrados
         * ate ultrapassar o tamanho do vector ou chegar a um NULL.    
	 */
	token = strtok(input, delimiters); 		/* este token e o comando */ 
	
	if (token != NULL && !strcmp(token, "exit")) return -2; 

        for (argc = 0; argc < argv_size && token != NULL; argc++) {
                argv[argc] = token;
                token = strtok(NULL, delimiters);
        } 
        
        for (i = argc; i < argv_size; i++) argv[i] = NULL;

        return argc;
}

void par_req(char* commands[], int numtokens)
{
        int i;
        int pipe = open(commands[0], O_WRONLY);

        for (i = 1; i < numtokens; i++)
                write(pipe, commands[i], strlen(commands[i])+1);
}

void stat_command()
{

}

int main()
{
        char* child_argv[BUFFER_SIZE];
        int numtokens;

	while (!exit_called())
	{
                numtokens = get_child_argv(child_argv, CHILD_ARGV_SIZE);
                
                switch (numtokens) {
	       
	                // -2 means exit called
	                case -2: set_exit_called(); break;
	                // -1: Error. print message and ask again
	                case -1: fputs(EMSG_INPUT, stderr);
	                //0: nothing input: ask again
		        case 0: continue;
		        // everything ok: fork requested command
		        default: par_req(child_argv, numtokens);
                }
        }
        return EXIT_SUCCESS;
}

#include <stdio.h> // getline()
#include <string.h> // strcmp(), puts()
#include <unistd.h>

#include "list.h"
#include "par_sync.h"

#define MSG_PROMPT "Par-shell now ready. Does not wait for jobs to exit!"
#define EMSG_INPUT "\npar-shell: couldn't read input. Retrying.\n"


#define INPUT_SIZE 128
#define CHILD_ARGV_SIZE 7

char* get_input(void)
{               
        static char input[INPUT_SIZE];
        fgets(input, INPUT_SIZE, stdin);
        return input;
}


/* get_child_argv:
Reads up to 'vectorSize' space-separated arguments from the standard input
and saves them in the entries of the 'argVector' argument.
This function returns once enough arguments are read or the end of the line 
is reached

Arguments: 
 'argVector' should be a vector of char* previously allocated with
 as many entries as 'vectorSize'
 'vectorSize' is the size of the above vector. A vector of size N allows up to 
 N-1 arguments to be read; the entry after the last argument is set to NULL.
 'buffer' is a buffer with 'buffersize' bytes, which will be 
 used to hold the strings of each argument.  

Return value:
 The number of arguments that were read; -1 if some error occurred; 
 -2 if the first token input is "exit."
*/
int get_child_argv(char* argv[], size_t argv_size)
{

        char* token;				        /* cada token encontrado */
        const char delimiters[] =" \t\n";		/* caracteres que acabam strtok */
        unsigned int i, argc;               		/* indice do array */
     
        char* input = get_input();
        /* Preencher o vector tokens com todos os tokens encontrados
         * ate ultrapassar o tamanho do vector ou chegar a um NULL.    
	 */
	token = strtok(input, delimiters); 		/* este token e o comando */ 

        for (i = argc = 0; i < argv_size; i++) {
        
                argv[i] = token; if (token == NULL) puts("EH CARALHO!");
                if (token != NULL) argc++;
                token = strtok(NULL, delimiters);
        }

        return argc;
}

/* este programa do sézinho é lindo. é favor dar um 20.*/

void par_run(char* argVector[]) 
{
	pid_t pid = synced_fork();

        switch (pid) {
        
                case -1: perror("par-shell: unable to fork"); break;
                
                case 0: /* Is forked child: */
                        execv(argVector[0], argVector);

		        if (argVector[0][0] != '/')		
			execvp(argVector[0], argVector);

		        // next line only reached if execv fails.
		        perror("par-shell: exec failed");
		        exit(EXIT_FAILURE);
		        
		 /* Is parent (par-shell, main): */
		default: regist_fork(pid, time(NULL));	
	}
}

int main(void) 
{	 
	char* child_argv[CHILD_ARGV_SIZE];
        
        list_t* children_list = lst_new();
       
        threading_init(children_list); /** NOTE: initiates multi-threading. */

        puts(MSG_PROMPT); 
	
	/** User interaction followed by command execution happens next,
	  * in the while loop. 
	  * This is the shell's main part.*/
	  
	while (!exit_called()) { 
	
	        switch (get_child_argv(child_argv, CHILD_ARGV_SIZE)) {
	       
	                case -2: 
	                        set_exit_called(); break;
	                case -1: 
	                        fputs(EMSG_INPUT, stderr);
		        case 0: 
		               continue;
		        default: 
		                par_run(child_argv);
                }	
	}
	
        threading_cleanup();
        lst_print(children_list);
	lst_destroy(children_list);

	return EXIT_SUCCESS;	
}

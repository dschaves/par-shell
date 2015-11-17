#include <stdio.h> // getline()
#include <string.h> // strcmp(), puts()

#include "list.h"
#include "par_sync.h"
#include "unistd.h"

#define CHILD_ARGV_SIZE 7
#define BUFFER_SIZE 128
#define MSG_PROMPT "Par-shell now ready. Does not wait for jobs to exit!"

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
int get_child_argv(char* argVector[], int vectorSize)
{
  static char buffer[BUFFER_SIZE]; 
  
  int numtokens = 0;
  char *s = " \r\n\t";

  int i;

  char *token;

  if (argVector == NULL || vectorSize == 0 || BUFFER_SIZE == 0)
    return 0;

  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
    return -1;
   
  /* get the first token */
  token = strtok(buffer, s);
  
  if (!strcmp(token, "exit")) return -2; 
  
  /* walk through other tokens */
  while( numtokens < vectorSize-1 && token != NULL ) {
  
    argVector[numtokens] = token;
    numtokens ++;
    
    token = strtok(NULL, s);
  }
   
  for (i = numtokens; i<vectorSize; i++)
    argVector[i] = NULL;
   
  return numtokens;
}

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
		default: signal_child_fork(pid, time(NULL));
}

int main(int argc, char* argv[]) 
{	
	char* child_argv[CHILD_ARGV_SIZE];
        pthread_t thread_monitor;
        
        list_t* children_list = lst_new();
       
        threading_init(children_list);

        puts(MSG_PROMPT); 
	
	/** The interaction with the user and  command execution happens next. 
	  * This is the shell's main part.*/
	while (!exit_called()) { 
	
	        switch (get_child_argv(child_argv, CHILD_ARGV_SIZE)) {
	       
	                case -2: set_exit_called(); break;
		        case -1: fputs("\npar-shell: couldn't read input. Retrying.\n", stderr);
		        case 0: continue;
		        default: par_run(child_argv);
                }	
	}
	
        threading_cleanup();
        lst_print(children_list);
	lst_destroy(children_list);

	return EXIT_SUCCESS;	
}

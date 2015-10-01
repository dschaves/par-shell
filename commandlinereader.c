#include <string.h> 
#include <stdio.h>
#include "commandlinereader.h"

int commandlinereader(char input[], char* tokens[], int argc_max)
{				
 	        
        char* token;						    /* cada token encontrado */
        const char delimiters[] = " \t\n";		/* caracteres que acabam strtok */
        int numtokens = 0;						/* indice do array */

        token = strtok(input, delimiters); 		/* este token e o comando */

        //for (numtokens = 0; numtokens <= argc_max && token != NULL ; numtokens++)
	    /* Preencher o vector tokens com todos os tokens encontrados, 
         * ate ultrapassar o tamanho do vector ou chegar a um NULL.    
	     * etc
	     */
        while ((tokens[numtokens++] = token) && numtokens < argc_max+2) 
            token = strtok(NULL, delimiters);

        if (token != NULL) // se houver demasiados argumentos 
            return -1;

        return numtokens;
}



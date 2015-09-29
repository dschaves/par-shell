#define ARGC_MAX 7

char** commandlinereader(char input[], char* tokens[], int argc_max)
{
        char* tokens[ARGC_MAX+1];				/* +1 para o comando */
        char* token;						/* cada token encontrado */
        const char delimiters[] = " \t\n";			/* caracteres que acabam strtok */
        int num_tokens;							/* indice do array */

        token = strtok(input, delimiters); 			/* este token e o comando */

        for (i = 0 ; i <= ARGC_MAX && token != NULL ; i++)
	/* Preencher o vector tokens com todos os tokens encontrados, ate ultrapassar o tamanho do vector ou chegar a um NULL.
	* etc
	*/	
        {
                tokens[i] = token;
                token = strtok(NULL, delimiters);
        }

        if (tokens[i] != NULL)
        {
		free(tokens);		
		printf("Too many arguments given. Maximum arguments allowed is %d.\n", ARGC_MAX)
		return NULL;
	}

        return i;
}

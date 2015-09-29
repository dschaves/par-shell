char** divide_linha(char input[], int argc_max)
{
        char** tokens = malloc(sizeof(char)*(argc_max+1));	/* +1 para o comando */
        char* token;						/* cada token encontrado */
        const char delimiters[] = " \t\n";			/* caracteres que acabam strtok */
        int i;							/* indice do array */

        token = strtok(input, delimiters); 			/* este token e o comando */

        for (i = 0 ; i <= argc_max && token != NULL ; i++)
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
		puts("Too many arguments given.")
		return NULL;
	}

        return tokens;
}

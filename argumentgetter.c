char** divide_linha(char input[], int argc_max)
{
        char** tokens = malloc(sizeof(char)*(argc_max+1)); /* +1 para o comando */
        char* token;
        const char delimiters[] = " \t\n";
        int i;

        token = strtok(input, delimiters); /* este token e o comando */

        for (i = 1 ; i <= argc_max && token != NULL ; i++)
        {
                tokens[i] = token;
                token = strtok(NULL, delimiters);
        }

        if (i > argc_max)
                return puts("Too many arguments given."), NULL;

        return tokens;
}

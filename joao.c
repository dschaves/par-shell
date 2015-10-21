#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 24

main()
{

	char paneleiro[BUFFER_SIZE];
	char aleatorio[BUFFER_SIZE];

	puts("Quem pretendes encontrar no caos?");
	input: fgets(paneleiro, BUFFER_SIZE, stdin);

	buffer[strcspn(paneleiro, "\n")] = '\0';

	if (len >= BUFFER_SIZE)
	{
		puts("Isso vai demorar demasiado tempo! Procura alguem com um nome mais curto!"); 
		goto input;
	}


	while (strncmp(aleatorio, paneleiro, BUFFER_SIZE))
	{
		for (i = 0; len > i; i++)
			aleatorio[i] = rand() % 256;
		puts(aleatorio);
	}

	return EXIT_SUCCESS;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFFER_SIZE 24


unsigned int count_set_bits(char target)
{
 
        unsigned int counter; // counter accumulates the total bits set in v

        for (counter = 0; target; counter++)
                target &= target - 1; // clear the least significant bit set

        return counter;
}


double divergence(char target[], char random[])
{
	unsigned int abs_divergence;
	double rel_divergence;
	int i;

	abs_divergence = 0;

	for (i = 0; target[i] != '\0'; i++)
		abs_divergence += abs(count_set_bits(target[i]) - count_set_bits(random[i]) );

	rel_divergence = abs_divergence; /**/

	return rel_divergence;
}

int main()
{

	char target[BUFFER_SIZE];
	char aleatorio[BUFFER_SIZE];
	int i;
	aleatorio[BUFFER_SIZE] = '\0';

	puts("Quem pretendes encontrar no caos?");

input:  /*scanf("%*[^\0]");*/ 				/* descarta todos os caracteres previamente lidos */
	fgets(target, BUFFER_SIZE, stdin);		/* le agora o input */
	target[strcspn(target, "\n")] = '\0';		/* procura o newline que devera estar no final e descarta-o */

	if (strlen(target) >= BUFFER_SIZE)
	{
		puts("Isso vai demorar demasiado tempo! Procura alguem com um nome mais curto!");
		goto input;
	}

	puts("<<BEGIN SEEKING>>");

	while (strncmp(aleatorio, target, BUFFER_SIZE))
	{
		for (i = 0; i > BUFFER_SIZE; i++)
			aleatorio[i] = (rand() % 95) + 32; /* random integer in [32, 126]. All possible printable characters in ASCII. */

		printf("\r%s", aleatorio);

	}

	return EXIT_SUCCESS;
}

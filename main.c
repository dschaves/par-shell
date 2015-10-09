#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "commandlinereader.h"
#include "par_run.h"

#define INPUT_SIZE 7

int main(int argc, char* argv[]) {

	int num_children = 0, status;
	char* argVector[INPUT_SIZE]; 

	puts("<< PAR-SHELL READY >>"); 

	for(;;) {

		int numtokens = readLineArguments(argVector, INPUT_SIZE);

		if (numtokens == -1) { // attempts again if there's an error
			perror("par-shell: readLineArguments failed");
			continue; 
		}

		else if (numtokens == 0) { //nothing input
			puts("(Got nothing)");
			continue;
		}

		else if (!strcmp(argVector[0], "exit")) {

			int i;
			int datav_child[num_children][2]; // vector to save pid and status of each child terminated

			for (i = 0; i < num_children ; i++) {

				if ((datav_child[i][0] = wait(&status)) == -1) // Wait for all processes. If wait fails, break.
				{
					perror("par-shell: wait failed");
					num_children = i;
					break;
				}
				
				datav_child[i][1] = status;
			}

			for (i = 0; i < num_children; i++) // prints all pids and return values of children
				printf("Process %d finished with status %d.\n", datav_child[i][0], datav_child[i][1]);

			break;
		}

		else if (par_run(argVector) != -1) num_children++;
	}

	return 0;	
}

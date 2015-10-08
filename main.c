#include <stdio.h>
#include <string.h>
#include "commandlinereader.h"
#include "par_run.h"
#include <sys/wait.h>

#define MAX_ARG 7

int main(int argc, char* argv[]){
	int num_child = 0, status;
	char* argVector[MAX_ARG]; 

	for(;;){
		int numtokens = readLineArguments(argVector, MAX_ARG);

		if (numtokens == -1) continue; // salta para a proxima interacao se houver erro
		else if (numtokens == 0){
			puts("(Got nothing)");
			continue;
		}
		else if(numtokens > MAX_ARG){
			printf("Too many arguments. Maximum is %d.\n", MAX_ARG-1);
			continue;
		}
		else if (!strcmp(argVector[0], "exit")){
			int i;
			int datav_child[num_child][2]; // vector to save pid and status of each child terminated
			for(i = 0; i < num_child; i++){
				datav_child[i][0] = wait(&status);
				datav_child[i][1] = status;
			}
			for(i = 0; i < num_child; i++){
				printf("Process %d finished with status %d.\n", datav_child[i][0], datav_child[i][1]);
			}
			return 0;
		}
		else {
			if(par_run(argVector) != -1) num_child++;
		}
	}
	return 0;	
}

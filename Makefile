# Makefile

par-shell: commandlinereader.o par_run.o main.o monitor.o list.o 
	gcc -pthread -g -o par-shell commandlinereader.o par_run.o main.o monitor.o list.o 

main.o: main.c main.h commandlinereader.h par_run.h monitor.h list.h
	gcc -pthread -Wall -g -c main.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -pthread -Wall -g -c commandlinereader.c

par_run.o: par_run.c commandlinereader.h par_run.h
	gcc -pthread -Wall -g -c par_run.c

monitor.o: monitor.c monitor.h list.h
	gcc -pthread -Wall -g -c monitor.c

list.o: list.c list.h
	gcc -pthread -Wall -g -c list.c
	 
clean:
	rm *.o

fibonacci: fibonacci.c
	gcc fibonacci.c -o fibonacci

remove: 
	rm par-shell fibonacci

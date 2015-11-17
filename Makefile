# Makefile

par-shell: main.o par_wait.o list.o 
	gcc -pthread -g -o par-shell par_run.o main.o par_wait.o list.o 

main.o: main.c par_wait.h list.h
	gcc -pthread -Wall -g -c main.c

par_wait.o: par_wait.c par_wait.h list.h
	gcc -pthread -Wall -g -c par_wait.c

list.o: list.c list.h
	gcc -pthread -Wall -g -c list.c

clean:
	rm *.o

fibonacci: fibonacci.c
	gcc fibonacci.c -o fibonacci

remove: 
	rm par-shell fibonacci

test: par-test.sh fibonacci par-shell
	./par-test.sh

delete:
	rm fibonacci par-shell

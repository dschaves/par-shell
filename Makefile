# Makefile for par-shell.

CCFLAGS=-pthread -Wall -g
# Flags detailed:
# pthread: enable multithreading
# Wall: All warnings.
# g: enable debugging symbols

par-shell: list.o par_sync.o par_wait.o main.o 
	$(CC) $(CCFLAGS) -o par-shell list.o par_sync.o par_wait.o main.o

main.o: main.c par_sync.h list.h
	$(CC) $(CCFLAGS) -c -o main.o main.c

par_sync.o: par_sync.c par_wait.h par_sync.h list.h
	$(CC) $(CCFLAGS) -c -o par_sync.o par_sync.c

par_wait.o: par_wait.c par_wait.h par_sync.h
	$(CC) $(CCFLAGS) -c -o par_wait.o par_wait.c

list.o: list.c list.h
	$(CC) $(CCFLAGS) -c list.c

clean:
	rm *.o

fibonacci: fibonacci.c
	$(CC) fibonacci.c -o fibonacci

remove: 
	rm par-shell fibonacci

test: par-test.sh fibonacci par-shell
	./par-test.sh

delete:
	rm fibonacci par-shell

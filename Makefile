# Makefile for par-shell.

CFLAGS=-pthread -Wall -Werror -g
# Flags detailed:
# pthread: enable multithreading
# Wall: All warnings.
# Werror: Warnings become errors.
# g: enable debugging symbols

all: par-shell par-shell-terminal

par-shell-terminal: par-shell-terminal.o par_sync.o
	$(CC) $(CFLAGS) -o par-shell-terminal par-shell-terminal.o par_sync.o

par-shell-terminal.o: par-shell-terminal.c 
	$(CC) $(CFLAGS) -c -o par-shell-terminal.o par-shell-terminal.c

par-shell: list.o remotes.o par_sync.o par_wait.o main.o 
	$(CC) $(CFLAGS) -o par-shell list.o remotes.o par_sync.o par_wait.o main.o

main.o: main.c par_sync.h list.h remotes.h
	$(CC) $(CFLAGS) -c -o main.o main.c

par_sync.o: par_sync.c par_wait.h par_sync.h list.h
	$(CC) $(CFLAGS) -c -o par_sync.o par_sync.c

par_wait.o: par_wait.c par_wait.h par_sync.h
	$(CC) $(CFLAGS) -c -o par_wait.o par_wait.c

remotes.o: remotes.c remotes.h par_wait.h
	$(CC) $(CFLAGS) -c -o remotes.o remotes.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

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

# Makefile

main: commandlinereader.o par_shell.o main.o
	gcc -o main commandlinereader.o par_shell.o main.o

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

parshell.o: par_shell.c commandlinereader.h par_shell.h
	gcc -Wall -g -c par_shell.c

main.o: main.c commandlinereader.h par_shell.h
	gcc -Wall -g -c main.c

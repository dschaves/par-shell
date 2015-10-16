# Makefile

par-shell: commandlinereader.o par_run.o main.o task_monitor.o
	gcc -pthread -o par-shell commandlinereader.o par_run.o main.o task_monitor.o

main.o: main.c main.h commandlinereader.h par_run.h task_monitor.h
	gcc -pthread -Wall -g -c main.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -pthread -Wall -g -c commandlinereader.c

par_run.o: par_run.c commandlinereader.h par_run.h
	gcc -pthread -Wall -g -c par_run.c

task_monitor.o: task_monitor.c task_monitor.h
	gcc -pthread -Wall -g -c task_monitor.c
	 
clean:
	rm -i *.o

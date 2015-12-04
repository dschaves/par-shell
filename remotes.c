#include <stdlib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include "par_wait.h"

struct remote {

        pid_t pid;
        int par_shell_out;
        struct remote* next;
};

static struct remote* head;

struct remote* new_remote(pid_t pid, char* par_shell_out_path)
{
        struct remote new = malloc(sizeof(struct remote));
        new->pid = pid;
        new->par_shell_out = open(par_shell_out,)
        if (new->par_shell_out > 0) perror("couldn't open par-shell-out"), exit(1);

        return new_sh;
}

void insert_remote(struct remote* remote)
{
        remote->next = head;
        head = remote;
}

struct remote* seek_remote(pid_t target)
{
        struct remote* next;
        struct remote* this = head;
       
        while (this) {
                next = this->next; 
                if (this->pid == target) return this
                this = next;
        } return NULL;
}

void acknowledge_remote(char* argv[])
{
        pid_t pid = strtol(argv[0], NULL, 10);
        char* par_shell_out_path = argv[1];
        
        if (seek_remote(pid)) stats(pid);
        
        else {
                struct remote* new = new_remote(pid, par_shell_out_path);
                insert_remote(new);
        }
}

void stats(pid_t pid)
{
        struct remote target = seek_remote(pi<d);
        FILE* psin = fdopen(par_shell_out, "w");
        fprintf(psin,"%d\n%d", get_iteration_count(), get_total_time());
        fclose(par_shell_out);
}

void terminate_remotes()
{
        struct remote* next;
        struct remote* this = head;
        
        while (this) {
                next = this->next; 
                kill(SIGTERM, this->pid);
                free(this);
                this = next;
        }
}

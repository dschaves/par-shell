#include <stdlib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "par_wait.h"

struct remote {

        pid_t pid;
        char* out_path;
        struct remote* next;
};

static struct remote* head;

struct remote* new_remote(pid_t pid, char* out_path)
{
        struct remote* new = malloc(sizeof(struct remote));
        new->pid = pid;
        new->out_path = strdup(out_path);

        return new;
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
                if (this->pid == target) return this;
                else this = next;
        } return NULL;
}

void stats(pid_t pid)
{
        struct remote* target = seek_remote(pid);
        FILE* out = fopen(target->out_path, "w");
        fprintf(out,"%d\n%d", get_iteration_count(), (int) get_total_time());
        fclose(out);
}


void acknowledge_remote(char* argv[])
{
        pid_t pid = strtol(argv[0], NULL, 10);
        char* out_path = argv[1];
        
        if (seek_remote(pid)) stats(pid);
        
        else {
                struct remote* new = new_remote(pid, out_path);
                insert_remote(new);
        }
}

void terminate_remotes()
{
        struct remote* next;
        struct remote* this = head;
        
        while (this) {
                next = this->next; 
                kill(SIGTERM, this->pid);
                free(this->out_path);
                free(this);
                this = next;
        }
}

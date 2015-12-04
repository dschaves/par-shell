#include <stdlib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

struct remote {

        pid_t pid;
        int par_shell_out;
        struct remote* next;
};

static struct remote* head;

void acknowledge_remote(char* argv[])
{
        (pid_t) pid = strtol(argv[0], NULL, 10);
        char* par_shell_out_path = argv[1];
        
        if (known_remote(pid)) stats(pid);
        
        else {
                struct remote* new = new_remote(pid, par_shell_out_path);
                insert_remote(new);
        }
}

struct remote* new_remote(pid_t pid, char* par_shell_out_path)
{
        struct remote_shell new = malloc(sizeof(struct remote_shell));
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

bool known_remote(pid_t target)
{
        struct remote* next;
        struct remote* this = head;
       
        while (this) {
                next = this->next; 
                if (this->pid == target) return true;
                this = next;
        } return false;
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

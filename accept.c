void accept_connections(char* accept_fifo_fp)
{
        char output_pipe_path[256];
        char input_pipe_path[256] = /tmp/par-shell-in
        FILE* output_pipe;
        FILE* input_pipe;
        
        /* Listen on pipe. When a client writes into it, get a line from it.
         * According to the current protocol this line should be the client's
         * output fifo. */
        while (!exit_called()) {
        
                /*Remain blocked while no open is done on the other side. */
                FILE* accept_pipe = fopen(accept_pipe_fp, "r"); 
                
                /* Reads until a newline is found. Error if NULL.*/
                if(!fgets(output_pipe_path, 256, accept_pipe))
                        perror("Couldn't read from accept FIFO.");
                        
                FILE* output_pipe = fopen(output_pipe_path);
                FILE* input_pipe = fdopen(mkfifo())
        }
        
}

struct remote_shell {

        pid_t pid;      
        FILE* output_pipe;
};

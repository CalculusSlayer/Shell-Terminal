#include <stdio.h>
#include <stdlib.h>

#include "sshell.h"

void sshell_system_pipe(Process *processes, int num_processes) 
{
    int pipe_FDs[(num_processes - 1) * 2];
    int count = 0;
    pid_t pid;


    /* Create all necessary pipes */
    for (int pipe_number = 0; pipe_number < num_processes - 1; pipe_number++) {
        if (pipe(pipe_FDs + pipe_number * 2) < 0) {
           printf(stderr, "sshell_system_pipe failed: pipe()\n");
           exit(1);
        }
    }
    

    while (process_count < num_processes) {
        pid = fork();
        if (pid == 0) {
            /* Child  process */

        }    
        else if (pid > 0) {
            /* Parent process */
        }
        else {
            /* Fork failure */
            printf(stderr, "sshell_system_pipe failed: fork()\n");
            exit(1);
        } 
        
    }


}

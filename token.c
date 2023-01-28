#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "sshell.h"
#include "token.h"
#include "linked_list.h"

Process pipes[4];

Process new_process() {
    Process p = malloc(sizeof(ProcessObj));
    p->program = NULL;
    p->FO_type = NO_FILE_OPERATOR;
    //p->background_job = false;
    p->left_args = NULL;
    p->file_name = NULL;
    //p->exit_status = 70;
    //p->cmd_str = strdup("bogus");

    return p;
}

void free_process(Process* p) {
    if (p != NULL && *p != NULL) {
        freeLinkedList(&((*p)->left_args));
        freeLinkedList(&((*p)->file_name));
        if ((*p)->program) {
            free((*p)->program);
            (*p)->program = NULL;
        }
        free(*p);
        p = NULL;
    }
}

StringArray new_StringArray(char** s, int length) {
    StringArray SA = malloc(sizeof(StringArrayObj));
    SA->arr = s; // char** s is already allocated elsewhere
               // right before creating this object
    SA->length = length;
    return SA;
}



// echo Hello world | grep Hello | wc -l
// char **x = {"echo Hello world", "grep Hello", "wc -l", NULL}

StringArray split_pipes(char *cmd_line) {
    char **pipe_array = calloc(4, sizeof(char*));
    char *cmd_line_copy = strdup(cmd_line);
    char *token = NULL;
    const char delimiter[] = "|";

    token = strtok(cmd_line_copy, delimiter);
    int pipe_array_index = 0;
    do {
        //printf("%d, %s\n", pipe_array_index, token);
        pipe_array[pipe_array_index] = strdup(token);
        token = strtok(NULL, delimiter);
        pipe_array_index++;
    } while (token);
    
    free(cmd_line_copy);
    return new_StringArray(pipe_array, 4);

}

Process split_redirection(char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0) {
        return NULL;
    }
    
    Process p = new_process();
    //char *cmd_copy = strdup(cmd);    
    // Need to make copy of buf
    char *needle = NULL;
    //const char delimiter[] = " ";
    if ((needle=strstr(cmd, ">>"))) {
        p->FO_type = APPEND_REDIRECTION;
        char *cmd_copy = strdup(cmd);
        cmd_copy[needle-cmd] = '\0';
        p->left_args = str_to_ll(cmd_copy);
        if (p->left_args && getLength(p->left_args) > 0) {
            p->program = strdup(front(p->left_args));
            popLeft(p->left_args);
        }
        else {
            p->program = strdup("NULL"); // p->program is char* type
            fprintf(stderr, "token.c: Expected argument(s) before >>");
        }
        
        //unsigned long int difference = needle - cmd; 
        char *file = needle + 2;
        if (*(file)) {
            p->file_name = str_to_ll(file);
            if (isEmpty(p->file_name)) {
                freeLinkedList(&(p->file_name));
                fprintf(stderr, "token.c: Expected file name after > >\n"); 
            }
            else if (getLength(p->file_name) > 1) {
                freeLinkedList(&(p->file_name));
                fprintf(stderr, "token.c: Expected only one argument after >>\n");
            }
        }

        else {
            fprintf(stderr, "token.c: Expected file name after >>\n");
        }
        free(cmd_copy);
    }

    else if ((needle=strstr(cmd, ">"))) {
        p->FO_type = TRUNCATE_REDIRECTION;
        char *cmd_copy = strdup(cmd);
        cmd_copy[needle-cmd] = '\0';
        p->left_args = str_to_ll(cmd_copy);
        if (p->left_args && getLength(p->left_args) > 0) {
            p->program = strdup(front(p->left_args));
            popLeft(p->left_args);
        }
        else {
            p->program = strdup("NULL"); // p->program is char* type
            fprintf(stderr, "token.c: Expected argument(s) before >");
        }
        
        //unsigned long int difference = needle - cmd; 
        char *file = needle + 1;
        if (*(file)) {
            p->file_name = str_to_ll(file);
            if (isEmpty(p->file_name)) {
                freeLinkedList(&(p->file_name));
                fprintf(stderr, "token.c: Expected file name after >\n"); 
            }
            else if (getLength(p->file_name) > 1) {
                freeLinkedList(&(p->file_name));
                fprintf(stderr, "token.c: Expected only one argument after >\n");
            }
        }

        else {
            fprintf(stderr, "token.c: Expected file name after >\n");
        }
        free(cmd_copy);
    }
    
    // No file direction operator
    // so just do a regular split
    else {
        p->left_args = str_to_ll(cmd);
        //printf("LOOK BELOW\n");
        //printLinkedList(stdout, p->left_args);
        if (p->left_args && getLength(p->left_args) > 0) {
            p->program = strdup(front(p->left_args));
            popLeft(p->left_args);
        }
        else {
            p->program = NULL;
        }
        //printf("In token.c: p->program = %s\n", p->program);
    }

    return p;

}


// Replaced char*** with StringArray
void deallocator(StringArray* SA) {
    
    if (SA == NULL || *SA == NULL) {
        return;
    }

    for (int c = 0; c < (*SA)->length; c++) {
        if (((*SA)->arr)[c]) {
            free(((*SA)->arr)[c]);
            ((*SA)->arr)[c] = NULL;
        }
    }
    
    free((*SA)->arr);
    (*SA)->arr = NULL;
    free(*SA);
    *SA = NULL;

    /*
    if (buf == NULL || *buf == NULL) {
        //fprintf(stderr, "character buffer is NULL\n");
        return;
    }

    for (char **ptr = *buf; *ptr; ptr++) {
        free(*ptr);
        *ptr = NULL;
    }
    free(*buf);
    *buf = NULL;
    */
}

int* sshell_system_pipe(Process *processes, int num_processes) 
{
    //printf("In function sshell_system_pipe: \n");
    int num_pipes = num_processes - 1;
    int pipe_FDs[num_pipes * 2];
    //int process_count = 0;
    int status;
    pid_t pid;

    static int pipe_exit_status[4];
    
    /* Linked List of program name strings */
    LinkedList commands = newLinkedList();
    for (int nodes = 0; nodes < num_processes; nodes++)
    {
        appendRight(commands, processes[nodes]->program);
        processes[nodes]->process_index = nodes;
    }
    
    int *process_count1 = malloc(sizeof(int));
    *process_count1 = 0;

    /* Create all necessary pipes (n - 1) */
    for (int pipe_number = 0; pipe_number < num_pipes; pipe_number++) {
        if (pipe(pipe_FDs + pipe_number * 2) < 0) {
           perror("sshell_system_pipe() failed: pipe()\n");
           exit(1);
        }
    }
    

    while (getLength(commands) > 0) {

        pid = fork();
        processes[num_processes - getLength(commands)]->process_id 
            = pid;
        if (pid == 0) {
            //printf("%d\n", *process_count1);
            /* Child  process */
            // Piping process taken from lecture slides 3 : syscalls

            // If not the first process, replace stdin with pipe read port
            if (*process_count1 != 0) {
                if (dup2(pipe_FDs[*process_count1 * 2 - 2], 0) < 0) {
                    perror("sshell_system_pipe() failed 1: dup2");
                    exit(1);
                }
            }
            // If not the last process, replace stdout with pipe write port
            if (*process_count1 != num_pipes) {
                if (dup2(pipe_FDs[*process_count1 * 2 + 1], 1) < 0) {
                    perror("sshell_system_pipe() failed 2: dup2");
                    exit(1);
                }
            }

            // Close now unused pipes
            for (int pipe_port = 0; pipe_port < num_pipes * 2; pipe_port++) {
                close(pipe_FDs[pipe_port]);
            }

            // Execute commands given by process objects
            char *prog_name = front(commands);
            char **execvp_arguments = ll_to_str_arr(processes[*process_count1]->left_args, prog_name);
            if (execvp(prog_name, execvp_arguments) < 0) {
                    perror("sshell_system_pipe() failed: execvp");
                    exit(1);
            }

        }    
        else if (pid < 0) {
            /* Fork failure */
            perror("sshell_system_pipe failed: fork()\n");
            exit(1);
        }  
        /* 
            Parent creates all child processes through fork(), 
            closes all of its file descriptors because it doesn't 
            use them, and then waits for each of the children to exit
        */

        popLeft(commands);
        (*process_count1)++;
        
    }
    for (int i = 0; i < num_pipes * 2; i++) {
        close(pipe_FDs[i]);
    }
   
    if (num_processes > 2) {
        for (int exit_status_index = 0; exit_status_index < num_processes; exit_status_index++) {
            //wait(&status);
            wait(&status);
            pipe_exit_status[exit_status_index] = WEXITSTATUS(status);
        }
    }

    /*
    if (num_processes == 4) {
        while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)==0);
        if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[3] = WEXITSTATUS(status);

            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[2]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[0] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[1] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[0] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[0] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[2] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[0] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[0] = WEXITSTATUS(status);
                }
            }
        }

        else if (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[0] = WEXITSTATUS(status);

            while (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[2]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[1] = WEXITSTATUS(status);
                while (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[3] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[3] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[2] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[3] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[3] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[3] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
            }
        }

        else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[1] = WEXITSTATUS(status);

            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[3]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[0] = WEXITSTATUS(status);
                while (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[3] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[3] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[2] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[0] = WEXITSTATUS(status);
                    while (waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[3] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[3] = WEXITSTATUS(status);
                    while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[0] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processe[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[3] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
            }
        }

        else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[2] = WEXITSTATUS(status);

            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[3]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[3] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[0] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[0] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processes[3]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[1] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[0] = WEXITSTATUS(status);
                    while (waitpid(processes[3]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[3] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[3] = WEXITSTATUS(status);
                    while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[0] = WEXITSTATUS(status);
                }
            }

            else if (waitpid(processe[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[3]->process_id, &status, WNOHANG)!=0) {

                pipe_exit_status[3] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                        waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[1] = WEXITSTATUS(status);
                    while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[2] = WEXITSTATUS(status);
                }
                else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                    pipe_exit_status[2] = WEXITSTATUS(status);
                    while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                    pipe_exit_status[1] = WEXITSTATUS(status);
                }
            }
        }
    }



    else if (num_processes == 3) {
        while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[2]->process_id, &status, WNOHANG)==0);
        if (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
            
            pipe_exit_status[0] = WEXITSTATUS(status);
            while (waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[2]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[1] = WEXITSTATUS(status);
                while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[2] = WEXITSTATUS(status);
            }
            else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[2] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[1] = WEXITSTATUS(status);
            }
        }
        
        else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {

            pipe_exit_status[1] = WEXITSTATUS(status);
            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[2]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[0] = WEXITSTATUS(status);
                while (waitpid(processes[2]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[2] = WEXITSTATUS(status);
            }
            else if (waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[2] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[0] = WEXITSTATUS(status);
            }
        }

        else if (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[1]->process_id, &status, WNOHANG)==0 &&
            waitpid(processes[2]->process_id, &status, WNOHANG)!=0) {

            pipe_exit_status[2] = WEXITSTATUS(status);
            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                    waitpid(processes[1]->process_id, &status, WNOHANG)==0);
            if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[0] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[1] = WEXITSTATUS(status);
            }
            else if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[1] = WEXITSTATUS(status);
                while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
                pipe_exit_status[0] = WEXITSTATUS(status);
            }
        }

    }
    */

    else if (num_processes == 2) {
        while (waitpid(processes[0]->process_id, &status, WNOHANG)==0 &&
                waitpid(processes[1]->process_id, &status, WNOHANG)==0);
        if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[0] = WEXITSTATUS(status);
            while (waitpid(processes[1]->process_id, &status, WNOHANG)==0);
            pipe_exit_status[1] = WEXITSTATUS(status);
        }
        
        else if (waitpid(processes[1]->process_id, &status, WNOHANG)!=0) {
            pipe_exit_status[1] = WEXITSTATUS(status);
            while (waitpid(processes[0]->process_id, &status, WNOHANG)==0);
            pipe_exit_status[0] = WEXITSTATUS(status);
        }
    }
       /*
        {
            if (waitpid(processes[0]->process_id, &status, WNOHANG)!=0) {
                pipe_exit_status[0] = WEXITSTATUS(status);
                while (waitpid(processes[1]->process_id, &status, WNOHANG)==0) {
                    pipe
            }
        }
        */

    freeLinkedList(&commands);
    return pipe_exit_status;

}


int sshell_system(Process p, bool background_job_flag, List background_jobs, char* cmd_msg) {
    // If NULL process is passed, return None
    if (!p) {
        return 0;
    }

    pid_t child_pid;
    child_pid = fork();

    //char** cmd_args = ll_to_str_arr(p->left_args);
    if (child_pid == 0) {
        if (p->FO_type == TRUNCATE_REDIRECTION
                || p->FO_type == APPEND_REDIRECTION) {
            //printf("This is the p->file_name: ");
            //printLinkedList(stdout, p->file_name);
            //printf("\n");
            
            unsigned int FILE_OPEN_MODE = 0;
            char* arrow = NULL;
            if (p->FO_type == TRUNCATE_REDIRECTION) {
                FILE_OPEN_MODE = O_TRUNC;
                arrow = ">";
            }
            else {
                FILE_OPEN_MODE = O_APPEND;
                arrow = ">>";
            }

            if (p->file_name && getLength(p->file_name)==1
                    && p->left_args && getLength(p->left_args)>0) {
                // TODO: CHANGE TO O_RDWR if needed
                // FILE_OPEN_MODE is either O_TRUNC or O_APPEND
                int f1 = open(front(p->file_name), O_CREAT|FILE_OPEN_MODE|O_WRONLY, 0644); 
                dup2(f1, STDOUT_FILENO);
                close(f1);
            }

            else if (!(p->program)) {
                fprintf(stderr, "sshell_system(), token.c: There is no argument(s) before %s\n",
                        arrow);
            }
            else if (!(p->file_name)) {
                fprintf(stderr, "sshell_system(), sshell.c: There is no argument after %s\n",
                        arrow);
            }
            else if (getLength(p->file_name) > 1) {
                fprintf(stderr, "sshell.c: There are too many arguments after %s\n", arrow);
            }
        }
        //char **cmd_args = splitter(cmd);
        //char *cmd_args[] = {cmd, "-l", NULL};
        StringArray cmd_args = new_StringArray(
                ll_to_str_arr(p->left_args, p->program), getLength(p->left_args)+2);
        execvp(p->program, cmd_args->arr);
        //perror("execv");
        fprintf(stderr, "Error: command not found\n");
        deallocator(&cmd_args);
        exit(1);
    }

    else if (child_pid > 0) {
        int child_status;
        if (background_job_flag) {
            BackgroundJobObj* x = malloc(sizeof(BackgroundJobObj));
            x->cmd_str = strdup(cmd_msg);
            x->exit_status = 10;
            x->pid = child_pid;
            append(background_jobs, x);
            //printf("cmd_str = %s\n", bg->cmd_str);
            //printf("p inside sshell_system() = %lu\n", (unsigned long) bg);
            //printf("background_job = %d\n", background_job);
            waitpid(child_pid, &(x->exit_status), WNOHANG);

        }
        else {
            waitpid(child_pid, &child_status, 0);
            //fprintf(stderr, "+ completed '%s' [%d]\n", cmd_msg, WEXITSTATUS(child_status));
            return WEXITSTATUS(child_status);
        }
        // Printing to stderr instead of stdout now. Instructions
        // said to print to stderr.
        //fprintf(stderr, "+ completed '%s' [%d]\n", cmd,  WEXITSTATUS(child_status));
        /*
        fprintf(stderr, "+ completed '%s", p->program);
        if (p->left_args && getLength(p->left_args) > 0) {
            fprintf(stderr, " ");
            printLinkedList(stderr, p->left_args);
        }

        // TODO: add if statement for printing out 
        // file director to stderr

        fprintf(stderr, "' [%d]\n", WEXITSTATUS(child_status));
        //deallocator(&cmd_args);
        */

        //TODO: DECIDE WHETHER TO FREE PROCESS IN MAIN OR HERE!!!
        //free_process(&p);
        
        //return WEXITSTATUS(child_status);
        return 0; // TODO: check here
    }

    else {
        perror("fork");
        exit(1);
    }
}

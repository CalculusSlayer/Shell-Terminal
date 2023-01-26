#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "sshell.h"
#include "token.h"
#include "linked_list.h"

const int MAX_ARGUMENTS = 16;
Process pipes[4];

Process new_process() {
    Process p = malloc(sizeof(ProcessObj));
    p->program = NULL;
    p->FO_type = NO_FILE_OPERATOR;
    //p->background_job = false;
    p->left_args = NULL;
    p->file_name = NULL;

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
        p->program = strdup(front(p->left_args));
        //printf("In token.c: p->program = %s\n", p->program);
        popLeft(p->left_args);
    }

    return p;

}

/*
char **splitter(char *buf) {
    char **cmd_args = calloc(MAX_ARGUMENTS, sizeof(char*));
    char *buf_copy = strdup(buf);    
    // Need to make copy of buf
    char *token = NULL;
    const char delimiter[] = " ";

    token = strtok(buf_copy, delimiter);
*/

    /*
    while (token != NULL) {
        printf("%s\n", token);
        test[token]
        token = strtok(NULL, delimiter);
    }
    */

/*
    for (int cmd_args_index=0; token != NULL;
            cmd_args_index++) {
        //printf("%s\n", token);
        //If more than 16 valid 
        if (cmd_args_index == MAX_ARGUMENTS) {
            free(buf_copy);
            return NULL;
        }
        cmd_args[cmd_args_index] = strdup(token);
        token = strtok(0, delimiter);
    }
    free(buf_copy);
    return cmd_args;
}
*/

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

int sshell_system(Process p) {
    // If NULL process is passed, return 0
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

            else if (!(p->left_args) || getLength(p->left_args)==0) {
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
        perror("execv");
        deallocator(&cmd_args);
        exit(1);
    }

    else if (child_pid > 0) {
        int child_status;
        waitpid(child_pid, &child_status, 0);
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
        return WEXITSTATUS(child_status);
    }

    else {
        perror("fork");
        exit(1);
    }
}

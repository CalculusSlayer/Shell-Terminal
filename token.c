#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "token.h"
#include "linked_list.h"

const int MAX_ARGUMENTS = 16;
Process pipes[4];

/*
int sshell_exec(Process *p) {
    return 0;
}
*/
Process new_process() {
    Process p = malloc(sizeof(ProcessObj));
    p->program = NULL;
    p->FO_type = NO_FILE_OPERATOR;
    p->background_job = false;
    p->left_args = NULL;
    p->right_args = NULL;

    return p;
}

void free_process(Process* p) {
    if (p != NULL and *p != NULL) {
        freeLinkedList((*p)->left_args);
        freeLinkedList((*p)->right_args);
        free((*p)->program);
        free(*p);
    }
}

char** split_pipes(char *cmd_line) {
    char **pipe_array = calloc(4, sizeof(char*));
    char *cmd_line_copy = strdup(cmd_line);
    char *token = NULL;
    const char delimiter[] = "|";

    token = strtok(cmd_line_copy, delimiter);
    int pipe_array_index = 0;
    do {
        printf("%d, %s\n", pipe_array_index, token);
        pipe_array[pipe_array_index] = strdup(token);
        token = strtok(NULL, delimiter);
        pipe_array_index++;
    } while (token);
    
    free(cmd_line_copy);
    return pipe_array;

}

Process* split_redirection(char *cmd) {
    
    Process p = new_process();
    char *cmd_copy = strdup(cmd);    
    // Need to make copy of buf
    char *token = NULL;
    //const char delimiter[] = " ";


    if (strstr(cmd_copy, ">")) {
        token = strtok(cmd_copy, ">");
        p->FO_type = TRUNCATE_REDIRECTION;
        if (token) {
            // convert token to Linked list
            // p->program = head(linked list)
            p->left_args = splitter(token);
        }
        token = strtok(cmd_copy, ">");
        if (token) {
            // convert token to linked list
            p->right_args = splitter(token);
        }
    }
    else if (strstr(cmd_copy, ">>")) {
        token = strtok(cmd_copy, ">>");
        p->FO_type = APPEND_REDIRECTION;
        if (token)
            p->left_args = splitter(token);
        token = strtok(cmd_copy, ">>");
        if (token)
            p->right_args = splitter(token);
    }
    else {
        p->left_args = splitter(cmd_copy);
        p->right_args = NULL;
        p->FO_type = NO_FILE_OPERATOR;
    }

    if (!token) {
        //fprintf(stderr, "

    while (token != NULL) {
        printf("%s\n", token);
        test[token]
        token = strtok(NULL, delimiter);
    }

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
    free(cmd_copy);
    return cmd_args;

}

/*
// str_to_LL() 
// Convert str to LinkedList and return it
LinkedList str_to_LL(char *str) {
    char *str_copy = strdup(str);    
    // Need to make copy of buf
    char *token = NULL;
    const char delimiter[] = " ";
    LinkedList L = newLinkedList();

    token = strtok(buf_copy, delimiter);

    while (token != NULL) {
        //printf("%s\n", token);
        appendRight(L, token);
        token = strtok(NULL, delimiter);
    }

    free(str_cpy);
    //printLinkedList(L);
    return L;

}
*/

char **splitter(char *buf) {
    char **cmd_args = calloc(MAX_ARGUMENTS, sizeof(char*));
    char *buf_copy = strdup(buf);    
    // Need to make copy of buf
    char *token = NULL;
    const char delimiter[] = " ";

    token = strtok(buf_copy, delimiter);

    /*
    while (token != NULL) {
        printf("%s\n", token);
        test[token]
        token = strtok(NULL, delimiter);
    }
    */

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

void deallocator(char ***buf) {
    if (buf == NULL || *buf == NULL) {
        fprintf(stderr, "character buffer is NULL\n");
        return;
    }

    for (char **ptr = *buf; *ptr; ptr++) {
        free(*ptr);
        *ptr = NULL;
    }
    free(*buf);
    *buf = NULL;
}


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const int MAX_ARGUMENTS = 16;

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


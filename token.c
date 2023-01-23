#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char **splitter(char *buf) {
    char **cmd_args = malloc(sizeof(char*) * 16);

    char *token = NULL;
    const char delimiter[] = " ";

    token = strtok(buf, delimiter);

    /*
    while (token != NULL) {
        printf("%s\n", token);
        test[token]
        token = strtok(NULL, delimiter);
    }
    */

    for (int cmd_args_index=0; token != NULL;
            cmd_args_index++) {
        printf("%s\n", token);
        cmd_args[cmd_args_index] = strdup(token);
        token = strtok(NULL, delimiter);
    }

    return cmd_args;
}

void deallocator(char ***buf) {
    for (char **ptr = *buf; *ptr; ptr++) {
        free(*ptr);
        *ptr = NULL;
    }
    free(*buf);
    *buf = NULL;
}


int main() {
    /*
    char **cmd_args = malloc(sizeof(char*) * 16);

    char str[] = "ls -l -x -x -f";
    char *token;
    const char delimiter[] = " ";

    token = strtok(str, delimiter);

    while (token != NULL) {
        printf("%s\n", token);
        test[token]
        token = strtok(NULL, delimiter);
    }

    for (int cmd_args_index=0; token != NULL;
            cmd_args_index++) {
        //printf("%s\n", token);
        cmd_args[cmd_args_index] = strdup(token);]
        token = strtok(NULL, delimiter);
    }
    */

    char str[] = "ls -l -x -x -f";
    char **x = splitter(str);
    // char * ptr = *x
    char **ptr = x;
    for (int i = 0; i < 5; i++,  ptr++) {
        printf("%s\n", *ptr);
    }

    deallocator(&x);

    return 0;
}

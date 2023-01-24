// token.h

#pragma once

char **splitter(char *buf);

void deallocator(char ***buf);

typedef struct {
    char *cmd;
    // change type to linked list
    char **args1;
    char **args2;
    // add file operation type
} Process;


// token.h

#pragma once

char **splitter(char *buf);

void deallocator(char ***buf);

typedef struct {
    char *cmd;
    char **args;
} Process;


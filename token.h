// token.h

#pragma once

char **splitter(char *buf);

void deallocator(char ***buf);

typedef struct {
    char *cmd = NULL;
    char **args = NULL;
} Process;


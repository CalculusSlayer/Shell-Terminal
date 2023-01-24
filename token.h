// token.h

#pragma once

#include "linked_list.h"

// TODO: Need to change parsing functions
char **splitter(char *buf);

void deallocator(char ***buf);

// split_pipes() 
// splits cmd line into a max of 4 string (3 pipes)
// and return char** object containing them.
char** split_pipes(char *cmd_line);

typedef enum File_Operator {
    NO_FILE_OPERATOR = 0,
    TRUNCATE_REDIRECTION,
    APPEND_REDIRECTION = 2
} File_Operator;

typedef struct Process {
    char *program;
    File_Operator FO_type;
    bool background_job;
    // change type to linked list
    LinkedList left_args;
    LinkedList right_args;
} Process;


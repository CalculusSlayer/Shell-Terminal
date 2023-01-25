// token.h

#pragma once

#include "linked_list.h"

typedef enum File_Operator {
    NO_FILE_OPERATOR = 0,
    TRUNCATE_REDIRECTION = 1,
    APPEND_REDIRECTION = 2
} File_Operator;

typedef struct ProcessObj* Process;

typedef struct ProcessObj {
    char *program;
    File_Operator FO_type;
    //bool background_job;
    // change type to linked list
    LinkedList left_args;
    LinkedList right_args;
} ProcessObj;

// new_process()
// Create reference to new ProcessObj
Process new_process();

// free_process()
// Free all memory for ProcessObj reference
void free_process(Process* p);


// TODO: Need to change parsing functions
char **splitter(char *buf);

void deallocator(char ***buf);

// split_pipes() 
// splits cmd line into a max of 4 string (3 pipes)
// and return char** object containing them.
char** split_pipes(char *cmd_line);

Process split_redirection(char* cmd);

void sshell_system(Process); 


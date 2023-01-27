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
    LinkedList file_name;
} ProcessObj;

// StringArray type is a pointer to StringArrayObj
// It will be used for storing char** with
// their lengths to make freeing memory easier.
typedef struct StringArrayObj* StringArray;

// StringArrayObj is a type that contains
// a char** parameter which represents the array
// and a int parameter which represents the length.
typedef struct StringArrayObj {
    char** arr;
    int length;
} StringArrayObj;

// new_process()
// Create reference to new ProcessObj
Process new_process();

// free_process()
// Free all memory for ProcessObj reference
void free_process(Process* p);


// TODO: Need to change parsing functions
//char **splitter(char *buf);

//void deallocator(char ***buf);
// Replace with this below
void deallocator(StringArray* buf);

// split_pipes() 
// splits cmd line into a max of 4 string (3 pipes)
// and return char** object containing them.
StringArray split_pipes(char *cmd_line);

Process split_redirection(char* cmd);

int* sshell_system_pipe(Process *processes, int num_processes);

int sshell_system(Process p, bool background_job); 


// token_test.c
// Test different helper
// functions used in
// sshell.c

#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int main() {

    char str[] = "ls      -l     -x               -x -f";
    //char str2[] = "echo Hello world|grep Hello|wc -l";
    char str2[] = "echo hello world > a.txt";

    char **x = splitter(str);
    char **pipe_strings = split_pipes(str2);
    // char * ptr = *x
    //char **ptr = x;
    /*
    for (int i = 0; i < 5; i++,  ptr++) {
        printf("%s*", *ptr);
    }
    */

    deallocator(&x);
    deallocator(NULL);
    

    // TODO: INITIALIZE PROCESS ARRAY OF SIZE 4
    // IN sshell.c
    Process yy[4] = {NULL, NULL, NULL, NULL};
    for (int i = 0; i < 4; i++) {
        /*
        if (pipe_strings[i] == NULL) {
            printf("\nPipe %i is NULL\n", i);
        }
        */
        printf("Pipe %i: %s\n", i, pipe_strings[i]);
        yy[i] = split_redirection(pipe_strings[i]);
    }

    //yy[0] = split_redirection(pipe_strings[0]);
 
    for (int i = 0; i < 4; i++) {
        if (yy[i] != NULL) {
            printf("program - %s, FO_TYPE - %d\n", yy[i]->program, yy[i]->FO_type);
            printf("left_args = ");
            if (yy[i]->left_args) {
                printLinkedList(stdout, yy[i]->left_args);
            }
            printf("file_name = ");
            if (yy[i]->file_name) {
                printLinkedList(stdout, yy[i]->file_name);
            }
        }
    }
    
    for (int i = 0; i < 4; i++) {
        sshell_system(yy[i]);
    }


    deallocator(&pipe_strings);

    /*
    LinkedList b1 = str_to_ll("      ");
    printf("printing linked list b1\n");
    printLinkedList(stdout, b1);
    freeLinkedList(&b1);
    */

    return 0;
}

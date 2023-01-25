// token_test.c
// Test different helper
// functions used in
// sshell.c

#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int main() {

    char str[] = "ls      -l     -x               -x -f";
    char str2[] = "echo Hello world|grep Hello|wc -l";

    char **x = splitter(str);
    char **pipe_strings = split_pipes(str2);
    // char * ptr = *x
    char **ptr = x;
    for (int i = 0; i < 5; i++,  ptr++) {
        printf("%s*", *ptr);
    }

    deallocator(&x);
    deallocator(NULL);
    
    
    Process yy[4] = {NULL, NULL, NULL, NULL};
    for (int i = 0; i < 4; i++) {
        printf("Pipe %i: %s\n", i, pipe_strings[i]);
        //yy[i] = split_redirection(pipe_strings[i]);
    }

    yy[0] = split_redirection(pipe_strings[0]);

    
    printf("program - %s, FO_TYPE - %d\n", yy[0]->program, yy[0]->FO_type);
    printLinkedList(stdout, yy[0]->left_args);
    printLinkedList(stdout, yy[0]->right_args);
    deallocator(&pipe_strings);

    /*
    LinkedList b1 = str_to_ll("      ");
    printf("printing linked list b1\n");
    printLinkedList(stdout, b1);
    freeLinkedList(&b1);
    */

    return 0;
}

// token_test.c
// Test different helper
// functions used in
// sshell.c

#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int main() {

    char str[] = "ls      -l     -x               -x -f";
    char str2[] = "echo Hello world | grep Hello|wc -l";

    char **x = splitter(str);
    char **pipe_strings = split_pipes(str2);
    // char * ptr = *x
    char **ptr = x;
    for (int i = 0; i < 5; i++,  ptr++) {
        printf("%s*", *ptr);
    }

    deallocator(&x);
    deallocator(NULL);
    

    for (int i = 0; i < 4; i++) {
        printf("Pipe %i: %s\n", i, pipe_strings[i]);
    }
    deallocator(&pipe_strings);

    LinkedList b1 = str_to_ll("      ");
    printf("printing linked list b1\n");
    printLinkedList(stdout, b1);
    freeLinkedList(&b1);

    return 0;
}

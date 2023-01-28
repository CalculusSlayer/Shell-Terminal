#include <stdlib.h>
#include <stdio.h>

#include "generalList.h"
#include "sshell.h"


int print_process(Process p) {
    printf("program: %s", p->program);
    printf("FO_type: %d", p->FO_type);

    printf("left_args = ");
    printLinkedList(stdout, p->left_args);
    printf("\n");

    printf("file_name = ");
    printLinkedList(stdout, p->file_name);
    printf("\n");
}

int main(void) {
    
    List jobs = newList();
    
    Process p1 = new_process();
    p1->program = strdup("ls");

    Process p2 = new_process();
    p2->program = strdup("cat");

    Process p3 = new_process();
    p3->program = strdup("grep");

    append(jobs, p1);
    append(jobs, p2);
    append(jobs, p3);
    
    moveFront(jobs);
    for (int i=getIndex(jobs); i < length(jobs); i++, moveNext(jobs)) {
        print_process((Process)get(jobs));
        printf("\n");
    }

    moveFront(jobs);
    for (int i=getIndex(jobs); i < length(jobs); i++, moveNext(jobs)) {
        Process to_delete = get(jobs);
        if (to_delete != NULL)
            free_process(&to_delete);
    }
    freeList(&jobs);

    return 0;
}


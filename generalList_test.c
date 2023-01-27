#include <stdlib.h>
#include <stdio.h>

#include "generalList.h"
#include "sshell.h"

int main(void) {
    
    List jobs = newList();
    
    Process p1 = newProcess();
    p1->program = "ls";
    Process p2 = newProcess();
    p2->program = "cat";
    Process p3 = newProcess();
    p3->program = "grep";

    append(jobs, p1);
    append(jobs, p2);
    append(jobs, p3);
    
    moveFront(jobs);
    for (int i=getIndex(L); i < length(L); i++, moveNext(L)) {
        printf("prog%d name: %s\n", i+1, ((Process)get(L))->program);
    }

    moveFront(jobs);
    for (int i=getIndex(L); i < length(L); i++, moveNext(L)) {
        free_process(&((Process)get(L)));
    }
    freeList(&jobs);

    return 0;
}


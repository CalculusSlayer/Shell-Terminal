#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int main() {

    char str[] = "ls -l -x -x -f";
    char **x = splitter(str);
    // char * ptr = *x
    char **ptr = x;
    for (int i = 0; i < 5; i++,  ptr++) {
        printf("%s\n", *ptr);
    }

    deallocator(&x);
    deallocator(NULL);

    return 0;
}

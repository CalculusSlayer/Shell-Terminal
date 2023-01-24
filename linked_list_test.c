// linked_list_test.c
// - Tests the Linked List class

#include "linked_list.h"

int main(void) {
    char *string_list[] = {"banana", "cookie",
                            "dog", "egg plant",
                            "frog juice", "ham"};

    LinkedList L1 = newLinkedList();
    appendRight(L1, "apple");
    printLinkedList(stdout, L1);
    fprintf(stdout, "L1 length: %d\n",  getLength(L1));

    int string_list_length = sizeof(string_list)/
                                sizeof(string_list[0]);

    for (int i = 0; i < string_list_length; i++) {
        appendRight(L1, string_list[i]);
    }

    printLinkedList(stdout, L1);
    fprintf(stdout, "L1 length: %d\n",  getLength(L1));

    freeLinkedList(&L1);
    return 0;
}

// LinkedList.c
// Linked List of strings class
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

// Private Node struct
typdef struct *NodeObj Node;

// Private NodeObj struct
typedef struct {
    char* val;
    Node next;
} NodeObj;

// Private LinkedListObj struct
typedef struct {
    Node head;
    Node tail;
    int length;
} LinkedListObj;

// newNode()
// Returns a reference to new Node object
Node newNode(string val) {
    Node N = malloc(sizeof(NodeObj));
    if (!N) {
        fprintf(stderr, "Could not allocate
                memory for Node\n");
        exit(EXIT_FAILURE);
    }
    N->val = strdup(val);
    N->next = NULL;
    return N;
}

// freeNode()
// Frees Node object.
void freeNode(*Node pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// newLinkedList()
// Allocates memory for a new Linked List
// and creates a reference for it.
LinkedList newLinkedList() {
    LinkedList L = malloc(sizeof(LinkedListObj));
    if (!L) {
        fprintf(stderr, "Could not allocate
                memory for LinkedList\n");
        exit(EXIT_FAILURE);
    }
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    return L;
}

// freeLinkedList()
// Frees all memory allocated for Linked List
// including memory for the individual nodes.
void freeLinkedList(*pL) {
    if (pL != NULL && *pL != NULL) {
        while (!isEmpty(*pL)) {
            popLeft(*pL);
        }
        free(*pL);
        *pL = NULL;
    }
}


// getLength()
// Get length of the Linked List
int getLength(LinkedList L) {
    if (!L) {
        fprintf(stderr, "Calling getLength() on NULL reference\n");
        exit(EXIT_FAILURE);
    }

    return L->length;
}

// isEmpty()
// Returns true or false depending on
// whether Linked List is empty or not.
bool isEmpty(LinkedList L) {
    if (!L) {
        fprintf(stderr, "Calling isEmpty() on NULL reference\n");
        exit(EXIT_FAILURE);
    }

    return (getLength(L)==0);
}

// Append()
// Append string to Linked List
void Append(LinkedList L, string val) {
    if (!L) {
        fprintf(stderr, "Calling Append() on NULL reference\n");
        exit(EXIT_FAILURE);
    }
    Node N = newNode(val);

    if (isEmpty(L)) {
        L->head = L->tail = N;
    }
    else {
        L->tail->next = N;
        L->tail = N;
    }
    L->length++;
}

void popLeft(LinkedList L) {
    if (!L) {
        fprintf(stderr, "Calling popLeft() on NULL reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L)) {
        fprintf(stderr, "Linked List is empty. Cannot call popLeft()
                on an empty Linked List\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->head;
    if (getLength(L) > 1) {
        L->head = L->head->next;
    }
    else {
        L->head = L->tail = NULL;
    }

    L->length--;
    freeNode(&N);
        
}

void printLinkedList(LinkedList L, FILE *fileStream) {
    if (!L) {
        fprintf(stderr, "Calling printLinkedList() on NULL reference\n");
        exit(EXIT_FAILURE);
    }

    for (Node N = L->head; N != NULL; N->next) {
        fprintf(fileStream, "%s", N->val);
        if (N != L->tail)
            fprintf(fileStream, " ");
    }

    fprintf(fileStream, "\n");
}


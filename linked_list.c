// linked_list.c
// Linked List of strings class

#include "linked_list.h"

// Private Node struct
typedef struct NodeObj* Node;

// Private NodeObj struct
typedef struct NodeObj{
    char* val;
    Node next;
} NodeObj;

// Private LinkedListObj struct
typedef struct LinkedListObj{
    Node head;
    Node tail;
    int length;
    Node cursor;
} LinkedListObj;

// newNode() - private function
// Returns a reference to new Node object
Node newNode(char* val) {
    Node N = malloc(sizeof(NodeObj));
    if (!N) {
        fprintf(stderr, "Could not allocate "
                "memory for Node\n");
        exit(EXIT_FAILURE);
    }
    N->val = strdup(val);
    N->next = NULL;
    return N;
}

// freeNode() - private function
// Frees Node object.
void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free((*pN)->val);
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
        fprintf(stderr, "Could not allocate"
                "memory for LinkedList\n");
        exit(EXIT_FAILURE);
    }
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    L->cursor = NULL;
    return L;
}

// freeLinkedList()
// Frees all memory allocated for Linked List
// including memory for the individual nodes.
void freeLinkedList(LinkedList* pL) {
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

char* front(LinkedList L) {
    if (!L) {
        fprintf(stderr, "Cannot call front on a NULL reference\n");
        exit(EXIT_FAILURE);
    }
    if (getLength(L) > 0) {
        return L->head->val;
    }
    else {
        fprintf(stderr, "Cannot return front since length <= 0\n");
        freeLinkedList(&L); // not needed. check before calling
        exit(EXIT_FAILURE);
    }
}

char* back(LinkedList L) {
    if (!L) {
        fprintf(stderr, "Cannot call back on a NULL reference\n");
        exit(EXIT_FAILURE);
    }
    if (getLength(L) > 0) {
        return L->tail->val;
    }
    else {
        fprintf(stderr, "Cannot return back since length <= 0\n");
        freeLinkedList(&L); // not needed. check before calling
        exit(EXIT_FAILURE);
    }
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

// appendRight()
// Append string to end of Linked List
void appendRight(LinkedList L, char* val) {
    if (!L) {
        fprintf(stderr, "Calling appendRight() on NULL reference\n");
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
        fprintf(stderr, "Linked List is empty. Cannot call popLeft()"
                "on an empty Linked List\n");
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

char** ll_to_str_arr(LinkedList L, char* prog_name) {
    char **ret = malloc((getLength(L)+2) * sizeof(char*));
    int ret_index = 0;
    ret[ret_index] = prog_name; 
    ret_index++;
    for (Node N = L->head; N; N = N->next) {
        ret[ret_index] = strdup(N->val);
        ret_index++;
    }
    ret[ret_index] = NULL;
    return ret;
}

LinkedList str_to_ll(char* str) {
    if (!str || strlen(str)==0)
        return NULL;
    char *str_copy = strdup(str);
    // Need to make copy of buf
    char *token = NULL;
    const char delimiter[] = " ";
    LinkedList L = newLinkedList();

    token = strtok(str_copy, delimiter);

    while (token != NULL) {
        //printf("%s\n", token);
        appendRight(L, token);
        token = strtok(NULL, delimiter);
    }

    free(str_copy);
    //printLinkedList(L);
    return L;

}

void printLinkedList(FILE *fileStream, LinkedList L) {
    if (!L) {
        fprintf(stderr, "Calling printLinkedList() on NULL reference\n");
        exit(EXIT_FAILURE);
    }

    for (Node N = L->head; N != NULL; N = N->next) {
        fprintf(fileStream, "%s", N->val);
        if (N != L->tail)
            fprintf(fileStream, " ");
    }

    //fprintf(fileStream, "\n");
}


//--------------------------------------------------
// Nayeel Imtiaz
// Ky Ngo
//
// generalList.h -
// This is a header file
// for the representation
// of a general Linked List
// that can support all
// object types. We use it in
// our program as a double ended
// queue.
// _________________________________________________

#include "generalList.h"

// structs

// private gNode type
// This is a pointer to gNodeObj
typedef struct gNodeObj *gNode;

// private gNodeObj type
// This is a node structure
typedef struct gNodeObj {
    ListElement data; 
	gNode next;
	gNode prev;
} gNodeObj;

// private ListObj type
// This is a List structure
typedef struct ListObj {
	gNode front;
	gNode back;
	int length;
	int index;
	gNode cursor;
} ListObj;

// Constructors-Destructors
// Creates and returns new node.
gNode new_gNode(ListElement data) {
	gNode N = malloc(sizeof(gNodeObj));
	N->data = data;
	N->next = NULL;
	N->prev = NULL;

	return N;
}

// Free the node and set it to NULL.
// Pre: pointer not NULL
void free_gNode(gNode *pN) {
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

// Creates and returns new empty list.
List newList(void) {
	List L = malloc(sizeof(ListObj));
	L->front = L->back = NULL;
	L->length = 0;
	L->index = -1;
	L->cursor = NULL;
	return L;
}

// Frees the list and sets pl to NULL.
// Pre: Pointer not NULL
void freeList(List *pL) {
	clear(*pL);
	free(*pL);
	*pL = NULL;
}

// Access Functions
// Return the number of elements.
// Pre: Not NULL
int length(List L) {
	if (L) {
		return L->length;
	}

	else {
		fprintf(stderr, "Error: Called length on NULL reference.\n");
		exit(EXIT_FAILURE);
	}
}

// Return the index of the cursor of List
// Pre: Not NULL
int getIndex(List L) {
	if (L) {
		return L->index;
	}

	else {
		fprintf(stderr, "Error: called getIndex() on NULL reference.\n");
		exit(EXIT_FAILURE);
	}
}

// Return the value at the front of list
// Pre: not NULL
ListElement listFront(List L) {
	if (!L) {
		fprintf(stderr, "Cannot call front on a NULL reference.\n");
		exit(EXIT_FAILURE);
	}
	if (L->length > 0) {
		return L->front->data;
	}
	else {
		fprintf(stderr, "Cannot return front since length <= 0\n");
		freeList(&L);
		exit(EXIT_FAILURE);
	}
}

// Returns the value of the value at the back of list.
// Pre: not NULL
ListElement listBack(List L) {
	if (!L) {
		fprintf(stderr, "Cannot call back on a NULL reference.\n");
		exit(EXIT_FAILURE);
	}

	if (L->length > 0) {
		return L->back->data;
	}
	else {
		fprintf(stderr, "Cannot return back since length <= 0\n");
		freeList(&L);
		exit(EXIT_FAILURE);
	}
}

// Get the value at the cursor's location.
// Pre: Not NULL
ListElement get(List L) {
	if (!L) {
		fprintf(stderr, "Cannot call get on a NULL reference.\n");
		exit(EXIT_FAILURE);
	}

	if (length(L) <= 0) {
		fprintf(stderr, "The length is not greater than 0.\n");
		freeList(&L);
		exit(EXIT_FAILURE);
	}
	else if (getIndex(L) < 0) {
		fprintf(stderr, "The index is not greater or equal to 0.\n");
		freeList(&L);
		exit(EXIT_FAILURE);
	}
	else {
		return L->cursor->data;
	}
}


// Manipulation Procedures
// Pre: Not NULL
void clear(List L) {
	if (!L) {
		fprintf(stderr, "Error: Cannot clear null list object.\n");
		// freeList(&L);	// CHECK TO SEE IF FUNCTION CAN FREE NULL LIST
		exit(EXIT_FAILURE);
	}
    
    while (length(L) > 0) {
        deleteFront(L);
    }

	L->front = L->back = L->cursor = NULL;
	L->length = 0;
	L->index = -1;

}


// Move cursor to front
// Pre: L is not NULL and len > 0
void moveFront(List L) {
	if (L && (length(L) > 0)) {
		L->cursor  = L->front;
		L->index = 0;
	}
}

// Move cursor to back
// L is not NULL and len > 0
void moveBack(List L) {
	if (L && (length(L) > 0)) {
		L->cursor = L->back;
		L->index = length(L) - 1;
	}
}

// Move cursor 1 back
// Pre: L is not NULL and index > 0
void movePrev(List L) {
	if (L && L->cursor && L->index > 0 && L->index <= length(L)-1) {
		L->cursor = L->cursor->prev;
		L->index -= 1;
	}

	else if (L && L->cursor && L->index == 0) {
		L->cursor = NULL;
		L->index = -1;
	}
}

// Move cursor 1 forward
// Pre: L is not NULL and index is not -1
void moveNext(List L) {
	if (L && L->cursor && L->index >= 0 && L->index < length(L)-1) {
		L->cursor = L->cursor->next;
		L->index += 1;
	}

	else if (L && L->cursor && L->index == length(L)-1) {
		L->cursor = NULL;
		L->index = -1;
	}
}

// Add edge cases for when length is only 1
// Pre: L is not NULL and index is not -1
void prepend(List L, ListElement x) {
	if (L && length(L) > 0) {
		gNode to_prepend = new_gNode(x);
		to_prepend->next = L->front;
		L->front->prev = to_prepend;
		L->front = to_prepend;
		L->length += 1;

		if (L->cursor != NULL) {
			L->index += 1;
		}
	}
	else if (L && length(L) == 0) {
		gNode to_prepend = new_gNode(x);
		L->front = L->back = to_prepend;
		L->length += 1;
	}
}

// Append x to the list.
// Pre: L is not NULL and len>0
void append(List L, ListElement x) {
	if (L && length(L) > 0) {
		gNode to_append = new_gNode(x);
		to_append->prev = L->back;
		L->back->next = to_append;
		L->back = to_append;
		L->length += 1;

	}
	else if (L && length(L) == 0) {
		gNode to_append = new_gNode(x);
		L->front = L->back = to_append;
		L->length += 1;
	}
}

// Delete the node at the front
// Pre: L is not NULL and len>0
void deleteFront(List  L) {
	if (L && length(L) > 0) {
		bool is_cursor_deleted = false;
		if (L->cursor == L->front) {
			L->cursor = NULL;
			L->index = -1;
			is_cursor_deleted = true;
		}

		if (length(L) == 1) {
			free_gNode(&(L->front));
			L->front = L->back = NULL;
		}

		else {
			L->front = L->front->next;
			free_gNode(&(L->front->prev));
			L->front->prev = NULL;
			if (!is_cursor_deleted) {
				L->index -= 1;
			}
		}

		L->length -= 1;
	}
}

// Delete Node at the back.
// Pre: L is not NULL and len > 0
void deleteBack(List L) {
	if (L && length(L) > 0) {
		if (L->cursor == L->back) {
			L->cursor = NULL;
			L->index = -1;
		}

		if (length(L) == 1) {
			free_gNode(&(L->back));
			L->front = L->back = NULL;
		}

		else {
			L->back = L->back->prev;
			free_gNode(&(L->back->next));
			L->back->next = NULL;
		}

		L->length -= 1;
	}
}


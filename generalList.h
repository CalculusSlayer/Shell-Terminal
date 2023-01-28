// List.h
// - Contains function prototypes for List ADT

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void* ListElement; // Change this to change the type of List.

// Exported type
typedef struct ListObj* List; // Pointer to list object

// Constructors/Destructors
List newList (void); // Creates a new list.
void freeList(List *pL); // Frees a list.

// Access Functions
int length(List L); // Get length of list
int getIndex(List L); // Get index of cursor
ListElement listFront(List L); // Get front of list
ListElement listBack(List L); // Get back of list
ListElement get(List L); // Get value at cursor

// Manipulation Procedures
void clear(List L); // Clear the list
void moveFront(List L); // Move cursor to front
void moveBack(List L); // Move cursor to back
void movePrev(List L); // Move cursor one back
void moveNext(List L); // Move cursor one forward
void prepend(List L, ListElement x); // Add element to the beginning
void append(List L, ListElement x); // Add element to the end
void deleteFront(List  L); // Delete the front node.
void deleteBack(List L); // Delete the back node.

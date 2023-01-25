// linked_list.h
// Header file for LinkedList class
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Public Linked List struct that
// other files can use.
typedef struct LinkedListObj* LinkedList;

// newLinkedList()
// Allocates memory for a new Linked List
// and creates a reference for it.
LinkedList newLinkedList();

// freeLinkedList()
// Frees all memory allocated for Linked List
// including memory for the individual nodes.
void freeLinkedList(LinkedList* pL);

// getLength()
// Get length of the Linked List
int getLength(LinkedList L);

// front()
// Return string at front of Linked List
char* front(LinkedList L);

// back()
// Return string at back of Linked List
char* back(LinkedList L);

// isEmpty()
// Returns true or false depending on
// whether Linked List is empty or not.
bool isEmpty(LinkedList L);

// appendRight()
// Append string to end of Linked List
void appendRight(LinkedList L, char* val);

// PopLeft()
// Pop head of Linked List
void popLeft(LinkedList L);

// ll_to_str_arr()
// Convert linked list into string array
char** ll_to_str_arr(LinkedList L);

// printLinkedList()
// Print Linked List to a fileStream using
// fprintf().
void printLinkedList(FILE *fileStream, LinkedList L);



#ifndef DOUBLYLINKEDSTACKPRIVATE
#define DOUBLYLINKEDSTACKPRIVATE
#include <DLS.h>

typedef struct doublyLinkedStackNode{
	void *data;
	struct doublyLinkedStackNode *next;
	struct doublyLinkedStackNode *prev;
} doublyLinkedStackNode;

typedef struct doublyLinkedStack{
	size_t length;
	struct doublyLinkedStackNode *stack;
} doublyLinkedStack;

#endif
#ifndef DOUBLYLINKEDSTACK
#define DOUBLYLINKEDSTACK

struct doublyLinkedStack;

typedef struct doublyLinkedStack doublyLinkedStack;

doublyLinkedStack *DLS_create (void);

void DLS_push (doublyLinkedStack *DLS, void *inData);

void *DLS_pop (doublyLinkedStack *DLS);

void *DLS_get (doublyLinkedStack *DLS, size_t index);

doublyLinkedStack *DLS_find (doublyLinkedStack *DLS, int (*cnd)(void *));

void DLS_destroy (doublyLinkedStack *DLS);

void DLS_save (doublyLinkedStack *DLS, char *path, 
			   void (*saveNodeData)(FILE *, void *sData));

doublyLinkedStack *DLS_load (char *path, void *(*loadNodeData)(FILE *));

size_t DLS_length (doublyLinkedStack *DLS);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <DLSprivate.h>

doublyLinkedStack *DLS_create (void){
	doublyLinkedStack *newStack = malloc(sizeof(*newStack));
	newStack->stack = NULL;
	newStack->length = 0;
	return newStack;
}

void DLS_push (doublyLinkedStack *DLS, void *inData){
	doublyLinkedStackNode *newNode = malloc(sizeof(*newNode));
	if (newNode == NULL){
		fprintf(stderr, "%s\n", "DLS_push: not enough memory");
		return;
	}
	newNode->data = inData;
	newNode->next = NULL;
	if (DLS->stack){
		DLS->stack->next = newNode; //(DLS->stack)->next
	}
	newNode->prev = DLS->stack;
	DLS->stack = newNode;
	DLS->length++;
}

void *DLS_pop (doublyLinkedStack *DLS){
	if (DLS->stack == NULL){
		fprintf(stderr, "%s\n", "DLS_pop: stack is empty");
		return NULL;
	}
	doublyLinkedStackNode *node = DLS->stack;
	DLS->stack = node->prev;
	if (DLS->stack){
		DLS->stack->next = NULL;
	}
	DLS->length--;
	void *data = node->data;
	free(node);
	return data;
}

void *DLS_get (doublyLinkedStack *DLS, size_t index){
	size_t i = 0;
	doublyLinkedStackNode *currentNode = DLS->stack;
	for (; currentNode && i < index; i++){
		currentNode = currentNode->prev;
	}
	if (i < index || currentNode == NULL){
		fprintf(stderr, "%s\n", "DLS_get: out of range");
		return NULL;
	}
	return currentNode->data;
}

doublyLinkedStack *DLS_find (doublyLinkedStack *DLS, int (*cnd)(void *)){
	if (DLS->stack == NULL){
		fprintf(stderr, "%s\n", "DLS-find: stack is empty");
		return NULL;
	}
	doublyLinkedStack *newStack = DLS_create();
	doublyLinkedStackNode *currNode = DLS->stack;
	while (currNode){
		if ((*cnd)(currNode->data)){
			DLS_push(newStack, currNode->data);
		}
		currNode = currNode->prev;
	}
	return newStack;
}

void DLS_destroy (doublyLinkedStack *DLS){
	while (DLS->stack){
		doublyLinkedStackNode *node = DLS->stack;
		DLS->stack = node->prev;
		free(node);
	}
	free(DLS);
}

void DLS_save (doublyLinkedStack *DLS, char *path, 
			   void (*saveNodeData)(FILE *, void *sData)){
	FILE *stream = fopen(path, "w");
	if (stream == NULL){
		fprintf(stderr, "%s%s\n", "DLS_save: file open error. Path: ", path);
		return;
	}
	doublyLinkedStackNode *currNode = DLS->stack;
	if (currNode == NULL){
		fprintf(stderr, "%s\n", "DLS_save: stack is empty");
		return;
	}
	unsigned long length = 1;
	while (currNode->prev){
		currNode = currNode->prev;
		length++;
	}
	fprintf(stream, "%lu\n", length);
	while (currNode->next){
		(*saveNodeData)(stream, currNode->data);
		currNode = currNode->next;
	}
	(*saveNodeData)(stream, currNode->data);
	fclose(stream);
}

doublyLinkedStack *DLS_load (char *path, void *(*loadNodeData)(FILE *)){
	FILE *stream = fopen(path, "r");
	if (stream == NULL){
		fprintf(stderr, "%s%s\n", "DLS_save: file open error. Path: ", path);
		return NULL;
	}
	unsigned long length;
	fscanf(stream, "%lu", &length);
	doublyLinkedStack *newStack = DLS_create();
	while (length--){
		DLS_push(newStack, (*loadNodeData)(stream));
	}
	return newStack;
}

size_t DLS_length (doublyLinkedStack *DLS){
	return DLS->length;
}
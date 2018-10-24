#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <DLS.h>

typedef struct Buyer{
	char surname[32];
	char name[32];
	char city[32];
	char street[32];
	int house;
	int flat;
	char account[32];
	int averageSum;
} Buyer;

void svnddt (FILE *stream, Buyer *data){
	fprintf(stream, "%s\n", data->surname);
	fprintf(stream, "%s\n", data->name);
	fprintf(stream, "%s\n", data->city);
	fprintf(stream, "%s\n", data->street);
	fprintf(stream, "%d\n", data->house);
	fprintf(stream, "%d\n", data->flat);
	fprintf(stream, "%s\n", data->account);
	fprintf(stream, "%d\n", data->averageSum);
}

Buyer *ldnddt (FILE *stream){
	Buyer *newBuyer = malloc(sizeof(*newBuyer));
	fscanf(stream, "%s\n", (char *)&(newBuyer->surname));
	fscanf(stream, "%s\n", (char *)&(newBuyer->name));
	fscanf(stream, "%s\n", (char *)&(newBuyer->city));
	fscanf(stream, "%s\n", (char *)&(newBuyer->street));
	fscanf(stream, "%d\n", &(newBuyer->house));
	fscanf(stream, "%d\n", &(newBuyer->flat));
	fscanf(stream, "%s\n", (char *)&(newBuyer->account));
	fscanf(stream, "%d\n", &(newBuyer->averageSum));
	return newBuyer;
}

void freeData (doublyLinkedStack *DLS){
	for (int i = 0; i < DLS_length(DLS); i++){
		free(DLS_get(DLS, i));
	}
}

int cond (Buyer *data){
	return !strcmp(data->surname, "Putin");
}

doublyLinkedStack *search (doublyLinkedStack *DLS, char *surname){
	doublyLinkedStack *newStack = DLS_create();
	for (int i = 0; i < DLS_length(DLS); i++){
		Buyer *currBuyer = DLS_get(DLS, i);
		if (!strcmp(currBuyer->surname, surname)){
			DLS_push(newStack, currBuyer);
		}
	}
	return newStack;
}

int main (int argc, char **argv){
	doublyLinkedStack *dls = DLS_load("dls_r.txt", (void *(*)(FILE *))ldnddt);
	doublyLinkedStack *arr = DLS_find(dls, (int (*)(void *))cond);
	DLS_save(arr, "dls_w.txt", (void (*)(FILE *, void *))svnddt);
	doublyLinkedStack *arr2 = search(dls, "Medvedev");
	DLS_save(arr2, "dls_w2.txt", (void (*)(FILE *, void *))svnddt);
	freeData(dls);
	DLS_destroy(dls);
	DLS_destroy(arr);
	DLS_destroy(arr2);
}
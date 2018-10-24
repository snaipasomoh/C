#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int *readFile (char *adress, int *len){
	FILE *fIn = fopen(adress, "r");
	if (fIn == NULL){
		printf("Can't open %s\n", adress);
		return NULL;
	}
	
	fscanf(fIn, "%d", len);
	int *arr = malloc(sizeof(*arr)*(*len));
	for (int i = 0; i < *len; fscanf(fIn, "%d", arr+(i++)));
	fclose(fIn);
	return arr;
}

int write2File (char *adress, int *arr, int len){
	FILE *fOut = fopen(adress, "w");
	if (!fOut){
		printf("Can't open %s\n", adress);
		return 0;
	}
	
	fprintf(fOut, "%d", len);
	for (int i = 0; i < len; fprintf(fOut, " %d", arr[i++]));
	fclose(fOut);
	return 1;
}

int cmpFunc (int *a, int *b){
	return *a < *b;
}

void coctailSort (void *seq[], int len, int (*cmp)(void *, void *)){
	int left = 0;
	int right = len - 1;
	int swapCounter = 0;
	while (left <= right){
		int flag = 0;
		for (int i = left; i < right; i++){;
			if (!(*cmp)(seq[i], seq[i+1])){
				void *t = seq[i];
				seq[i] = seq[i+1];
				seq[i+1] = t;
				flag = 1;
				swapCounter++;
			}
		}
		if (!flag){
			break;
		}
		right--;
		flag = 0;
		for (int i = right; i > left; i--){
			if ((*cmp)(seq[i], seq[i-1])){
				void *t = seq[i];
				seq[i] = seq[i-1];
				seq[i-1] = t;
				flag = 1;
				swapCounter++;
			}
		}
		if (!flag){
			break;
		}
		left++;
	}
	printf("Array length in coctail sort: %d\n", len);
	printf("Swaps in coctail sort: %d\n", swapCounter);
}

void radixSort (int seq[], int len, int elMaxLen, int range){
	int **tempTable = malloc(sizeof(*tempTable)*range);
	for (int i = 0; i < range; i++){
		tempTable[i] = malloc(sizeof(**tempTable)*(len+1));
		tempTable[i][0] = 0;
	}
	
	for (int i = 0; i < elMaxLen; i++){
		for (int j = 0; j < len; j++){
			int key = seq[j]/((int)pow(10, i))%10;
			tempTable[key][++tempTable[key][0]] = seq[j];
		}
		int j = 0;
		for (int k = 0; k < range; k++){
			for (int l = 0; l < tempTable[k][0]; l++){
				seq[j++] = tempTable[k][l+1];
			}
			tempTable[k][0] = 0;
		}
	}
	
	for (int i = 0; i < range; i++){
		free(tempTable[i]);
	}
	free(tempTable);
}

int main (int argc, char **argv){
	srand(time(NULL));
	int i = rand() % 10000 + 10000;
	int *t = malloc(sizeof(*t) * i);
	for (int j = 0; j < i; j++){
		t[j] = rand();
	}
	write2File("mIn.txt", t, i);
	free(t);
	
	
	int len;
	int *arr = readFile("mIn.txt", &len);
	radixSort(arr, len, 5, 10);
	write2File("mOut.txt", arr, len);
	free(arr);
	
	arr = readFile("mIn.txt", &len);
	int **arrPtr = malloc(sizeof(*arrPtr) * len);
	for (int i = 0; i < len; i++){
		arrPtr[i] = arr + i;
	}
	coctailSort((void **)arrPtr, len, (int (*)(void *, void *))cmpFunc);
	int *newArr = malloc(sizeof(*newArr) * len);
	for (int i = 0; i < len; i++){
		newArr[i] = *(arrPtr[i]);
	}
	free(arr);
	free(arrPtr);
	write2File("mOut2.txt", newArr, len);
	free(newArr);
}
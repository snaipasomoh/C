#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int *calcNorms (int **matrix, int height, int width){
	int *norms = malloc(sizeof(*norms) * width);
	for (int i = 0; i < width; i++){
		norms[i] = 0;
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			norms[j] += matrix[i][j] * matrix[i][j];
		}
	}
	return norms;
}

void mergeSortIndices (int *seq, int len, int *norms){
	if (len > 1){
		int len1 = len / 2;
		int len2 = len - len1;
		int *seq1 = seq;
		int *seq2 = seq+len1;
		mergeSortIndices(seq1, len1, norms);
		mergeSortIndices(seq2, len2, norms);
		
		int i = 0;
		int j = 0;
		int seqf[len];
		for (int k = 0; k < len; k++){
			if (i == len1){
				seqf[k] = seq2[j++];
			}
			else if (j == len2){
				seqf[k] = seq1[i++];
			}
			else if (norms[seq1[i]] < norms[seq2[j]]){
				seqf[k] = seq1[i++];
			}
			else{
				seqf[k] = seq2[j++];
			}
		}
		
		for (int n = 0; n < len; n++){
			seq[n] = seqf[n];
		}
	}
}

int **sortVectors (int **matrix, int height, int width,
				   int *(*makeNorms)(int **m, int h, int w)){
	int *norms = (*makeNorms)(matrix, height, width);//malloc(sizeof(*norms) * width);
	int *indices = malloc(sizeof(*indices) * width);
	for (int i = 0; i < width; i++){
		indices[i] = i;
	}
	
	mergeSortIndices(indices, width, norms);
	free(norms);
	
	int **sortedMatrix = malloc(sizeof(*sortedMatrix) * height);
	for (int i = 0; i < height; i++){
		sortedMatrix[i] = malloc(sizeof(**sortedMatrix) * width);
		for (int j = 0; j < width; j++){
			sortedMatrix[i][j] = matrix[i][indices[j]];
		}
	}
	free(indices);
	return sortedMatrix;
}

int main (int argc, char **argv){
	int height;
	int width;
	printf("Enter matrix size: ");
	scanf("%d%d", &height, &width);
	int **matrix = malloc(sizeof(*matrix)*height);
	for (int i = 0; i < height; i++){
		matrix[i] = malloc(sizeof(**matrix)*width);
		for (int j = 0; j < width; j++){
			scanf("%d", matrix[i] + j);
		}
	}
	
	int **sortedMatrix = sortVectors(matrix, height, width,
									 (int *(*)(int **, int, int))calcNorms);
	
	printf("\nSorted matrix:\n");
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			printf("%d%s", sortedMatrix[i][j], j+1 == width ? "\n" : " ");
		}
		free(matrix[i]);
		free(sortedMatrix[i]);
	}
	free(matrix);
	free(sortedMatrix);
}
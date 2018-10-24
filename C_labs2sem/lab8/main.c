#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Buyer{
	char *name;
	char *surname;
	char *city;
	char *street;
	int house;
	int flat;
	char *account;
	int avCheqSum;
} Buyer;

typedef struct Tree{
	struct Buyer *buyer;
	struct Tree *left;
	struct Tree *right;
	struct Tree *parent;
} Tree;

Buyer *createBuyer (){
	Buyer *newBuyer = malloc(sizeof(*newBuyer));
	newBuyer->name = NULL;
	newBuyer->surname = NULL;
	newBuyer->city = NULL;
	newBuyer->street = NULL;
	newBuyer->house = 0;
	newBuyer->flat = 0;
	newBuyer->account = NULL;
	newBuyer->avCheqSum = 0;
	return newBuyer;
}

void destroyBuyer (Buyer *buyer){
	free(buyer->name);
	free(buyer->surname);
	free(buyer->city);
	free(buyer->street);
	free(buyer->account);
	free(buyer);
}

Buyer *copyBuyer (Buyer *buyer){
	Buyer *newBuyer = createBuyer();
	newBuyer->name = malloc(sizeof(*(newBuyer->name)) * 32);
	newBuyer->surname = malloc(sizeof(*(newBuyer->surname)) * 32);
	newBuyer->city = malloc(sizeof(*(newBuyer->city)) * 32);
	newBuyer->street = malloc(sizeof(*(newBuyer->street)) * 32);
	newBuyer->account = malloc(sizeof(*(newBuyer->account)) * 32);
	strcpy(newBuyer->name, buyer->name);
	strcpy(newBuyer->surname, buyer->surname);
	strcpy(newBuyer->city, buyer->city);
	strcpy(newBuyer->street, buyer->street);
	newBuyer->house = buyer->house;
	newBuyer->flat = buyer->flat;
	strcpy(newBuyer->account, buyer->account);
	newBuyer->avCheqSum = buyer->avCheqSum;
	return newBuyer;
}

void saveBuyer (Buyer *buyer, FILE *stream){
	fprintf(stream, "%s\n", buyer->surname);
	fprintf(stream, "%s\n", buyer->name);
	fprintf(stream, "%s\n", buyer->city);
	fprintf(stream, "%s\n", buyer->street);
	fprintf(stream, "%d\n", buyer->house);
	fprintf(stream, "%d\n", buyer->flat);
	fprintf(stream, "%s\n", buyer->account);
	fprintf(stream, "%d\n", buyer->avCheqSum);
}

Buyer *loadBuyer (FILE *stream){		//TODO: function for reading string of any length
	if (feof(stream)){
		return NULL;
	}
	Buyer *newBuyer = malloc(sizeof(*newBuyer));
	newBuyer->name = malloc(sizeof(*(newBuyer->name)) * 32);
	newBuyer->surname = malloc(sizeof(*(newBuyer->surname)) * 32);
	newBuyer->city = malloc(sizeof(*(newBuyer->city)) * 32);
	newBuyer->street = malloc(sizeof(*(newBuyer->street)) * 32);
	newBuyer->account = malloc(sizeof(*(newBuyer->account)) * 32);
	fscanf(stream, "%s\n", newBuyer->surname);
	fscanf(stream, "%s\n", newBuyer->name);
	fscanf(stream, "%s\n", newBuyer->city);
	fscanf(stream, "%s\n", newBuyer->street);
	fscanf(stream, "%d\n", &(newBuyer->house));
	fscanf(stream, "%d\n", &(newBuyer->flat));
	fscanf(stream, "%s\n", newBuyer->account);
	fscanf(stream, "%d\n", &(newBuyer->avCheqSum));
	return newBuyer;
}

void initBuyer (Buyer *buyer, 
				char *surname,
				char *name,
				char *city,
				char *street,
				int house,
				int flat,
				char *account,
				int avCheqSum){
	buyer->name = malloc(sizeof(*(buyer->name)) * 32);
	buyer->surname = malloc(sizeof(*(buyer->surname)) * 32);
	buyer->city = malloc(sizeof(*(buyer->city)) * 32);
	buyer->street = malloc(sizeof(*(buyer->street)) * 32);
	buyer->account = malloc(sizeof(*(buyer->account)) * 32);
	strcpy(buyer->surname, surname);
	strcpy(buyer->name, name);
	strcpy(buyer->city, city);
	strcpy(buyer->street, street);
	strcpy(buyer->account, account);
	buyer->house = house;
	buyer->flat = flat;
	buyer->avCheqSum = avCheqSum;
}

Tree *createTree (){
	Tree *newTree = malloc(sizeof(*newTree));
	newTree->buyer = NULL;
	newTree->left = NULL;
	newTree->right = NULL;
	newTree->parent = NULL;
	return newTree;
}

void addBuyer (Tree *tree, Buyer *buyer){
	if (tree == NULL){
		fprintf(stderr, "addBuyer: tree is NULL\n");
		return;
	}
	if (buyer == NULL){
		fprintf(stderr, "addBuyer: buyer is NULL\n");
		return;
	}
	if (tree->buyer == NULL){
		tree->buyer = buyer;
		return;
	}
	if (strcmp(buyer->surname, tree->buyer->surname) < 0){
		if (tree->left == NULL){
			Tree *newTree = createTree();
			newTree->buyer = buyer;
			newTree->parent = tree;
			tree->left = newTree;
			return;
		}
		addBuyer(tree->left, buyer);
	}
	else{
		if (tree->right == NULL){
			Tree *newTree = createTree();
			newTree->buyer = buyer;
			newTree->parent = tree;
			tree->right = newTree;
			return;
		}
		addBuyer(tree->right, buyer);
	}
}

void removeBuyer (Tree *tree, char *keySurname){
	if (tree == NULL){
		return;
	}
	if (tree->buyer == NULL){
		return;
	}
	int t = strcmp(keySurname, tree->buyer->surname);
	if (t < 0){
		removeBuyer (tree->left, keySurname);
	}
	else if (t > 0){
		removeBuyer (tree->right, keySurname);
	}
	else{
		while (tree->right && tree->right->buyer && 
			   strcmp(tree->right->buyer->surname, keySurname) == 0){
			tree->right = tree->right->right;
			destroyBuyer(tree->right->parent->buyer);
			free(tree->right->parent);
			tree->right->parent = tree;
		}
		if (tree->left == NULL && tree->right == NULL){
			if (tree == tree->parent->right){
				tree->parent->right = NULL;
			}
			else{
				tree->parent->left = NULL;
			}
			destroyBuyer(tree->buyer);
			free(tree);
		}
		else if (tree->left == NULL){
			if (tree == tree->parent->right){
				tree->parent->right = tree->right;
			}
			else{
				tree->parent->left = tree->right;
			}
			destroyBuyer(tree->buyer);
			free(tree);
		}
		else if (tree->right == NULL){
			if (tree == tree->parent->right){
				tree->parent->right = tree->left;
			}
			else{
				tree->parent->left = tree->left;
			}
			destroyBuyer(tree->buyer);
			free(tree);
		}
		else{
			if (tree->right->left == NULL){
				if (tree == tree->parent->right){
					tree->parent->right = tree->right;
				}
				else{
					tree->parent->left = tree->right;
				}
				tree->right->parent = tree->parent;
				tree->right->left = tree->left;
			}
			else{
				Tree *currTree = tree->right->left;
				while (currTree->left){
					currTree = currTree->left;
				}
				currTree->parent->left = currTree->right;
				if (currTree->right){
					currTree->right->parent = currTree->parent;
				}
				if (tree == tree->parent->right){
					tree->parent->right = currTree;
				}
				else{
					tree->parent->left = currTree;
				}
				currTree->right = tree->right;
				currTree->left = tree->left;
				currTree->parent = tree->parent;
			}
			destroyBuyer(tree->buyer);
			free(tree);
		}
	}
}

Tree *findBuyer (Tree *tree, char *keySurname, int needCopies){
	if (tree == NULL){
		return NULL;
	}
	if (tree->buyer == NULL){
		fprintf(stderr, "findBuyer: buyer is NULL\n");
		return NULL;
	}
	int t = strcmp(keySurname, tree->buyer->surname);
	if (t < 0){
		return findBuyer(tree->left, keySurname, needCopies);
	}
	if (t > 0){
		return findBuyer(tree->right, keySurname, needCopies);
	}
	Tree *newTree = createTree();
	while (tree && tree->buyer && 
		   strcmp(keySurname, tree->buyer->surname) == 0){
		if (needCopies){
			addBuyer(newTree, copyBuyer(tree->buyer));
		}
		else{
			addBuyer(newTree, tree->buyer);
		}
		tree = tree->right;
	}
	return newTree;
}

void searchBuyerPrivate (Tree *tree, Tree *newTree,
						 int (*cond)(Buyer *), int needCopies){
	if (tree == NULL){
		return;
	}
	if (tree->buyer == NULL){
		return;
	}
	if ((*cond)(tree->buyer)){
		if (needCopies){
			addBuyer(newTree, copyBuyer(tree->buyer));
		}
		else{
			addBuyer(newTree, tree->buyer);
		}
	}
	searchBuyerPrivate(tree->left, newTree, cond, needCopies);
	searchBuyerPrivate(tree->right, newTree, cond, needCopies);
}

Tree *searchBuyer (Tree *tree, int (*cond)(Buyer *), int needCopies){
	Tree *newTree = createTree();
	searchBuyerPrivate(tree, newTree, cond, needCopies);
	return newTree;
}

void saveTreePrivate (Tree *tree, FILE *stream,
					  void (*saveBuyer)(Buyer *, FILE *)){
	if (tree == NULL){
		return;
	}
	(*saveBuyer)(tree->buyer, stream);
	saveTreePrivate(tree->left, stream, saveBuyer);
	saveTreePrivate(tree->right, stream, saveBuyer);
}

void saveTree (Tree *tree, char *path, 
			   void (*saveBuyer)(Buyer *, FILE *)){
	FILE *stream = fopen(path, "w");
	if (stream == NULL){
		fprintf(stderr, "saveTree: no such file %s\n", path);
		return;
	}
	saveTreePrivate(tree, stream, saveBuyer);
	fclose(stream);
}

Tree *loadTree (char *path, Buyer *(*loadBuyer)(FILE *)){
	FILE *stream = fopen(path, "r");
	if (stream == NULL){
		fprintf(stderr, "loadTree: no such file %s\n", path);
		return NULL;
	}
	Tree *newTree = createTree();
	Buyer *newBuyer;
	while ((newBuyer = (*loadBuyer)(stream)) != NULL){
		addBuyer(newTree, newBuyer);
	}
	fclose(stream);
	return newTree;
}

void destroyTree (Tree *tree, int needDataFree){
	if (tree == NULL){
		return;
	}
	destroyTree(tree->left, needDataFree);
	destroyTree(tree->right, needDataFree);
	if (needDataFree && tree->buyer){
		destroyBuyer(tree->buyer);
	}
	free(tree);
}

int main (int argc, char **argv){
	Tree *foo = loadTree("dls_r.txt", &loadBuyer);
	Tree *baz = findBuyer(foo, "Putin", 1);
	saveTree(baz, "dls_w2.txt", &saveBuyer);
	destroyTree(baz, 1);
	removeBuyer(foo, "Medvedev");
	Buyer *bar = createBuyer();
	initBuyer(bar, "Ivanov", "Ivan", "Ivanovsk",
			  "Ivanovskaya", 13, 666, "5675", 0);
	addBuyer(foo, bar);
	saveTree(foo, "dls_w.txt", &saveBuyer);
	destroyTree(foo, 1);
}

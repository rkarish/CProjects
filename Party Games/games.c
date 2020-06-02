#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct player {
	char name[20];
	char game[20];
};

struct node {
	struct player player;
	struct node *left;
	struct node *right;
};

struct node *createNode(struct player player),
		    *insertNode(struct node *root, struct node *element),
			*parent(struct node *root, struct node *node),
			*findNode(struct node *node, char *name),
			*minVal(struct node *root),
			*maxVal(struct node *root),
			*deleteNode(struct node *root, char *name);

int isLeaf(struct node *node),
	onlyLeftChild(struct node *node),
	onlyRightChild(struct node *node);

void freeNode(struct node *node),
	 print(struct node *node),
	 printGame(struct node *node, char *game);

int size = 0, i = 0;
FILE *inputFile, 
	 *outputFile;

int main() {
	char command[20], game[20], name[20];
	struct node *root = NULL;
	struct player temp;
	struct player *playerArray;
	int j = 0, cont = 1;

	//Open the input and output files
	inputFile = fopen("party.txt", "r");
	outputFile = fopen("partygames.out", "w");

	fscanf(inputFile, "%s", &command);

	//Read the commands from the input file
	while (cont != 0) {
		if (strcmp(command, "ADD") == 0) {
			fscanf(inputFile, "%s %s", &temp.name, &temp.game);
			root = insertNode(root,createNode(temp));
		} else if (strcmp(command, "REMOVE") == 0) {
			fscanf(inputFile, "%s", &name);
			deleteNode(root, name);
		} else if (strcmp(command, "PRINTGAME") == 0) {
			fscanf(inputFile, "%s", &game);
			printf("%s:\n", game);
			fprintf(outputFile, "%s:\n", game);
			printGame(root, game);
		} else if (strcmp(command, "PRINT") == 0) {
			printf("PRINT:\n");
			fprintf(outputFile, "PRINT:\n");
			print(root);
		} else if (strcmp(command, "PART2") == 0) {
			printf("PART2\n");
			fprintf(outputFile, "PART2\n");
			cont = 0;
			fprintf(outputFile, "MONOPOLY\n");
			printf("MONOPOLY\n");
			printGame(root, "MONOPOLY");
			fprintf(outputFile, "SCRABBLE\n");
			printf("SCRABBLE\n");
			printGame(root, "SCRABBLE");
		}
		fscanf(inputFile, "%s", &command);
	}

	scanf("%s", &command);
	return 0;
}

//Creates a new node
struct node *createNode(struct player player) {
	struct node *temp;
	temp = (struct node *)malloc(sizeof(struct node));
	temp->player = player;
	temp->left = temp->right = NULL;
	size++;
	return temp;
}

//Inserts a node into the BST
struct node *insertNode(struct node *root, struct node *element) {
	if (root == NULL) {
		return element;
	} else {
		if (strcmp(element->player.name, root->player.name) > 0) {
			root->right = insertNode(root->right, element);
		} else {
			root->left = insertNode(root->left, element);
		}
		return root;
	}
}

//Frees a node from memory
void freeNode(struct node *node) {
	free(node);
}

//Prints the contents of a BST inorder
void print(struct node *node) {
	if (node != NULL) {
		print(node->left);
		printf("%s %s\n", node->player.name, node->player.game);
		fprintf(outputFile, "%s %s\n", node->player.name, node->player.game);
		print(node->right);
	}
}

//Prints a specific game of the BST
void printGame(struct node *node, char *game) {
	if (node != NULL) {
		printGame(node->left, game);
		if (strcmp(node->player.game, game) == 0) {
			printf("%s\n", node->player.name);
			fprintf(outputFile, "%s\n", node->player.name);
		}
		printGame(node->right, game);
	}
}

//Finds the parents of a node
struct node *parent(struct node *root, struct node *node) {
	if (root == NULL || root == node) {
		return NULL;
	} else if (root->left == node || root->right == node) {
		return root;
	} else if (strcmp(node->player.name, root->player.name) < 0) {
		return parent(root->left, node);
	} else if (strcmp(node->player.name, root->player.name) > 0) {
		return parent(root->right, node);
	} else {
		return NULL;
	}
}

//Finds a specific node
struct node *findNode(struct node *node, char *name) {
	if (node != NULL) {
		if (strcmp(node->player.name, name) == 0) {
			return node;
		} else if (strcmp(name, node->player.name) < 0) {
			return findNode(node->left, name);
		} else {
			return findNode(node->right, name);
		}
	} else {
		return NULL;
	}
}

//Determines the min value
struct node *minVal(struct node *root) {
	if (root->left == NULL) {
		return root;
	} else {
		return minVal(root->left);
	}
}

//Determines the max value
struct node *maxVal(struct node *root) {
	if (root->right == NULL) {
		return root;
	} else {
		return maxVal(root->right);
	}
}

//Determines if a node is a leaf
int isLeaf(struct node *node) {
	return (node->left == NULL && node->right == NULL);
}

int onlyLeftChild(struct node *node) {
	return (node->left != NULL && node->right == NULL);
}

int onlyRightChild(struct node *node) {
	return (node->left == NULL && node->right != NULL);
}

//Deletes a node from the BST
struct node *deleteNode(struct node *root, char *name) {
	struct node *delete_node, *new_delete_node, *save_node, *parent_node;
	char save_player[20];

	delete_node = findNode(root, name);
	parent_node = parent(root, delete_node);

	size--;

	if (isLeaf(delete_node)) {
		if (parent_node == NULL) {
			free(root);
			return NULL;
		} else if (strcmp(name, parent_node->player.name) < 0) {
			free(parent_node->left);
			parent_node->left = NULL;
		} else {
			free(parent_node->right);
			parent_node->right = NULL;
		}
		return root;
	} else if (onlyLeftChild(delete_node)) {
		if (parent_node == NULL) {
			save_node = delete_node->left;
			free(delete_node);
			return save_node;
		} else if (strcmp(name, parent_node->player.name) < 0) {
			save_node = parent_node->left;
			parent_node->left = parent_node->left->left;
			free(save_node);
		} else {
			save_node = parent_node->right;
			parent_node->right = parent_node->right->left;
			free(save_node);
		}
		return root;
	} else if (onlyRightChild(delete_node)) {
		if (parent_node == NULL) {
			save_node = delete_node->right;
			free(delete_node);
			return save_node;
		} else if (strcmp(name, parent_node->player.name) < 0) {
			save_node = parent_node->left;
			parent_node->left = parent_node->left->right;
			free(save_node);
		} else {
			save_node = parent_node->right;
			parent_node->right = parent_node->right->right;
			free(save_node);
		}
		return root;
	}

	new_delete_node = minVal(delete_node->right);
	strcpy(save_player, new_delete_node->player.name);
	deleteNode(root, save_player);
	strcpy(delete_node->player.name, save_player);
	return root;
}


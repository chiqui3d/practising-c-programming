#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Represents a node
struct Node {
    int number;
    struct Node *left;
    struct Node *right;
};

// Creates a new node

void addNode(struct Node **tree, int number) {
    if (*tree == NULL) {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->left = temp->right = NULL;
        temp->number = number;
        *tree = temp;
        return;
    }

    // If the number is less than the root, then it should be inserted in the left subtree
    if (number < (*tree)->number) {
        addNode(&(*tree)->left, number);
    } // If the number is greater than the root, then it should be inserted in the right subtree
    else if (number > (*tree)->number) {
        addNode(&(*tree)->right, number);
    } else {
        printf("Number %d already exists in the tree\n", number);
    }
}

struct Node *searchNodeByNumber(struct Node *tree, int number) {
    if (tree == NULL) {
        return NULL;
    } else if (number < tree->number) {
        return searchNodeByNumber(tree->left, number);
    } else if (number > tree->number) {
        return searchNodeByNumber(tree->right, number);
    } else if (number == tree->number) {
        return tree;
    } else {
        return NULL;
    }
}

// https://www.cs.usfca.edu/~galles/visualization/BST.html
void removeNode(struct Node **tree, int number) {
    if (*tree == NULL) {
        return;
    } else if (number < (*tree)->number) {
        removeNode(&(*tree)->left, number);
    } else if (number > (*tree)->number) {
        removeNode(&(*tree)->right, number);
    } else {
        // Case 1: No child
        if ((*tree)->left == NULL && (*tree)->right == NULL) {
            printf("Case 1\n");
            free(*tree);
            *tree = NULL;
            // Case 2: One child (right))
        } else if ((*tree)->left == NULL) {
            printf("Case 2\n");
            struct Node *temp = *tree;
            *tree = (*tree)->right;
            free(temp);
            // Case 3: One child (left)
        } else if ((*tree)->right == NULL) {
            printf("Case 3\n");
            struct Node *temp = *tree;
            *tree = (*tree)->left;
            free(temp);
            // Case 4: Two children
        } else {
            printf("Case 4\n");
            struct Node *temp = (*tree)->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            //printf("%d - %d\n", (*tree)->number, temp->number);
            (*tree)->number = temp->number;
            removeNode(&(*tree)->right, temp->number);
        }
    }
}

// Prints the struct node in-order
void printTree(struct Node *tree) {
    if (tree != NULL) {
        printTree(tree->left);
        printf("%d\n", tree->number);
        printTree(tree->right);
    }
}

void preOrder(struct Node *tree) {
    if (tree != NULL) {
        printf("%d\n", tree->number);
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

void postOrder(struct Node *tree) {
    if (tree != NULL) {
        postOrder(tree->left);
        postOrder(tree->right);
        printf("%d\n", tree->number);
    }
}

// reverse in-order
void printTreeReverse(struct Node *tree) {
    if (tree != NULL) {
        printTreeReverse(tree->right);
        printf("%d\n", tree->number);
        printTreeReverse(tree->left);
    }
}

void freeTree(struct Node *tree) {
    if (tree != NULL) {
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree);
    }
}

int sizeTree(struct Node *tree) {
    if (tree == NULL) {
        return 0;
    } else {
        return sizeTree(tree->left) + 1 + sizeTree(tree->right);
    }
}

void heightTree(struct Node *tree, int *height, int currentHeight) {
    if (tree == NULL) {
        return;
    } else {
        currentHeight++;
        if (currentHeight > *height) {
            *height = currentHeight;
        }
        heightTree(tree->left, height, currentHeight);
        heightTree(tree->right, height, currentHeight);
    }
}

void heightLeftTree(struct Node *tree, int *height, int currentHeight) {
    if (tree == NULL) {
        return;
    } else {
        currentHeight++;
        if (currentHeight > *height) {
            *height = currentHeight;
        }
        heightLeftTree(tree->left, height, currentHeight);
    }
}

void heightRightTree(struct Node *tree, int *height, int currentHeight) {
    if (tree == NULL) {
        return;
    } else {
        currentHeight++;
        if (currentHeight > *height) {
            *height = currentHeight;
        }
        heightRightTree(tree->right, height, currentHeight);
    }
}

// Balance tree or AVL: 1, 0, or -1
int calculateBalanceFactor(struct Node *tree) {
    int heightLeft = 0;
    int heightRight = 0;
    heightLeftTree(tree->left, &heightLeft, 0);
    heightRightTree(tree->right, &heightRight, 0);
    return heightLeft - heightRight;
}

int main(void) {
    struct Node *tree = NULL;

    addNode(&tree, 50); // root
    addNode(&tree, 30);
    addNode(&tree, 70);
    addNode(&tree, 20);
    addNode(&tree, 40);
    addNode(&tree, 60);
    addNode(&tree, 80);
    addNode(&tree, 15);

    removeNode(&tree, 50);

    printTree(tree);
    printf("\n");
    printTreeReverse(tree);
    printf("\n");

    struct Node *node = searchNodeByNumber(tree, 70);

    if (node != NULL) {
        printf("Found node with number %d\n", node->number);
    } else {
        printf("Node not found\n");
    }

    printf("Size: %d\n", sizeTree(tree));

    int height = 0;
    heightTree(tree, &height, 0);
    printf("Height: %d\n", height);

    height = 0;
    heightLeftTree(tree, &height, 0);
    printf("Height left: %d\n", height);

    height = 0;
    heightRightTree(tree, &height, 0);
    printf("Height right: %d\n", height);

    printf("Balance factor: %d\n", calculateBalanceFactor(tree));

    freeTree(tree);

    return EXIT_SUCCESS;
}

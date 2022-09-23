#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// Binary Search Tree AVL


// Represents a node
struct Node {
    int number;
    int height; // height of the node respect to the absolute root
    struct Node *left;
    struct Node *right;
};

void addNode(struct Node **tree, int number);
void balanceTree(struct Node **tree, int number);
void deleteNode(struct Node **tree, int number);

void rotateRight(struct Node **tree);
void rotateLeft(struct Node **tree);

void printTree(struct Node *tree);
void printPreOrder(struct Node *tree);

void freeTree(struct Node *tree);

int height(struct Node *tree);
int max(int left, int right);
int calcHeight(struct Node *tree);
struct Node *minValueNode(struct Node *node);
struct Node *maxValueNode(struct Node *node);
int calculateBalanceFactor(struct Node *tree);
int totalNodes(struct Node *tree);

int main(void) {
    struct Node *tree = NULL;

    addNode(&tree, 50); // root
    addNode(&tree, 30);
    addNode(&tree, 70);
    addNode(&tree, 20);
    addNode(&tree, 40);
    addNode(&tree, 100);
    addNode(&tree, 120);
    addNode(&tree, 60);
    addNode(&tree, 80);
    addNode(&tree, 15);
    addNode(&tree, 12);
    addNode(&tree, 5);
    addNode(&tree, 10);
    addNode(&tree, 75);
    addNode(&tree, 45);

   
    printf("Preorder before delete:\n");
    printf("Root: %d - height: %d\n", tree->number, tree->height);
    printf("Preorder left:\n");
    printPreOrder(tree->left);
    printf("Preorder right:\n");
    printPreOrder(tree->right);
    printf("\n");

    deleteNode(&tree, 40);
    deleteNode(&tree, 70);
    deleteNode(&tree, 50);
    deleteNode(&tree, 100);

    printf("Preorder after delete:\n");
    printf("Root: %d - height: %d\n", tree->number, tree->height);
    printf("Preorder left:\n");
    printPreOrder(tree->left);
    printf("Preorder right:\n");
    printPreOrder(tree->right);
    printf("\n");

    printf("Sort tree:\n");
    printTree(tree);
    printf("\n");

    printf("Left nodes: %d\n", totalNodes(tree->left));
    printf("Right nodes: %d\n", totalNodes(tree->right));
    printf("Total nodes: %d\n", totalNodes(tree));
    printf("Balance factor: %d\n", calculateBalanceFactor(tree));

    freeTree(tree);

    return EXIT_SUCCESS;
}

void addNode(struct Node **tree, int number) {
    if (*tree == NULL) {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        if (temp == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        temp->left = temp->right = NULL;
        temp->number = number;
        temp->height = 0;
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
        return;
    }

    balanceTree(&(*tree), number);
}

void balanceTree(struct Node **tree, int number) {

     (*tree)->height = calcHeight(*tree);
    // Calculate the balance factor of the tree
    int balanceFactor = calculateBalanceFactor(*tree);


    // https://en.wikipedia.org/wiki/AVL_tree#Rebalancing
    // https://stackoverflow.com/questions/575772/the-best-way-to-calculate-the-height-in-a-binary-search-tree-balancing-an-avl
    // https://gist.github.com/Harish-R/097688ac7f48bcbadfa5
    // printf("Balance factor: %d\n", balanceFactor);
    
    // left unbalanced case
    if (balanceFactor == 2) {
         // left case
        if (calculateBalanceFactor((*tree)->left) == 1 || number < (*tree)->left->number) {
            rotateRight(&(*tree));
            return;
        }else{
            // left right case
            rotateLeft(&(*tree)->left);
            rotateRight(&(*tree));
            return;
        }
    }else if (balanceFactor == -2) { // right unbalanced case
       
        // right case
        if (calculateBalanceFactor((*tree)->right) == -1 || number > (*tree)->right->number) {
            rotateLeft(&(*tree));
            return;
        }else{
            // right left case
            rotateRight(&(*tree)->right);
            rotateLeft(&(*tree));
            return;
        }

    }
}

void deleteNode(struct Node **tree, int number) {
    if (*tree == NULL) {
        return;
    } else if (number < (*tree)->number) {
        deleteNode(&(*tree)->left, number);
    } else if (number > (*tree)->number) {
        deleteNode(&(*tree)->right, number);
    } else {
        // Case 1: Node with only one child or no child
        if (((*tree)->left == NULL) || ((*tree)->right == NULL)) {
            struct Node *temp = (*tree)->left ? (*tree)->left : (*tree)->right;
            // Case 1.1: No child case
            if (temp == NULL) {
                temp = *tree;
                (*tree) = NULL;
            } else {
                // Case 1.2: One child case
                **tree = *temp;
                // Copy the contents of the non-empty child
            }
            free(temp);
        } else {
            // Case 2: node with two children: Get the inorder successor (smallest in the right subtree)
            struct Node *temp = minValueNode((*tree)->right);

            // Copy the inorder successor's data to this node
            (*tree)->number = temp->number;

            // Delete the inorder successor
            deleteNode(&(*tree)->right, temp->number);
        }
    }

    if (*tree == NULL) {
        return;
    }

   balanceTree(&(*tree), number);

}

struct Node *minValueNode(struct Node *node) {
    struct Node *current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL){
        current = current->left;
    }

    return current;
}

struct Node *maxValueNode(struct Node *node) {
    struct Node *current = node;

    /* loop down to find the leftmost leaf */
    while (current->right != NULL){
        current = current->right;
    }

    return current;
}


void rotateLeft(struct Node **tree) {
    struct Node *temp = (*tree)->right;
    (*tree)->right = temp->left;
    temp->left = *tree;
    (*tree)->height = calcHeight(*tree);
    temp->height = calcHeight(temp);
    *tree = temp;
}


void rotateRight(struct Node **tree) {
    struct Node *temp = (*tree)->left;
    (*tree)->left = temp->right;
    temp->right = *tree;
    (*tree)->height = calcHeight(*tree);
    temp->height = calcHeight(temp);
    *tree = temp;
}


// Prints the struct node in-order
void printTree(struct Node *tree) {
    if (tree != NULL) {
        printTree(tree->left);
        printf("%d - height: %d\n", tree->number, tree->height);
        printTree(tree->right);
    }
}

void printPreOrder(struct Node *tree) {
    if (tree != NULL) {
        printf("%d - height: %d\n", tree->number, tree->height);
        printPreOrder(tree->left);
        printPreOrder(tree->right);
    }
}

void freeTree(struct Node *tree) {
    if (tree != NULL) {
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree);
    }
}
int height(struct Node *tree) {
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}
int max(int left, int right) {

    return (left > right) ? left : right;
}

// Take the maximum of the height of the subtrees + 1 by recursion.
int calcHeight(struct Node *tree) {
    if (tree == NULL) {
        return 0;
    }
    return max(height(tree->left), height(tree->right)) + 1;
}
// Balance tree or AVL: 1, 0, or -1
int calculateBalanceFactor(struct Node *tree) {
    return height(tree->left) - height(tree->right);
}

int totalNodes(struct Node *tree) {
    if (tree == NULL) {
        return 0;
    }
    return 1 + totalNodes(tree->left) + totalNodes(tree->right);
}
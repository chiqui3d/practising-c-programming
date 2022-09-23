#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// Binary Search Tree Red Black

enum NodeColor {
    RED,
    BLACK
};

// Represents a node
struct Node {
    int number;
    enum NodeColor color;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
};

struct Node *newNode(int number);
struct Node *addNode(struct Node **tree, struct Node *newNode);
void addNodeRedBlack(struct Node **tree, struct Node *newNode);
void fixUp(struct Node **tree, struct Node *newNode);
void rotateLeft(struct Node **tree, struct Node *node);
void rotateRight(struct Node **tree, struct Node *node);

struct Node *searchNode(struct Node *tree, int number);

struct Node *minValueNode(struct Node *node);
struct Node *maxValueNode(struct Node *node);
int totalNodes(struct Node *tree);

void printTree(struct Node *tree);
void printPreOrder(struct Node *tree);
void freeTree(struct Node *tree);





int main(void) {
    struct Node *tree = NULL;

    addNodeRedBlack(&tree, newNode(50)); // root
    addNodeRedBlack(&tree, newNode(30));
    addNodeRedBlack(&tree, newNode(70));
    addNodeRedBlack(&tree, newNode(20));
    addNodeRedBlack(&tree, newNode(40));
    addNodeRedBlack(&tree, newNode(100));
    addNodeRedBlack(&tree, newNode(120));
    addNodeRedBlack(&tree, newNode(60));
    addNodeRedBlack(&tree, newNode(80));
    addNodeRedBlack(&tree, newNode(15));
    addNodeRedBlack(&tree, newNode(12));
    addNodeRedBlack(&tree, newNode(5));
    addNodeRedBlack(&tree, newNode(10));
    addNodeRedBlack(&tree, newNode(75));
    addNodeRedBlack(&tree, newNode(45));

    printf("Preorder before delete:\n");
    printf("Root: %d - color: %s\n", tree->number, tree->color == RED ? "RED" : "BLACK");
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

    freeTree(tree);

    return EXIT_SUCCESS;
}

struct Node *newNode(int number){
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    node->number = number;
    node->color = RED;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct Node *addNode(struct Node **tree, struct Node *newNode) {
    if (*tree == NULL) {
        *tree = newNode;
        return *tree;
    }
    if (newNode->number < (*tree)->number) {
        (*tree)->left = addNode(&(*tree)->left, newNode);
        (*tree)->left->parent = *tree;
    } else if (newNode->number > (*tree)->number) {
        (*tree)->right = addNode(&(*tree)->right, newNode);
        (*tree)->right->parent = *tree;
    }

    return *tree;
}

void addNodeRedBlack(struct Node **tree, struct Node *newNode) {
   
   addNode(&(*tree), newNode);
   (*tree)->color = BLACK;
   fixUp(&(*tree), newNode);

}

void rotateLeft(struct Node **tree, struct Node *node) {

    struct Node *right = node->right;
    node->right = right->left;

    if (node->right != NULL) {
        node->right->parent = node;
    }

    right->parent = node->parent;

    if (node->parent == NULL) { // node is root
        *tree = right;
    } else if (node == node->parent->left) { // node is left child
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left = node;
    node->parent = right;
   
}

void rotateRight(struct Node **tree, struct Node *node) {
    
        struct Node *left = node->left;
        node->left = left->right;
    
        if (node->left != NULL) {
            node->left->parent = node;
        }
    
        left->parent = node->parent;
    
        if (node->parent == NULL) {
            *tree = left;
        } else if (node == node->parent->left) {
            node->parent->left = left;
        } else {
            node->parent->right = left;
        }
    
        left->right = node;
        node->parent = left;
}

void fixUp(struct Node **tree, struct Node *newNode) {
    // Case 1: newNode is root
    if (newNode->parent == NULL) {
        newNode->color = BLACK;
        return;
    }
    // Case 2: parent is black
    if (newNode->parent->color == BLACK) {
        return;
    }
    // Case 3: parent and uncle are red
    if (newNode->parent->parent == NULL) {
        return;
    }
    struct Node *grandParent = newNode->parent->parent;
    struct Node *uncle = NULL;

    // Case 3: parent and uncle are red
    if (grandParent->left == newNode->parent) {
        uncle = grandParent->right;
    } else {
        uncle = grandParent->left;
    }
    // Uncle is red
    if (uncle != NULL && uncle->color == RED) {
        newNode->parent->color = BLACK;
        uncle->color = BLACK;
        grandParent->color = RED;
        fixUp(&(*tree), grandParent);
    } else { // Uncle is black
        // Case 4: newNode is right child and parent is left child
        if (grandParent->left == newNode->parent && newNode->parent->right == newNode) {
            rotateLeft(&(*tree), newNode->parent);
            newNode = newNode->left;
         // Case 5: newNode is left child and parent is right child
        } else if (grandParent->right == newNode->parent && newNode->parent->left == newNode) {
            rotateRight(&(*tree), newNode->parent);
            newNode = newNode->right;
        }
        newNode->parent->color = BLACK;
        grandParent->color = RED;
        if (grandParent->left == newNode->parent) {
            rotateRight(&(*tree), grandParent);
        } else {
            rotateLeft(&(*tree), grandParent);
        }
    }
    
}

struct Node *searchNode(struct Node *tree, int number) {
    if (tree == NULL) {
        return NULL;
    }
    if (number < tree->number) {
        return searchNode(tree->left, number);
    } else if (number > tree->number) {
        return searchNode(tree->right, number);
    } else {
        return tree;
    }
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




// Prints the struct node in-order
void printTree(struct Node *tree) {
    if (tree != NULL) {
        printTree(tree->left);
        printf("%d - color: %s\n", tree->number, tree->color == RED ? "RED" : "BLACK");
        printTree(tree->right);
    }
}

void printPreOrder(struct Node *tree) {
    if (tree != NULL) {
        printf("%d - color: %s\n", tree->number, tree->color == RED ? "RED" : "BLACK");
        printPreOrder(tree->left);
        printPreOrder(tree->right);
    }
}

void freeTree(struct Node *tree) {
    if (tree != NULL) {
        freeTree(tree->left);
        freeTree(tree->right);
        if (tree->parent != NULL) {
            if (tree->parent->left == tree) {
                tree->parent->left = NULL;
            } else {
                tree->parent->right = NULL;
            }
        }
        free(tree);
    }
}


int totalNodes(struct Node *tree) {
    if (tree == NULL) {
        return 0;
    }
    return 1 + totalNodes(tree->left) + totalNodes(tree->right);
}
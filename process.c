#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"


int freeTree (Node *root) {
    if (root == NULL )
        return 0;
    free(root->info);
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 0;
}

Node *traversal (Tree *tree, Node *ptr) {
    if (tree->root == NULL || (ptr && !ptr->left && !ptr->right))
        return NULL;
    if (ptr == NULL || tree->root->left) {
        if (ptr == NULL)
            return ptr = tree->root;
        ptr = tree->root->left;
        while (ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }
    if (ptr == tree->root) {
        ptr = tree->root->right;
        while (ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }
    if (ptr->right != NULL) {
        ptr = ptr->right;
        return ptr;
    }
    if (ptr->right == NULL && ptr->par->par == NULL) {
        ptr = ptr->par;
        return ptr;
    }
    if (ptr->par->par != NULL)
        ptr = ptr->par->par;
    return ptr;
}

int findByKey(Tree *tree, unsigned int key) {
    if (tree->root == NULL)
        return 0;
    Node *pos = NULL;
    for (int i = 0; i <= tree->tsize; i++) {
        pos = traversal(tree, pos);
        if (pos->key == key) {
            return 1;
        } else return 0;
    }
    return 0;
}

Node *maxElement (Tree *tree) {
    if (tree == NULL)
        return NULL;
    Node *pos = tree->root;
    while (pos != NULL) {
        if (pos->right)
            pos = pos->right;
    }
    return pos;
}

Node *minElement (Tree *tree) {
    if (tree == NULL)
        return NULL;
    Node *pos = tree->root;
    while (pos != NULL) {
        if (pos->left)
            pos = pos->left;
    }
    return pos;
}

Node *special (Tree *tree, unsigned int key) {
    if (tree == 0)
        return NULL;
    Node *max = maxElement(tree);
    Node *min = minElement(tree);
    Node *pos = NULL;
    for (int i = 0; i <= tree->tsize; i++) {
        pos = traversal(tree, pos);
        if (pos->key == key)
            break;
    }
    pos = (max->key - pos->key > pos->key - min->key) ? max : min;
    return pos;
}

char checkRootColor(Node *root) {
    if (root == NULL)
        return black;
    return (char) (root->color == red);
}

int rootColorFlip(Node *root) {
    if (root == NULL)
        return 1;
    root->color = !root->color;
    root->left->color = !root->left->color;
    root->right->color = !root->right->color;
    return 0;
}

Node *treeRotationLeft(Node *root) {
    if (root == NULL)
        return NULL;
    Node *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    temp->color = root->color;
    root->color = red;
    return temp;
}

Node *treeRotationRight(Node *root) {
    if (root == NULL)
        return NULL;
    Node *child = root->left;
    root->left = child->right;
    child->right = root;
    child->color = root->color;
    root->color = red;
    return child;
}

Node *newNode (unsigned int key, char *info, int color) {
    Node *node = calloc(1, sizeof(Node));
    node->key = key;
    node->info = info;
    node->color = color;
    return node;
}

Node *processAdd(Node *root, unsigned int key, char *info) {
    if (root == NULL)
        return newNode(key, info, red);
    if (root->key > key)
        root->left = processAdd(root->left, key, info);
    if (root->key < key)
        root->right = processAdd(root->left, key, info);
    if (checkRootColor(root->right) && !checkRootColor(root->left))
        root = treeRotationLeft(root);
    if (checkRootColor(root->left) && checkRootColor(root->left->left))
        root = treeRotationRight(root);
    if (checkRootColor(root->left) && checkRootColor(root->right))
        rootColorFlip(root);
    return root;
}

int addElement (Tree *tree, unsigned int key, char *info) {
    const char *mess = "Error: key was added recently";
    if (findByKey(tree, key) == 1) {
        puts(mess);
        return 1;
    }
    tree->root = processAdd(tree->root, key, info);
    tree->tsize++;
    return 0;
}

int readTree (FILE *fb, Tree *tree) {
    int key, info;
    if (tree == 0 || fb == 0)
        return 1;
    fread(&tree->tsize, sizeof(int), 1, fb);
    for (int i = 0; i <= tree->tsize; i++) {
        fread(&key, sizeof(int), 1, fb);
        fread(&info, sizeof(int), 1, fb);
        addElement(tree, key, info);
    }
    return 0;
}

Node *delLeft(Node *node) {
    if (node == NULL) {
        return NULL;
    }
    rootColorFlip(node);
    if (node->right != NULL && checkRootColor(node->right->left)) {
        node->right = treeRotationRight(node->right);
        node = treeRotationLeft(node);
        rootColorFlip(node);
    }
    return node;
}

Node *delRight(Node *node) {
    if (node == NULL) {
        return NULL;
    }
    rootColorFlip(node);
    if (node->left != NULL && checkRootColor(node->left->left)) {
        node = treeRotationRight(node);
        rootColorFlip(node);
    }
    return node;
}

Node *processDel(Node *root, unsigned int key) {
    if (root == NULL) {
        return NULL;
    }
    if (root->key > key) {
        if (root->left != NULL && !checkRootColor(root->left) && !checkRootColor(root->left->left)) {
            root = delLeft(root);
        }
        if (root != NULL) {
            root->left = processDel(key, root->left);
        }
    } else {
        if (checkRootColor(root->left)) {
            root = treeRotationRight(root);
        }
        if (root->key == key && root->right == NULL) {
            free(root->info);
            free(root);
            return NULL;
        }
        if (root->right != NULL && !checkRootColor(root->right) && !checkRootColor(root->right->left)) {
            root = delRight(root);
        }
        if (!strcmp(root->key, key)) {
            Node *next = minElement(root->right);
            root->key = next->key;
            free(root->info);
            root->info = next->info;
            root->right = processDel(root->right, root->right->key);
        }
        else {
            root->right = processDel(key, root->right);
        }
    }
    return root;
}

int delElement(Tree *tree, unsigned int key) {
    if (findByKey(tree, key) == 0 || tree->root == NULL) {
        return 1;
    }
    if (!checkRootColor(tree->root->left) && !checkRootColor(tree->root->right)) {
        tree->root->color = red;
    }
    tree->root = processDel(tree->root, key);
    if (tree->root != NULL) {
        tree->root->color = black;
    }
    return 0;
}

int printTree (Node *root, int p) {
    if (root->left)
        printTree(root->left, p + 5);
    for (int i = 0; i <= p; i++)
        printf(" ");
    printf("(%u %s)\n", root->key, root->info);
    if (root->right)
        printTree(root->right, p + 5);
}

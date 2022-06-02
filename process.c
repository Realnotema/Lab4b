#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int processWriteTree (Tree *tree, FILE *fb, Node *root, int headKey, int p) {
    if (root->left)
        processWriteTree(tree, fb, root->left, headKey, p + 5);
    if (root->key != headKey && root->par != NULL) {
        if (root->par != tree->root)
            fprintf(fb, "\t%d->%d\n", root->par->key, root->key);
    }
    if (root->right)
        processWriteTree(tree, fb, root->right, headKey, p + 5);
}

int processWriteColor (Tree *tree, FILE *fb, Node *root, int headKey, int p) {
    char *color = (root->color == 1) ? "red" : "black";
    fprintf(fb, "\t%d [fillcolor = %s]\n", root->key, color);
    if (root->left)
        processWriteColor(tree, fb, root->left, headKey, p + 5);
    if (root->right)
        processWriteColor(tree, fb, root->right, headKey, p + 5);
}

int writeTree (FILE *fb, Tree *tree) {
    if (tree == 0)
        return 1;
    fprintf(fb, "digraph GG {\n\t{\n");
    processWriteColor(tree, fb, tree->root, tree->root->key, 0);
    fprintf(fb, "\t}\n");
    processWriteTree(tree, fb, tree->root, tree->root->key, 0);
    fprintf(fb, "}");
    return 0;
}

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

void rotateToRight(Node *node) {
    Node *right = node->right;
    Node *par = node->par;
    node->right = par;
    if (par->par)
        node->par = par->par;
    else node->par = NULL;
    par->par = node;
    par->left = right;
    if (right != NULL)
        right->par = par;
}

void rotateToLeft (Node *node) {
    Node *left = node->left;
    Node *par = node->par;
    node->left = par;
    if (par->par)
        node->par = par->par;
    else node->par = NULL;
    par->par = node;
    par->right = left;
    if (left != NULL)
        left->par = par;
}

void swapColors(Node *node1, Node *node2) {
    if (node1 == NULL || node2 == NULL)
        return;
    int temp = node1->color;
    node1->color = node2->color;
    node2->color = temp;
}

int addElement (Tree *tree, unsigned int key, char *info) {
    Node *temp = (Node *) calloc(1, sizeof(Node));
    temp->key = key;
    temp->info = info;
    temp->color = red;
    if (tree->root == NULL) {
        tree->root = temp;
        tree->tsize++;
        return 0;
    }
    Node *ptr = tree->root;
    Node *par = NULL;
    while (ptr != NULL) {
        par = ptr;
        if (temp->key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }
    if (par->key == temp->key)
        return 1;
    if (temp->key < par->key)
        par->left = temp;
    else
        par->right = temp;
    temp->par = par;
    if (par->par && par->par->left && par->par->left->color == red) {
        if (par->par == tree->root)
            tree->root = par;
        rotateToRight(par->par->left);
    }
    if (par->right == temp) {
        if (par == tree->root)
            tree->root = temp;
        rotateToLeft(temp);
    }
    if (tree->root->color == red) {
        if (tree->root->left->color != red)
            swapColors(tree->root, tree->root->left);
        else tree->root->color = black;
    }
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

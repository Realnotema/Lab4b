#ifndef TREE_H
#define TREE_H

#define red 1
#define black 0

typedef struct Node {
    unsigned int key;
    char *info;
    struct Node *left, *right;
    int color;
} Node;

typedef struct Tree {
    Node *root;
    int tsize;
} Tree;

#endif TREE_H

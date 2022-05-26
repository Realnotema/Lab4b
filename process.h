#ifndef PROCESS_H
#define PROCESS_H

#include "tree.h"

int freeTree (Node *root);
int addElement (Tree *tree, unsigned int key, char *info);
int printTree (Node *root, int p);
Node *traversal (Tree *tree, Node *ptr);
int findByKey (Tree *tree, unsigned int key);
Node *special (Tree *tree, unsigned int key);
int delElement(Tree *tree, unsigned int key);
int readTree (FILE *fb, Tree *tree);

#endif

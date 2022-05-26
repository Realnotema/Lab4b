#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "input.h"
#include "process.h"

int dialog(const char *options[], int countOptions) {
    int choose = 0;
    for (int i = 0; i < countOptions; i++)
        puts(options[i]);
    char *errmess = "";
    do {
        puts(errmess);
        do {
            choose = getInt();
        } while (choose > countOptions || choose < 0);
        errmess = (choose < 0, choose > countOptions) ? "Incorrect input. Try again" : "";
    } while (choose < 0 || choose > countOptions);
    return choose;
}

int Menu (int choose, Tree *tree) {
    Node *pos = NULL;
    unsigned int key = 0;
    char *info = NULL;
    int flag = 0;
    char *mess[] = {"Enter your key", "Enter info", "No match key"};
    switch (choose) {
        case 1:
            puts(mess[0]);
            key = getInt();
            puts(mess[1]);
            info = getStr();
            addElement(tree, key, info);
            break;
        case 2:
            puts(mess[0]);
            key = getInt();
            delElement (tree, key);
            break;
        case 3:
            for (int i = 0; i <= tree->tsize; i++) {
                pos = traversal(tree, pos);
                if (pos != NULL)
                    printf("%d", pos->key);
            }
            printf("\n");
            break;
        case 4:
            puts(mess[0]);
            key = getInt();
            flag = findByKey (tree, key);
            if (flag == 1)
                puts(mess[2]);
            printf("\n");
            break;
        case 5:
            pos = special(tree, key);
            printf("(%d)\n", pos->key);
            break;
        case 6:
            printTree (tree->root, 0);
            break;
        case 7:
            puts("1. Add Element\n2. Find by key\n3. Delete element");
            timing(tree, getInt());
            break;
        case 8:
            puts("Enter name of file:");
            char *filename = getStr();
            FILE *fo = fopen(filename, "w");
            readTree(fo, tree);
            break;
        case 9:
            return 1;
    }
    return 0;
}

int exitCode (Tree *tree) {
    if (tree == NULL)
        return 0;
    if (tree->root == NULL)
        return 1;
    freeTree(tree->root);
    free(tree);
    return 1;
}

int main() {
    Tree *tree = (Tree *) calloc(1, sizeof(Tree));
    int flag = 0, choose = 0, n = 9;
    const char *options[] = {"1. Add element", "2. Delete element", "3. Tree traversal", "4. Find element by key", "5. Special find", "6. Print tree", "7. Timing", "8. Open file", "9. Quit"};
    do {
        flag = Menu(dialog(options, n), tree);
    } while (flag != 1);
    exitCode(tree);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"
#include "process.h"

int randTree (Tree *tree, int count, int arr[]) {
    for (int i = 0; i < count; i++) {
        int n = rand() % 1000;
        AddElement (tree, n, NULL);
    }
    return 0;
}

int timingFind (Tree *tree, int key) {
    Node *pos = tree->root;
    while (pos != NULL || pos->key == key) {
        if (pos->key > key)
            pos = pos->left;
        else
            pos = pos->right;
    }
}

int timing (Tree *tree, int n) {
    int randNum = 0;
    srand(time(NULL));
    clock_t time1;
    FILE *fb = fopen("timing", "w");
    fprintf(fb, "Count\tTime\n");
    for (int i = 100000; i <= 1000000; i += 100000) {
        int arr[i];
        double time = 0;
        for (int j = 0; j < 10; j++) {
            switch (n) {
                case 1:
                    time1 = clock();
                    randTree(tree, i, arr);
                    break;
                case 2:
                    randTree(tree, i, arr);
                    randNum = rand() % (i - 1) + 1;
                    time1 = clock();
                    timingFind(tree, randNum);
                    break;
                case 3:
                    randTree(tree, i, arr);
                    randNum = rand() % (i - 1) + 1;
                    delElement(tree, randNum);
                    break;
                case 4:
                    return 0;
            }
            clock_t time2 = clock();
            time += (double) (time2 - time1) / CLOCKS_PER_SEC;
        }
        fprintf(fb, "%d %lf\n", i, time);
    }
    fclose(fb);
}

#include <stdio.h>
#include <stdlib.h>
#include "kdtree.h"

#include <unistd.h>

#define NUM_ELEMENTS 100000



int
main(int argc, char** argv)
{
    printf("Starting program.\n");

    int points[NUM_ELEMENTS][DIMENSIONS];
    int i;

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        points[i][0] = i;
        points[i][1] = i;
    }

    KD_Tree* tree;
    tree = construct_kd_tree(points, NUM_ELEMENTS);
    free_tree(tree);

    for (i = 0; i < 10; i++)
    {
        tree = construct_kd_tree(points, NUM_ELEMENTS);
        int* dummy = calloc(1000000, sizeof(int));
        print_all_node_values(tree);
        free(dummy);
        free_tree(tree);
    }

    return 0;
}





















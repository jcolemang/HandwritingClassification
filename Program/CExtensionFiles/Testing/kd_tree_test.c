#include <stdio.h>
#include <stdlib.h>
#include "kdtree.h"

#include <unistd.h>

#define NUM_ELEMENTS 10



int
main(int argc, char** argv)
{
    printf("Starting program.\n");

    int points[NUM_ELEMENTS][DIMENSIONS];
    int i;

    int lower[] = {0, 0};
    int upper[] = {3, 4};

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        points[i][0] = i;
        points[i][1] = i;
    }

    KD_Tree* tree;
    tree = construct_kd_tree(points, NUM_ELEMENTS);

    printf("Num in bounds: %d\n", get_number_in_bounds(tree, lower, upper) );

    free_tree(tree);
    return 0;
}





















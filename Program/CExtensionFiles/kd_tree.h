#ifndef GLOBAL_H
    #include "global.h"
    #define GLOBAL_H 0
#endif

#ifndef STDIO_H
    #include <stdio.h>
    #define STDIO_H 0
#endif

#ifndef STDLIB_H
    #include <stdlib.h>
    #define STDLIB_H 0
#endif

#ifndef DYNAMIC_ARRAY_H
    #include "dynamic_array.h"
    #define DYNAMIC_ARRAY_H 0
#endif

#ifndef STRING_H
    #include <string.h>
    #define STRING_H
#endif


typedef struct KD_Node
{
    int split_dimension;
    int split_value;
    int is_a_leaf;
    int has_left_child;
    int has_right_child;
    struct KD_Node* left_child;
    struct KD_Node* right_child;
    void* value;
    int* location;
} KD_Node;


typedef struct KD_Tree 
{
    KD_Node* root;
    int num_dimensions;
} KD_Tree;

KD_Tree* construct_kd_tree(int[][DIMENSIONS], int);

DynamicArray* range_search(KD_Tree*, int[DIMENSIONS], int[DIMENSIONS]);

int* get_nearest_neighbor(KD_Tree*, int[DIMENSIONS]);

void free_tree(KD_Tree*);

void print_all_node_values( KD_Tree* );

int get_number_in_bounds( KD_Tree*, int[DIMENSIONS], int[DIMENSIONS] );

KD_Node* find_node( KD_Tree*, int[DIMENSIONS] );












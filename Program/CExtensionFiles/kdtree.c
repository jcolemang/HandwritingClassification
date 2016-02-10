#include <stdlib.h>
#include <stdio.h>
#include "kdtree.h"

#define UNDEFINED -1
#define SAMPLE_SIZE 50
#define LEFT 0
#define RIGHT 1


KD_Node* construct_kd_tree_helper(int[][DIMENSIONS], int, int);
int get_sample_median(int[][DIMENSIONS], int, int, int);
void insertion_sort(int*, int);
void print_array(int*, int);
void swap(int*, int, int);
KD_Node* find_node(KD_Node*, int*);
void free_nodes(KD_Node*);
void print_all_node_values_helper( KD_Node* );



/*
 * It is pretty important that I actually do this at some point.
 */
int**
range_search(KD_Tree tree, int** bounds)
{
    /* DIMENSIONS will be the length of bounds */ 
    return NULL;
}


/*
 * I think this is technically an approximation.
 * The rectangular search area makes things a bit
 * weird.
 */
int*
get_nearest_neighbor( KD_Tree* tree, int point[DIMENSIONS] )
{
    return (*find_node( (*tree).root, point )).value;
}


/*
 *
 */
get_nearest_neighbor_helper( KD_Node* current, int point[DIMENSIONS], int current_dimension)
{
    if ( (*current).has_left_child )
    return NULL;
}


/*
 * Basically a nearest neighbor helper method.
 */
KD_Node*
find_node( KD_Node* current, int* val_to_find )
{
    if ( (*current).is_a_leaf == 1)
    {
        return current;
    }

    if ( (*current).is_a_leaf != 0)
        printf("Problem. Leaf val: %d\n", (*current).is_a_leaf);

    int val = (*current).split_value;
    int split_dimension = (*current).split_dimension;

    if ( val_to_find[split_dimension] < val )
    {
        if ( (*current).has_left_child )
             return find_node( (*current).left_child, val_to_find );
        else if ( (*current).has_right_child )
             return find_node( (*current).right_child, val_to_find );
    }
    else
    {
        if ( (*current).has_right_child )
            return find_node( (*current).right_child, val_to_find );
        else if ( (*current).has_left_child )
            return find_node( (*current).left_child, val_to_find );
    }
    
    // This should never happen.
    return NULL;
}

/* 
 * Basically exclusively for sanity checks. This should find most
 * of the obvious memory errors.
 */
void
print_all_node_values( KD_Tree* tree )
{
    print_all_node_values_helper( (*tree).root ); 
}


void
print_all_node_values_helper( KD_Node* node )
{
    if ( (*node).is_a_leaf )
    {
        printf("(%d, %d)\n", (*node).value[0], (*node).value[1]);
    }
    if ( (*node).has_left_child )
    {
        print_all_node_values_helper( (*node).left_child );
    }
    if ( (*node).has_right_child )
    {
        print_all_node_values_helper( (*node).right_child );
    }

    if ( !(*node).has_left_child && !(*node).has_right_child && !(*node).is_a_leaf )
    {
        printf("\n\n\n\nWE HAVE A SERIOUS PROBLEM.\n\n\n\n");
    }
}



/*
 * I doubt I will ever actually use this.
 */
KD_Tree
get_sub_tree(KD_Node* node, int left_or_right)
{
    KD_Tree subtree;
    subtree.num_dimensions = DIMENSIONS;
    
    if (left_or_right == LEFT)
    {
        subtree.root = (*node).left_child;
    }
    else if (left_or_right == RIGHT)
    {
        subtree.root = (*node).left_child;
    }

    return subtree;
}


/*
 * The free() of my tree. recursively frees the nodes and their contents
 */
void
free_tree(KD_Tree* tree)
{
    free_nodes( (*tree).root);
    free(tree);
}


void
free_nodes(KD_Node* head)
{
    if ( (*head).has_left_child )
        free_nodes( (*head).left_child );

    if ( (*head).has_right_child )
        free_nodes( (*head).right_child );

    free( (*head).value );
    free(head);
}


/*
 * Insanely easy to implement and I only sort small lists. This will do.
 */
void
insertion_sort(int* array_to_sort, int num_elements)
{
    int i;
    int j;
    int temp;
    for (i = 1; i < num_elements; i++)
    {
        j = i;
        while ( (j > 0) && (array_to_sort[j] < array_to_sort[j-1]) )
        {
            temp = array_to_sort[j];
            array_to_sort[j] = array_to_sort[j-1];
            array_to_sort[j-1] = temp;
            j--;
        }
    }
}



/*
 * Taking sample medians saves the time needed to sort the entire list. 
 * This probably isn't all that significant for the small data sizes
 * I am using but its super cool so I'm doing it anyways.
 */
int
get_sample_median(int points[][DIMENSIONS], int list_size, int sample_size, int dim)
{
    int random_number;
    if (sample_size > list_size)
        sample_size = list_size;

    int sample[sample_size];

    int i;
    for (i = 0; i < sample_size; i++)
    {
        random_number = rand() % list_size; 
        sample[i] = points[random_number][dim];
    }
    
    insertion_sort(sample, sample_size);

    int median = sample[sample_size/2];

    return median;
}


/*
 * My tree's 'malloc'. Uses the points to make a 
 * dope tree.
 */
KD_Tree* construct_kd_tree(int points[][DIMENSIONS], int num_points)
{
    KD_Tree* tree = malloc(sizeof(KD_Tree));
    (*tree).num_dimensions = DIMENSIONS;
    (*tree).root = construct_kd_tree_helper(points, num_points, 0);
    return tree;
}


/*
 * The meat of my tree construction. Recursively splits the points (approximately)
 * in half until it reaches a leaf, then gives the node a value. 
 */
KD_Node* construct_kd_tree_helper( int points[][DIMENSIONS], int num_points, int split_dimension)
{
    KD_Node* node = malloc( sizeof(KD_Node) );
    int i;

    /* This must be a leaf */
    if (num_points == 1)
    {
        (*node).split_dimension = UNDEFINED;
        (*node).split_value = UNDEFINED;
        (*node).is_a_leaf = 1;
        (*node).has_left_child = 0;
        (*node).has_right_child = 0;
        (*node).left_child = NULL;
        (*node).right_child = NULL;
        (*node).value = malloc( DIMENSIONS * sizeof(int) );
        for (i = 0; i < DIMENSIONS; i++)
            (*node).value[i] = points[0][i];
        return node;
    }

    /* Not a leaf, so split the remaining data and recurse */
    (*node).is_a_leaf = 0;
    (*node).split_dimension = split_dimension;
    
    /* splitting the data */
    int median = get_sample_median(points, num_points, SAMPLE_SIZE, split_dimension);

    (*node).split_value = median;

    

    /* Setting up split arrays */
    int left_split_points[num_points][DIMENSIONS];
    int num_left_points = 0;
    int right_split_points[num_points][DIMENSIONS];
    int num_right_points = 0;
    int j;


    /*
    size_t row_pointer_bytes = num_points * sizeof(*left_split_points);
    size_t row_element_bytes = DIMENSIONS * sizeof(**left_split_points);
    left_split_points =  malloc( row_pointer_bytes + row_element_bytes * num_points );
    right_split_points = malloc( row_pointer_bytes + row_element_bytes * num_points );
    int* left_data_ptr = left_split_points + num_points;
    int* right_data_ptr = right_split_points + num_points;

    for ( j = 0; j < num_points; j++ )
    {
        left_split_points[j] = left_data_ptr + j * DIMENSIONS;
        right_split_points[j] = right_data_ptr + j * DIMENSIONS; 
    }
    */


    /* Actually splitting the data. */
    int current;

    for (i = 0; i < num_points; i++)
    {
        current = points[i][split_dimension];

        
        // NOTE The < is actually very important. Putting something equal
        // on the other side later could mess with accuracy.
        if (current < median)
        {
            for (j = 0; j < DIMENSIONS; j++)
                left_split_points[num_left_points][j] = points[i][j];
            num_left_points++;
        }
        else
        {
            for (j = 0; j < DIMENSIONS; j++)
                right_split_points[num_right_points][j] = points[i][j];
            num_right_points++;
        }
    }
        
    int new_split_dimension = (split_dimension + 1) % DIMENSIONS;

    if ( num_left_points > 0 ) 
    {
        (*node).has_left_child = 1;
        (*node).left_child = construct_kd_tree_helper(
                left_split_points, num_left_points, new_split_dimension);
    }
    else
    {
        (*node).has_left_child = 0;
        (*node).left_child = NULL;
    }

    if (num_right_points > 0)
    {
        (*node).has_right_child = 1;
        (*node).right_child = construct_kd_tree_helper(
                right_split_points, num_right_points, new_split_dimension);
    }
    else
    {
        (*node).has_right_child = 0;
        (*node).right_child = NULL;
    }
    
    if ( !(*node).is_a_leaf )
    {
        (*node).value = NULL;
    }

    return node;
}



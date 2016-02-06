#include <stdlib.h>
#include <stdio.h>
#include "kdtree.h"

#define SAMPLE_SIZE 10
#define DIMENSIONS 2

Node* construct_kd_tree_helper(int**, int, int);
int get_sample_median(int**, int, int, int);
void insertion_sort(int*, int);
void print_array(int*, int);
void swap(int*, int, int);
int** construct_2d_array(int, int);


void
print_array(int* array, int num_elements)
{
    int i;
    for (i = 0; i < num_elements; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("\n");
}


void
swap(int* array, int index1, int index2)
{
    int temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
}


void
insertion_sort(int* array_to_sort, int num_elements)
{
    int i;
    int j;
    for (i = 1; i < num_elements; i++)
    {
        j = i;
        while (j > 0 && array_to_sort[j] < array_to_sort[j-1])
        {
            swap(array_to_sort, j, j-1);
            j--;
        }
    }
}


int**
construct_2d_array(int num_rows, int num_cols)
{
    int** array;
    int i;

    array = (int**) malloc(num_rows*sizeof(int*));
    for (i = 0; i < num_rows; i++)
    {
        array[i] = (int*) malloc(num_cols*sizeof(int));        
    }

    return array;
}


int
get_sample_median(int** points, int list_size, int sample_size, int dim)
{
    int random_number;

    int* sample = (int*) malloc(sample_size * sizeof(int));
    
    int i;
    for (i = 0; i < sample_size; i++)
    {
        random_number = rand() % list_size; 
        sample[i] = points[random_number][dim];
    }

    insertion_sort(sample, sample_size);

    int median = sample[sample_size/2];
    free(sample);
    return median;
}


KD_Tree construct_kd_tree(int** points, int num_points)
{
    KD_Tree tree;
    tree.root = construct_kd_tree_helper(points, num_points, 0);
    return tree;
}


Node* create_node(void)
{
    Node* ptr;
    Node node;
    ptr = &node;
    return ptr;
}


Node* construct_kd_tree_helper( int** points, int num_points, int split_dimension)
{

    //printf("construct_kd_tree_helper method call.\n");
    //printf("Operating on...\n");

    Point point;
    Node* node = create_node();

    int i;
    for (i = 0; i < num_points; i++)
    {
        //printf("%d, %d\n", points[i][0], points[i][1]);
    }

    /* This must be a leaf */
    if (num_points == 1)
    {
        point.x = points[0][0];
        point.y = points[0][1];

        (*node).is_a_leaf = 1;
        (*node).value = point;

        //printf("Leaf: %d, %d\n", point.x, point.y);
        
        return node;
    }

    /* Not a leaf, so split the remaining data and recurse */
    (*node).is_a_leaf = 0;
    (*node).split_axis = split_dimension;
    
    /* splitting the data */
    int median = get_sample_median(points, num_points, SAMPLE_SIZE, split_dimension);

    //printf("Split value: %d\n", median);
    //printf("Split dimension: %d\n", split_dimension);

    (*node).split_value = median;

    int** left_split_points;
    int num_left_points = 0;
    int** right_split_points;
    int num_right_points = 0;

    /* num_points is overkill for this but I don't want to implement an arraylist. */
    left_split_points = construct_2d_array(num_points, DIMENSIONS);
    right_split_points = construct_2d_array(num_points, DIMENSIONS);

    int current;

    for (i = 0; i < num_points; i++)
    {
        current = points[i][split_dimension];

        if (current >= median)
        {
            right_split_points[num_right_points] = points[i]; 
            num_right_points++;
        }
        else
        {
            left_split_points[num_left_points] = points[i];
            num_left_points++;
        }
    }

    int new_split_dimension = (split_dimension + 1) % DIMENSIONS;

    if ( num_left_points != 0 ) 
    {
        (*node).has_left_node = 1;
        (*node).left_node = construct_kd_tree_helper(
                left_split_points, num_left_points, new_split_dimension);
    }
    else
    {
        (*node).has_left_node = 0;
    }

    if (num_right_points != 0)
    {
        (*node).has_right_node = 1;
        (*node).right_node = construct_kd_tree_helper(
                right_split_points, num_right_points, new_split_dimension);
    }
    else
    {
        (*node).has_right_node = 0;
    }
    
    return node;
}



#include "kd_tree.h"

#define UNDEFINED -69
#define SAMPLE_SIZE 50
#define LEFT 0
#define RIGHT 1


KD_Node* construct_kd_tree_helper(int[][DIMENSIONS], int, int);
int get_sample_median(int[][DIMENSIONS], int, int, int);
void insertion_sort(int*, int);
void print_array(int*, int);
void swap(int*, int, int);
void free_nodes(KD_Node*);
void print_all_node_values_helper( KD_Node* );
int get_num_in_bounds_helper(KD_Node*, int[DIMENSIONS], int[DIMENSIONS]);
void range_search_helper( KD_Node*, DynamicArray*, int[DIMENSIONS], int[DIMENSIONS] );
KD_Node* find_node_helper(KD_Node*, int[DIMENSIONS]);


KD_Node*
find_node(KD_Tree* tree, int location[DIMENSIONS])
{
    return find_node_helper( (*tree).root, location ); 
}


KD_Node*
find_node_helper(KD_Node* node, int loc[DIMENSIONS])
{
    if ( (*node).is_a_leaf )
        return node;

    int dim = (*node).split_dimension;

    if ( loc[dim] < (*node).split_value)
    {
        if ( (*node).has_left_child )
            return find_node_helper( (*node).left_child, loc ); 
    }

    if ( (*node).has_right_child )
        return find_node_helper( (*node).right_child, loc ); 

    printf("PROBLEM\n");
    return NULL;


}


/*
 * A poor attempt at something vaguely generic.
 * The idea is you could just edit the header 
 * file to change what gets put in the value of 
 * the node.
 *
 * This is called at the very end, so you
 * have access to the location and everything
 * else in the KD_Node struct.
 */
void
construct_kd_node_value( KD_Node* node )
{
    int i;
    DBScanPoint* dbpt;
    dbpt = malloc( sizeof(DBScanPoint) );

    for (i = 0; i < DIMENSIONS; i++)
        (*dbpt).location[i] = (*node).location[i]; 

    (*dbpt).classification = NOISE;
    (*dbpt).num_in_threshold = -1;
    (*dbpt).has_been_visited = 0;
    (*dbpt).points_in_threshold = NULL;

    (*node).value = dbpt;
}


/*
 * It is pretty important that I actually do this at some point.
 */
DynamicArray*
range_search(KD_Tree* tree, int lower[DIMENSIONS], int upper[DIMENSIONS])
{
    DynamicArray* nodes_in_bounds;
    nodes_in_bounds = create_dynamic_array(16);
    range_search_helper( (*tree).root, nodes_in_bounds, lower, upper );
    return nodes_in_bounds;
}


void 
range_search_helper(KD_Node* node, DynamicArray* arr, int lower[DIMENSIONS], int upper[DIMENSIONS])
{
    if ( (*node).is_a_leaf )
    {
        int i;
        for (i = 0; i < DIMENSIONS; i++)
        {
            if ( (*node).location[i] < lower[i] || (*node).location[i] > upper[i] )
                return;
        }

        dynamic_array_append( arr, (*node).value);
        
        return;
    }
  
    int dim = (*node).split_dimension;

    if ( (*node).has_left_child )
    {
        // checking for any overlap
        if ( (*node).split_value >= lower[dim])
            range_search_helper( (*node).left_child, arr, lower, upper );
    }

    if ( (*node).has_right_child )
    {
        if ( (*node).split_value <= upper[dim] ) 
            range_search_helper( (*node).right_child, arr, lower, upper );
    }

}


/*
 * 
 */
int
get_number_in_bounds(KD_Tree* tree, int lower[DIMENSIONS], int upper[DIMENSIONS])
{
    return get_num_in_bounds_helper( (*tree).root, lower, upper );
}


int 
get_num_in_bounds_helper(KD_Node* node, int lower[DIMENSIONS], int upper[DIMENSIONS])
{
    if ( (*node).is_a_leaf )
    {
        int i;
        for (i = 0; i < DIMENSIONS; i++)
        {
            if ( (*node).location[i] < lower[i] || (*node).location[i] > upper[i] )
                return 0;
        }

        return 1;
    }
  
    int num = 0;
    int dim = (*node).split_dimension;

    if ( (*node).has_left_child )
    {
        // checking for any overlap
        if ( (*node).split_value >= lower[dim])
            num += get_num_in_bounds_helper( (*node).left_child, lower, upper );
    }

    if ( (*node).has_right_child )
    {
        if ( (*node).split_value <= upper[dim] ) 
            num += get_num_in_bounds_helper( (*node).right_child, lower, upper );
    }

    return num;
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
        printf("(%d, %d)\n", (*node).location[0], (*node).location[1]);
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

    free( (*head).location );
    free(head);

    // Note that the value is not freed. This is 
    // dumb but really convenient for this program
    // and I'm running out of time.
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
        (*node).location = malloc( DIMENSIONS * sizeof(int) );
        memcpy( (*node).location, points[0], DIMENSIONS * sizeof(int) ) ;
        construct_kd_node_value(node);
        //printf("Point: (%d, %d)    Copied: (%d, %d)\n", points[0][0], points[0][1], (*node).location[0], (*node).location[1] );
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
        (*node).location = NULL;
    }

    return node;
}



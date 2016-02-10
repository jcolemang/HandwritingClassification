
#define DIMENSIONS 2

typedef struct Point
{
    int x;
    int y;
} Point;


typedef struct KD_Node
{
    int split_dimension;
    int split_value;
    int is_a_leaf;
    int has_left_child;
    int has_right_child;
    struct KD_Node* left_child;
    struct KD_Node* right_child;
    int* value;
} KD_Node;


typedef struct KD_Tree 
{
    KD_Node* root;
    int num_dimensions;
} KD_Tree;

KD_Tree* construct_kd_tree(int[][DIMENSIONS], int);

int** range_search(KD_Tree, int**);

int* get_nearest_neighbor(KD_Tree*, int[DIMENSIONS]);

void free_tree(KD_Tree*);

void print_all_node_values( KD_Tree* );




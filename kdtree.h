typedef struct Point Point;
struct Point
{
    int x;
    int y;
};


typedef struct Node Node;
struct Node
{
    int split_axis;
    int split_value;
    int is_a_leaf;
    int has_left_node;
    int has_right_node;
    Node* left_node;
    Node* right_node;
    Point value;
};


typedef struct KD_Tree KD_Tree;
struct KD_Tree 
{
    Node* root;
};

struct KD_Tree construct_kd_tree(int**, int);

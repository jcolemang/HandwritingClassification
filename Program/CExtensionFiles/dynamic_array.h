
#define FREE_ELEMENTS 0
#define LEAVE_ELEMENTS 1

typedef struct DynamicArray 
{
    unsigned int num_elements;
    unsigned int _capacity;
    void** _elements;
} DynamicArray;

int dynamic_array_append( DynamicArray*, void* );

void* dynamic_array_get_element(DynamicArray*, int);

DynamicArray* create_dynamic_array(int);

void destroy_dynamic_array(DynamicArray*, int);
void destroy_n_nested_dynamic_array( DynamicArray*, int, int );


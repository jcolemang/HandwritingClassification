
typedef struct DynamicArray 
{
    unsigned int num_elements;
    unsigned int _capacity;
    void** _elements;
} DynamicArray;

int dynamic_array_append( DynamicArray*, void* );

void* get_element(DynamicArray*, int);

DynamicArray* create_dynamic_array(int);

void destroy_dynamic_array(DynamicArray*);


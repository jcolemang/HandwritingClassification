#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

int double_size( DynamicArray* );

DynamicArray*
create_dynamic_array( int start_capacity )
{
    DynamicArray* arr = malloc( sizeof(DynamicArray) );
    (*arr)._capacity = start_capacity;
    (*arr)._elements = malloc( (*arr)._capacity * sizeof(void*) );
    if ((*arr)._elements == NULL)
        return NULL;
    (*arr).num_elements = 0;
    return arr;
}


void
destroy_dynamic_array( DynamicArray* arr, int free_elements)
{
    if (free_elements == FREE_ELEMENTS)
    {
        int i;
        for (i = 0; i < (*arr).num_elements; i++)
            free((*arr)._elements[i]); 
    }

    // freeing element pointers
    free((*arr)._elements);
}


void 
destroy_n_nested_dynamic_array( DynamicArray* arr, int num_nested, int free_elements )
{
    int j;
    if ( num_nested == 1 && free_elements == FREE_ELEMENTS )
    {
        for ( j = 0; j < (*arr).num_elements; j++ )
        {
            free( (*arr)._elements[j] );
        }

        //free(arr);
        return;
    }

    DynamicArray* element;
    for (j = 0; j < (*arr).num_elements; j++ )
    {
        element = dynamic_array_get_element( arr, j );
        if (element == NULL)
            printf("Might want to look at this.\n");
        else
        {
            destroy_n_nested_dynamic_array( element, num_nested-1, free_elements );
            //free(element);
            //free(arr);
        }

        
    }
        
    //printf("asd;flkj\n");
    //free( (*arr)._elements );


}


int
dynamic_array_append( DynamicArray* arr, void* thing )
{
   if ( (*arr)._capacity == (*arr).num_elements )      
   {
        if ( !double_size(arr) )
            return 0;
   }

   (*arr)._elements[ (*arr).num_elements ] = thing;
   ++(*arr).num_elements;
   return 1;
}


void*
dynamic_array_get_element( DynamicArray* arr, int index )
{
    if (index >= (*arr).num_elements || index < 0 )
    {
        printf("OUT OF BOUNDS\n");
        return NULL;
    }

    return (*arr)._elements[index];
}


int
double_size( DynamicArray* arr )
{
    void* new_mem = realloc( (*arr)._elements, 2*(*arr)._capacity*sizeof(void*) );
    if (new_mem == NULL)
        return 0;
    (*arr)._elements = new_mem;
    (*arr)._capacity *= 2;
    return 1;
}


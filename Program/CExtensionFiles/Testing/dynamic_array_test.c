#include <stdlib.h>
#include <stdio.h>
#include "dynamic_array.h"

int
main(int argc, char** argv)
{
    printf("Testing DynamicArray.\n");

    int num_to_append = 1000000000;

    printf("Creating array.\n");
    DynamicArray* array = create_dynamic_array(2);

    int i;
    for (i = 0; i < num_to_append; i++)
    {
        if (!dynamic_array_append( array, &i ))
        {
            printf("FAIL\n");
            destroy_dynamic_array(array);
        }
    }

    int** dummy = calloc(10000, sizeof(int));
    free(dummy);

    int* element;
    for (i = 0; i < num_to_append; i++)
    {
        element = (int*)get_element(array, i);
        if (*element != i)
        {
            printf("FAIL\n");
            destroy_dynamic_array(array);
            return 1;
        }
    }

    destroy_dynamic_array(array);

    return 0;
}



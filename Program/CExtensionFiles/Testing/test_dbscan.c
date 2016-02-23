
#include <stdlib.h>
#include <stdio.h>
#include "dbscan.h"

#define NUM_PTS 10

int
main(int argc, char** argv)
{
    printf("Start!\n");

    int points[NUM_PTS][DIMENSIONS];
    
    int i;
    int j;
    //int* dumm;

    for (i = 0; i < NUM_PTS; i++)
    {
        for (j = 0; j < DIMENSIONS; j++)
        {
            points[i][j] = i;
            //dumm = malloc(sizeof(int));
        }
    }

    printf("Entering dbscan.\n");
    DynamicArray* dummy;
    dummy = dbscan( points, NUM_PTS, 1, 1 );    
    printf("Leaving dbscan.\n");

    printf("Num clusters: %d\n", (*dummy).num_elements);

    printf("Freeing the main list of points\n");
    DynamicArray* all_points = dynamic_array_get_element( dummy, 0 );
    DBScanPoint* pt;
    DynamicArray* arr;
    for ( i = 0; i < NUM_PTS; i++ )
    {
        pt = dynamic_array_get_element( all_points, i );
        arr = (*pt).points_in_threshold;
        free( (*arr)._elements );
        free(arr); 
        free(pt);
    }

    free( (*all_points)._elements );
    free( all_points );

    printf("Doing this shit.\n");
    DynamicArray* inner_arr;
    for ( i = 1; i < (*dummy).num_elements; i++ )
    {
        inner_arr = dynamic_array_get_element( dummy, i );
        if (inner_arr == NULL)
            printf("GOD DAMNIT\n");

        /*
        for (j = 0; j < (*inner_arr).num_elements; j++)
        {
            free( dynamic_array_get_element( inner_arr, j ) );
        }
        */

        free( (*inner_arr)._elements );
        free( inner_arr );
    }

    free( (*dummy)._elements );
    free( dummy );

    printf("Now im doing this shit\n");
    //destroy_dynamic_array( dummy, LEAVE_ELEMENTS );

    //destroy_n_nested_dynamic_array( dummy, 2, LEAVE_ELEMENTS );
    printf("Done freeing shit.\n");




    return 0;
}



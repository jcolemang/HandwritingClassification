#include "dbscan.h"

void get_points_connected_to( DBScanPoint*, DynamicArray* );


void
destroy_DBScanPoint( DBScanPoint point )
{
    destroy_dynamic_array( point.points_in_threshold, FREE_ELEMENTS ); 
}


DynamicArray*
dbscan( int points[][DIMENSIONS], int num_points, 
        int dist_threshold, int num_threshold)
{
    printf("Helper function.\n");
    DBScanPoint* dbscan_points[num_points];
    KD_Tree* tree = construct_kd_tree(points, num_points);    
    int lower[DIMENSIONS];
    int upper[DIMENSIONS];
    
    DynamicArray* arr;
    KD_Node* current;

    printf("Initial pass\n");
    // Initial pass. Getting all of the core points.
    int i, j;
    for (i = 0; i < num_points; ++i)
    {
        for (j = 0; j < DIMENSIONS; j++)
        {
            lower[j] = points[i][j] - dist_threshold;
            upper[j] = points[i][j] + dist_threshold;
        }

        // I need the pointer to the node. These need to be modified in place.
        current = find_node( tree, points[i] );

        if ( (*current).location[0] != points[i][0] || (*current).location[1] != points[i][1] )
        {
            // This isn't really fatal but it will cause some real problems with accuracy
            printf("Found wrong node.");
        }

        // Setting all the range stuff. 
        dbscan_points[i] = (*current).value;
        arr = range_search( tree, lower, upper );
        (*dbscan_points[i]).points_in_threshold = arr;
        (*dbscan_points[i]).num_in_threshold = (*arr).num_elements;

        if ( (*dbscan_points[i]).num_in_threshold >= num_threshold)
            (*dbscan_points[i]).classification = CORE;
        else
            (*dbscan_points[i]).classification = NOISE;

    }
    
    free_tree(tree);
    
    /*
    printf("DBScan Points: ");
    for (i = 0; i < num_points; i++)
        printf("(%d, %d), ", (*dbscan_points[i]).location[0], (*dbscan_points[i]).location[1] );
    printf("\n");
    */


    printf("Actually clustering the points together.\n");

    DynamicArray* clusters = create_dynamic_array(8);
    if (!clusters)
        printf("PROBLEM.\n");
    DynamicArray* new_cluster;

    for (i = 0; i < num_points; ++i)
    {
        if ( !(*dbscan_points[i]).has_been_visited 
                && (*dbscan_points[i]).classification == CORE )
        {
            new_cluster = create_dynamic_array(256);
            dynamic_array_append( clusters, new_cluster );
            get_points_connected_to( dbscan_points[i], new_cluster ); 
        }

        // otherwise do nothing. Its been taken care of.
    }

    // I now have a dynamic array of dynamic arrays.

    printf("Done clustering.\n");

    return clusters;
}


void
get_points_connected_to( DBScanPoint* point, DynamicArray* cluster )
{
    //printf("In method with point (%d, %d)\n", (*point).location[0], (*point).location[1]);
    DynamicArray* connected_pts;
    int num = (*point).num_in_threshold;
    connected_pts = (*point).points_in_threshold;
    DBScanPoint* element;

    int i;
    for (i = 0; i < num; i++ )
    {
        element = dynamic_array_get_element(connected_pts, i);

        if (!element)
            printf("~~~~~~~~~~~Well fuck.~~~~~~~~~~~\n");

        if ( !(*element).has_been_visited )
        {
            (*element).has_been_visited = 1;
            dynamic_array_append( cluster, element );

            // This implicitly removes the noise. The lines
            // above add the border points.
            if ( (*element).classification == CORE )
                get_points_connected_to( element, cluster );

        }
        else
        {
            //printf("(%d, %d) has been visited.\n", (*element).location[0], (*element).location[1]);
        }
    } 
}



    

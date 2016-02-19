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
    DBScanPoint* dbscan_points[num_points];
    KD_Tree* tree = construct_kd_tree(points, num_points);    
    
    int lower[DIMENSIONS];
    int upper[DIMENSIONS];
    
    DynamicArray* arr;
    KD_Node* current;

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

        // Setting all the range stuff. 
        dbscan_points[i] = (*current).value;
        (*dbscan_points[i]).points_in_threshold = range_search( tree, lower, upper );
        arr = (*dbscan_points[i]).points_in_threshold; 
        (*dbscan_points[i]).num_in_threshold = (*arr).num_elements;

        if ( (*dbscan_points[i]).num_in_threshold >= num_threshold)
            (*dbscan_points[i]).classification = CORE;
        else
            (*dbscan_points[i]).classification = NOISE;

    }
    
    free_tree(tree);

    DynamicArray* clusters = create_dynamic_array(8);

    // This is to make it easier to free later.
    DynamicArray* all_points = create_dynamic_array(num_points);
    dynamic_array_append(clusters, all_points);

    DynamicArray* new_cluster;
    for (i = 0; i < num_points; ++i)
    {
        dynamic_array_append( all_points, dbscan_points[i] );

        if ( !(*dbscan_points[i]).has_been_visited 
                && (*dbscan_points[i]).classification == CORE )
        {
            new_cluster = create_dynamic_array(64);
            get_points_connected_to( dbscan_points[i], new_cluster ); 
            dynamic_array_append( clusters, new_cluster );

        }

        // otherwise do nothing. The point has been taken care of.
    }

    return clusters;
}


// I think this is basically a graph search.
// If there is a path from any point to 
// the original, it is going in the cluster.
void
get_points_connected_to( DBScanPoint* point, DynamicArray* cluster )
{
    DynamicArray* connected_pts;
    int num = (*point).num_in_threshold;
    connected_pts = (*point).points_in_threshold;
    DBScanPoint* element;

    int i;
    for (i = 0; i < num; i++ )
    {
        element = dynamic_array_get_element(connected_pts, i);

        if ( !(*element).has_been_visited )
        {
            (*element).has_been_visited = 1;
            dynamic_array_append( cluster, element );

            if ( (*element).classification == CORE )
                get_points_connected_to( element, cluster );

            // Noise is removed implicitly. 

        }
    } 
}



    

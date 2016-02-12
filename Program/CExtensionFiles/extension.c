#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "kdtree.h"

#define CORE 0
#define BORDER 1
#define NOISE 2


typedef struct DBScanPoint 
{
    int* location;
    int classification;
    int num_within_threshold;
    int visited;
    DynamicArray* connected_points;

} DBScanPoint;


void
destroy_DBScanPoint( DBScanPoint* point )
{
    destroy_dynamic_array( (*point).connected_points ); 
}


void
dbscan_helper( int points[][DIMENSIONS], int num_points, 
        int dist_threshold, int num_threshold)
{
    KD_Tree* tree = construct_kd_tree(points, num_points);    
    DBScanPoint dbscan_points[num_points];
    int lower[DIMENSIONS];
    int upper[DIMENSIONS];
    DynamicArray* arr;


    // Initial pass. Getting all of the core points.
    int i;
    for (i = 0; i < num_points; i++)
    {
        lower[0] = points[i][0] - dist_threshold;
        lower[1] = points[i][1] - dist_threshold;
        upper[0] = points[i][0] + dist_threshold;
        upper[1] = points[i][1] + dist_threshold;
        arr = range_search( tree, lower, upper );

        // neither of these should change so this shouldn't
        // be a problem. Watch out for memeory though!!!
        dbscan_points[i].location = points[i];

        if ( (*arr).num_elements >= num_threshold )
            dbscan_points[i].classification = CORE;
        else
            dbscan_points[i].classification = NOISE;

        dbscan_points[i].connected_points = arr;
        dbscan_points[i].num_within_threshold = (*arr).num_elements;
        dbscan_points[i].visited = 0;
    }

    // TODO:
    // Go through the points again.
    // if the point is a core and hasn't been visited, put it in a new cluster
    // go through all of its connected points, mark them as visited, 
    // and put them in the cluster.
    // Expand each of these nodes, doing the same to their neighbors.
    // if the point has been visited or is not core, skip it.
    // I will need a new method for this.

    printf("Done\n");

    for (i = 0; i < num_points; i++)
        destroy_DBScanPoint( &dbscan_points[i] );
    free_tree(tree);

}


/* Sole purpose is a wrapper function. */
static PyObject* 
dbscan(PyObject* self, PyObject* args)
{
    PyObject* list;
    double threshold_dist;
    int threshold_num;
    int i;

    /* Parsing the list */
    if ( !PyArg_ParseTuple(args, "Oid", &list, &threshold_num, &threshold_dist) )
    {
       return NULL;  
    }

    /* Preparing my 'point' container */
    int list_length = PyList_Size(list);
    int points[list_length][DIMENSIONS];

    /* Parsing the tuples in the list into pairs of integers */
    /* Quite a lot of overhead, but it is better than using python. I think. */
    int x, y, num_points = 0;
    PyObject* tuple;
    for (i = 0; i < list_length; i++)
    {
        tuple = PyList_GetItem(list, i);
        
        if (!PyArg_ParseTuple(tuple, "ii", &x, &y))
        {
            return Py_BuildValue("i", 1);
        }

        points[i][0] = x;
        points[i][1] = y;
        num_points++;
    }

    /* The actual scanning */
    dbscan_helper( points, num_points, threshold_dist, threshold_num );

    return Py_BuildValue("i", 0);
}


static PyMethodDef ExtensionMethods[] =
{
    {"dbscan", dbscan, METH_VARARGS, "Run dbscan"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
initdbscan(void)
{
    (void) Py_InitModule("dbscan", ExtensionMethods);
}

    

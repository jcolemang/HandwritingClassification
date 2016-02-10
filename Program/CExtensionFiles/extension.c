#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "kdtree.h"

#define NOISE 0
#define BORDER 1
#define CORE 2


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
    
    /* Now lets actually do some scanning */
    KD_Tree* tree = construct_kd_tree(points, num_points);    

    int pt_to_find[] = {100, 100};

    //print_all_node_values(tree);

    int* nearest_neighbor = get_nearest_neighbor( tree,  pt_to_find );
    printf("(%d, %d) is the nearest to (%d, %d)\n", 
            nearest_neighbor[0], nearest_neighbor[1], pt_to_find[0], pt_to_find[1]);

    free_tree(tree);
    return Py_BuildValue("i", 0);
}


static PyMethodDef DBScanMethods[] =
{
    {"dbscan", dbscan, METH_VARARGS, "Run dbscan"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
initdbscan(void)
{
    (void) Py_InitModule("dbscan", DBScanMethods);
}

    

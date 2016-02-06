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

    printf("Calling c dbscan.\n");

    PyObject* list;
    double threshold_dist;
    int threshold_num;


    /* Parsing the list */
    if ( !PyArg_ParseTuple(args, "Oid", &list, &threshold_num, &threshold_dist) )
    {
       return NULL;  
    }



    /* Preparing my 'point' container */
    int** points; 
    int list_length = PyList_Size(list);
    int num_columns = 2;
    int num_rows = list_length;

    points = (int**) malloc(num_rows * sizeof(int*));
    
    int i;
    for (i = 0; i < num_rows; i++)
        points[i] = (int*) malloc(num_columns * sizeof(int));

    //printf("All points:\n");

    /* Parsing the tuples in the list into pairs of integers */
    int x, y, num_points = 0;
    PyObject* tuple;
    for (i = 0; i < list_length; i++)
    {
        tuple = PyList_GetItem(list, i);
        
        if (!PyArg_ParseTuple(tuple, "ii", &x, &y))
        {
            free(points);
            return NULL;
        }

        points[i][0] = x;
        points[i][1] = y;
        num_points++;
        //printf("%d, %d\n", points[i][0], points[i][1]);
    }

    //printf("Thresholds: %d, %f\n", threshold_num, threshold_dist);

    /* Now lets actually do some scanning */
    printf("Calling kd tree method.\n");
    KD_Tree tree = construct_kd_tree(points, num_points);    
    printf("Returned from kd tree method.\n");
    

    free(points);
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

    

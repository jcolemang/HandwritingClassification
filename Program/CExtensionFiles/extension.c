#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "dbscan.h"

// Note this is also defined in kd_tree.h
#define DIMENSIONS 2

/* Sole purpose is a wrapper function. */
static PyObject* 
dbscan_wrapper(PyObject* self, PyObject* args)
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

    printf("Num points: %d\n", list_length);

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
    DynamicArray* clusters = dbscan( points, num_points, threshold_dist, threshold_num );

    // Turning the result into something python can use.
    
    int j;
    int k;
    DynamicArray* cluster;
    PyObject* py_clusters = PyList_New( (*clusters).num_elements );
    Py_INCREF(py_clusters);
    PyObject* py_cluster;
    PyObject* pt;
    DBScanPoint* element;

    for (i = 0; i < (*clusters).num_elements; i++)
    {
        cluster = dynamic_array_get_element( clusters, i );
        py_cluster = PyList_New( (*cluster).num_elements );
        Py_INCREF(py_cluster);

        for (j = 0; j < (*cluster).num_elements; j++ )
        {
            pt = PyTuple_New(DIMENSIONS);             
            Py_INCREF(pt);
            element = dynamic_array_get_element(cluster, j);

            for (k = 0; k < DIMENSIONS; k++)
            {
                PyTuple_SetItem(pt, k, Py_BuildValue( "i", (*element).location[k] ));
            }

            PyList_SetItem( py_cluster, j, pt );
            
        }

        PyList_SetItem(py_clusters, i, py_cluster);;
        
    }


    destroy_dynamic_array(clusters, FREE_ELEMENTS);
    return py_clusters;
}


static PyMethodDef ExtensionMethods[] =
{
    {"dbscan", dbscan_wrapper, METH_VARARGS, "Run dbscan"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
initdbscan(void)
{
    (void) Py_InitModule("dbscan", ExtensionMethods);
}

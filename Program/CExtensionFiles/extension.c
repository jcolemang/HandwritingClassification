#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "dbscan.h"


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

    /* The actual scanning */
    DynamicArray* clusters = dbscan( points, num_points, threshold_dist, threshold_num );

    // Turning the result into something python can use.
    
    int j;
    int k;
    DynamicArray* cluster;
    // Element 0 isn't a cluster so there are one too many elements in the dynamic array.
    PyObject* py_clusters = PyList_New( (*clusters).num_elements - 1 );
    PyObject* py_cluster;
    PyObject* pt;
    DBScanPoint* element;

    // I use element 0 to store all the elements to make it 
    // easy to free them.
    for (i = 1; i < (*clusters).num_elements; i++)
    {
        cluster = dynamic_array_get_element( clusters, i );
        py_cluster = PyList_New( (*cluster).num_elements );

        for (j = 0; j < (*cluster).num_elements; j++ )
        {
            pt = PyTuple_New(DIMENSIONS);             
            element = dynamic_array_get_element(cluster, j);

            for (k = 0; k < DIMENSIONS; k++)
            {
                PyTuple_SetItem(pt, k, Py_BuildValue( "i", (*element).location[k] ));
            }

            PyList_SetItem( py_cluster, j, pt );
            
        }

        PyList_SetItem(py_clusters, i-1, py_cluster);
        
    }

    // Freeing everything. This is a sign of awful design and I
    // apologize for it but I am very, very new to c and 
    // did not imagine this would be as hard as it is.
    DynamicArray* all_points = dynamic_array_get_element( clusters, 0 );
    DBScanPoint* pt_to_free;
    DynamicArray* arr;
    for ( i = 0; i < num_points; i++ )
    {
        pt_to_free = dynamic_array_get_element( all_points, i );
        arr = (*pt_to_free).points_in_threshold;
        free( (*arr)._elements );
        free(arr); 
        free(pt_to_free);
    }

    free( (*all_points)._elements );
    free( all_points );

    DynamicArray* inner_arr;
    for ( i = 1; i < (*clusters).num_elements; i++ )
    {
        inner_arr = dynamic_array_get_element( clusters, i );
        free( (*inner_arr)._elements );
        free( inner_arr );
    }

    free( (*clusters)._elements );
    free( clusters );

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

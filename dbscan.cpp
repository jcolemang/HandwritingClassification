#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


static PyObject* 
dbscan(PyObject* self, PyObject* args)
{
    printf("Hello, world!\n");
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

    

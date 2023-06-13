#include "main.h"


// Module methods
static PyMethodDef own_methods[] =
{
    {
        "new_number",
        new_number_from_py,
        METH_VARARGS,
        "A new number in the specified number system"
    },
    {NULL, NULL, 0, NULL}
};


// Module definition
static PyModuleDef simple_module =
{
    PyModuleDef_HEAD_INIT,
    "more_numbers",
    "documentation",
    -1,
    own_methods
};


// Module initialization
PyMODINIT_FUNC PyInit_more_numbers(void)
{
    PyObject* module = PyModule_Create(&simple_module);
    if (module==NULL)
        return NULL;

    my_number_Type.tp_new = PyType_GenericNew;
    if(PyType_Ready(&my_number_Type) < 0)
    	return NULL;
    Py_INCREF(&my_number_Type);

    return module;
}

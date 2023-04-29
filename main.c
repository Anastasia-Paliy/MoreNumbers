#include "main.h"

// методы модуля
static PyMethodDef own_methods[] = {
    {
        "new_number",
        new_number_from_py,
        METH_VARARGS,
        "A new number in the specified number system"
    },
    {NULL, NULL, 0, NULL}
};


static PyModuleDef simple_module = {
    PyModuleDef_HEAD_INIT,
    "more_numbers",
    "documentation",
    -1,
    own_methods
};


// инициализация модуля
PyMODINIT_FUNC PyInit_more_numbers(void)
{
    PyObject* module;
    module = PyModule_Create(&simple_module);
    if (module==NULL)
        return NULL;

    my_number_Type.tp_new = PyType_GenericNew;
    if(PyType_Ready(&my_number_Type) < 0)
    	return NULL;
    Py_INCREF(&my_number_Type);

    return module;
}

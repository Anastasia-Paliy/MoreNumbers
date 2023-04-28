#include "main.h"

PyObject* new_number_from_py(PyObject* self, PyObject* args) {
    Py_INCREF(Py_None);
    return Py_None;
}

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

PyTypeObject my_number_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "number",
    .tp_basicsize = 0,
    .tp_dealloc = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A new number in the specified number system",
    .tp_methods = 0,
    .tp_as_number = 0,
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



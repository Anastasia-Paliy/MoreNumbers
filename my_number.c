#include "my_number.h"

PyObject* new_number_from_py(PyObject* self, PyObject* args) {
    Py_INCREF(Py_None);
    return Py_None;
}

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

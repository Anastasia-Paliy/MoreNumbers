#ifndef MN_H
#define MN_H
#include "Python.h"

PyObject* new_number_from_py(PyObject* self, PyObject* args);
PyTypeObject my_number_Type;

#endif // MN_H

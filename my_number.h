#ifndef MN_H
#define MN_H
#include "Python.h"


typedef struct {
    PyObject_HEAD
    int base;
    int *number;
    int length;
    int sign;
} my_number;


static void *get_array_of_digits(PyObject *str, int base, my_number* a);
PyObject* new_number_from_py(PyObject* self, PyObject* args);
void delete_my_number(my_number* self);
PyObject* my_number_repr(PyObject* self);
int my_number_comparator(my_number* a, my_number* b);
PyObject* my_number_tp_richcompare(PyObject *self, PyObject *other, int op);
PyObject* _my_number_add(my_number* a, my_number* b);
PyObject* my_number_absolute(PyObject* self);
PyObject* my_number_negative(PyObject* self);
my_number* _my_number_sub(my_number* a, my_number* b);
PyObject* my_number_add(PyObject* self, PyObject* another);
PyObject* my_number_sub(PyObject* self, PyObject* another);


PyTypeObject my_number_Type;

#endif // MN_H

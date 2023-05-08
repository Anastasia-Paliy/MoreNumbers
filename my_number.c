#include "my_number.h"


PyObject* new_number_from_py(PyObject* self, PyObject* args) {
    my_number* a = PyObject_NEW(my_number, &my_number_Type);
    PyObject *obj, *str;
    int base;
    long length;
    if (!PyArg_ParseTuple(args, "Oi", &obj, &base)) {
        printf("Неверные аргументы функции");
        return NULL;
    }
    const char* st = obj->ob_type->tp_name;
    if (strcmp("int", st)!=0) {
        printf("Первый аргумент - int");
        return NULL;
    }
    str = PyObject_Repr(obj);
    length =  PyObject_Length(str);
    printf("%d\n", length);
    a->base = base;

    return (PyObject*)a;
}

void delete_my_number(my_number* self) {
    Py_XDECREF(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject my_number_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "number",
    .tp_basicsize = 0,
    .tp_dealloc = delete_my_number,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A new number in the specified number system",
    .tp_methods = 0,
    .tp_as_number = 0,
};

#include "my_number.h"

static PyMethodDef number_methods[] = {
    {NULL, NULL, 0, NULL}
};

static int *get_array_of_digits(PyObject *str, int base) {
    long length =  PyObject_Length(str);
    int *array = (int*)malloc(sizeof(int) * length);
    for (int i = 0; i < length; ++i) {
        PyObject *pItem = PyObject_GetItem(str, Py_BuildValue("i", i));
        PyObject *encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
        char* s = PyBytes_AsString(encodedString);
        array[i] = s[0] - 48;
    }
    return array;
}

PyObject* new_number_from_py(PyObject* self, PyObject* args) {
    my_number* a = PyObject_NEW(my_number, &my_number_Type);
    PyObject *obj, *str;
    int base;
    if (!PyArg_ParseTuple(args, "Oi", &obj, &base)) {
        printf("Неверные аргументы функции");
        return NULL;
    }
    const char* st = obj->ob_type->tp_name;
    if (strcmp("int", st) != 0) {
        printf("Первый аргумент - int");
        return NULL;
    }
    if (base <= 1 || base > 16) {
        printf("Второй аргумент - число от 0 до 16 включительно");
        return NULL;
    }
    str = PyObject_Repr(obj);
    a->base = base;
    a->number = get_array_of_digits(str, base);
    a->length = PyObject_Length(str);
    return (PyObject*)a;
}

void delete_my_number(my_number* self) {
    free(self->number);
    Py_XDECREF(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* my_number_repr(PyObject* self)
{
    my_number* a = (my_number*)self;
    char *str = (char*)malloc(sizeof(char) * (a->length));
    for (int i = 0; i < a->length; ++i) {
        if (a->number[i] < 10) {
            str[i] = a->number[i] + 48;
        } else {
            str[i] = a->number[i] - 10 + 65;
        }
    }

    PyObject *repr = PyUnicode_FromString(str);
    free(str);
    return repr;
}

PyTypeObject my_number_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "number",
    .tp_basicsize = sizeof(my_number),
    .tp_dealloc = delete_my_number,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A new number in the specified number system",
    .tp_methods = number_methods,
    .tp_repr = my_number_repr,
    .tp_as_number = 0,
};

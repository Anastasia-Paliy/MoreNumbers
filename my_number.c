#include "my_number.h"

static int *dividing_long_by_short(int *array, int length, int b, int base);

static PyMethodDef number_methods[] = {
    {NULL, NULL, 0, NULL}
};

static void extend_array(int **array, int length) {
    int *array1 = *array;
    int *array2 = (int*)malloc(sizeof(int) * length * 2);
    for (int i = 0; i < length; ++i) {
        array2[i] = array1[i];
    }

    free(array1);
    *array = array2;
}

static int *dividing_long_by_short(int *array, int length, int b, int base, int *new_len) {
    int *array2 = (int*)malloc(sizeof(int) * length);
    int i = 0, size = length, c = 0;
    while (c < length) {
        int carry = 0;
        for (int j = 0; j < length; ++j) {
            int cur = array[j] + carry * base;
            array[j] = cur / b;
            carry = cur % b;
        }
        while (array[c] == 0) c++;


        if (i >= size) {
            extend_array(&array2, size);
            size *= 2;
        }

        array2[i] = carry;
        i++;
    }
    int *res = (int*)malloc(sizeof(int) * i);
    for (int j = 0; j < i; ++j) {
        res[i - 1 - j] = array2[j];
    }
    *new_len = i;
    free(array2);
    return res;
}

static int *get_array_of_digits(PyObject *str, int base, int *new_len) {
    long length =  PyObject_Length(str);
    int *array = (int*)malloc(sizeof(int) * length);
    for (int i = 0; i < length; ++i) {
        PyObject *pItem = PyObject_GetItem(str, Py_BuildValue("i", i));
        PyObject *encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
        char* s = PyBytes_AsString(encodedString);
        array[i] = s[0] - 48;
    }
    int* res = dividing_long_by_short(array, length, base, 10, new_len);
    return res;
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
    a->number = get_array_of_digits(str, base, &(a->length));
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
    char *str = (char*)malloc(sizeof(char) * (a->length + 1));
    for (int i = 0; i < a->length; ++i) {
        if (a->number[i] < 10) {
            str[i] = a->number[i] + 48;
        } else {
            str[i] = a->number[i] - 10 + 65;
        }
    }
    str[a->length] = '\0';
    PyObject *repr = PyUnicode_FromString(str);
    free(str);
    return repr;
}

static PyNumberMethods my_number_as_number = {
    .nb_add = 0,
    .nb_negative = 0,
    .nb_subtract = 0,
    .nb_multiply = 0,
    .nb_true_divide = 0,
};

PyTypeObject my_number_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "number",
    .tp_basicsize = sizeof(my_number),
    .tp_dealloc = delete_my_number,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "A new number in the specified number system",
    .tp_methods = number_methods,
    .tp_repr = my_number_repr,
    .tp_as_number = &my_number_as_number,
};

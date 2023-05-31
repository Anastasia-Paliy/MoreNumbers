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

static void *get_array_of_digits(PyObject *str, int base, my_number* a) {
    long length =  PyObject_Length(str);
    PyObject *pItem = PyObject_GetItem(str, Py_BuildValue("i", 0));
    PyObject *encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
    char* s = PyBytes_AsString(encodedString);
    int *array;
    int bo = (s[0]=='-');
    array = (int*)malloc(sizeof(int) * (length-bo));
    for (int i = 0; i < length-bo; ++i) {
        pItem = PyObject_GetItem(str, Py_BuildValue("i", i+bo));
        encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
        s = PyBytes_AsString(encodedString);
        array[i] = s[0] - 48;
    }
    a->sign = bo ? -1 : 1;
    a->number = dividing_long_by_short(array, length-bo, base, 10, &(a->length));
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
    get_array_of_digits(str, base, a);
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
    char *str;
    int i0 = (a->sign != 1);
    str = (char*)malloc(sizeof(char) * (a->length + 1 + i0));
    if (i0) {
        str[0] = '-';
    }
    str[a->length+i0] = '\0';
    for (int i = 0; i < a->length; ++i) {
        if (a->number[i] < 10) {
            str[i+i0] = a->number[i] + 48;
        } else {
            str[i+i0] = a->number[i] - 10 + 65;
        }
    }
    PyObject *repr = PyUnicode_FromString(str);
    free(str);
    return repr;
}

PyObject* _my_number_add(my_number* a, my_number* b) {
    my_number* c = PyObject_NEW(my_number, &my_number_Type);
    int al = a->length, bl = b->length;
    int len = (al > bl ? al : bl) + 1, base = a->base;
    int *array = (int*)malloc(sizeof(int) * len);
    array[0] = 0;
    int l = 0;
    int carry = 0;
    for (int i = 0; i < len; ++i) {
        int t = ((al-1-i) < 0 ? 0 : a->number[al-1-i]) + ((bl-1-i) < 0 ? 0 : b->number[bl-1-i]) + carry;
        array[len - 1 - i] = (t % base);
        carry = t / base;
    }
    if (array[0] == 0) {
        len--;
        int *array2 = (int*)malloc(sizeof(int) * len);
        for (int i = 0; i < len; ++i) {
            array2[i] = array[1 + i];
        }
        free(array);
        array = array2;
    }
//    for (int i = 0; i < len; ++i) {
//        printf("%d\n",array[i]);
//    }
    c->number = array;
    c->length = len;
    c->sign = 1;
    c->base = base;
    return c;
}

my_number* _my_number_sub(my_number* a, my_number* b) {


}

PyObject* my_number_add(PyObject* self, PyObject* another) {
    my_number *a = (my_number*)self, *b = (my_number*)another;
    if (a->base != b->base) {
        printf("Основания должны быть одинаковые\n");
        return NULL;
    }
    if (a->sign > 0 && b->sign > 0)
        return _my_number_add(a, b);
    if (a->sign > 0 && b->sign < 0)
        return _my_number_sub(a, b);
    if (a->sign < 0 && b->sign > 0) {
        my_number* c = _my_number_sub(b, a);
        c->sign = -1;
        return c;
    }
    if (a->sign < 0 && b->sign < 0) {
        printf("assdfsd\n");
        my_number* c = _my_number_add(a, b);
        c->sign = -1;
        return c;
    }
}

static PyNumberMethods my_number_as_number = {
    .nb_add = my_number_add,
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

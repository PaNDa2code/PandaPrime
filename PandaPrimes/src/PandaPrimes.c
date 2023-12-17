#include <Python.h>
#include <primesieve.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <stdio.h>
// Definition for primes_range

    typedef struct
{
    PyObject_HEAD
        primesieve_iterator it;
    size_t start, end;
} primes_range;

static PyObject *primes_range_new(PyTypeObject *type, PyObject *args, PyObject *kwarg)
{
    unsigned long long start, end;

    // Check the number of arguments
    if (PyTuple_Size(args) == 2)
    {
        // Attempt to parse two integers
        if (!PyArg_ParseTuple(args, "KK", &start, &end))
        { // Use "KK" for unsigned long long
            PyErr_SetString(PyExc_TypeError, "Invalid argument types. Expected two integers.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        // Attempt to parse a single integer
        if (!PyArg_ParseTuple(args, "K", &end))
        { // Use "K" for unsigned long long
            PyErr_SetString(PyExc_TypeError, "Invalid argument type. Expected a single integer.");
            return NULL;
        }

        // Handle the case where start is not provided (set it to a default value, for example)
        start = 0;
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Invalid number of arguments.");
        return NULL;
    }

    primes_range *gen = (primes_range *)type->tp_alloc(type, 0);

    if (!gen)
    {
        return NULL;
    }

    gen->start = start;
    gen->end = end;
    primesieve_init(&gen->it);
    primesieve_jump_to(&gen->it, start, end);

    return (PyObject *)gen;
}

static void primes_range_dealloc(primes_range *gen)
{
    primesieve_clear(&gen->it);
    Py_TYPE(gen)->tp_free((PyObject *)gen);
}

static PyObject *primes_range_next(primes_range *gen)
{
    size_t prime = primesieve_next_prime(&gen->it);
    if (prime <= gen->end)
    {
        return Py_BuildValue("n", prime);
    }
    else
    {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
}

static PyObject *primes_range_iter(primes_range *gen)
{
    Py_INCREF(gen);
    return (PyObject *)gen;
}

static PyMethodDef primes_range_methods[] = {
    {"next_prime", (PyCFunction)primes_range_next, METH_NOARGS, "Get the next prime in the range."},
    {NULL, NULL, 0, NULL}};

static PyTypeObject primes_rangeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "primes_range",
    .tp_basicsize = sizeof(primes_range),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)primes_range_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "`primes_range` object is an object like Python's built-in range but iterates only over prime numbers.",
    .tp_iter = (getiterfunc)primes_range_iter,
    .tp_iternext = (iternextfunc)primes_range_next,
    .tp_methods = primes_range_methods,
    .tp_new = primes_range_new,
};

// Definition for Iterator

typedef struct
{
    PyObject_HEAD
        primesieve_iterator it;
} Iterator;

static PyObject *Iterator_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Iterator *_it = (Iterator *)type->tp_alloc(type, 0);

    if (!_it)
    {
        return NULL;
    }
    primesieve_init(&_it->it);

    return (PyObject *)_it;
}

static void Iterator_dealloc(Iterator *_it)
{
    primesieve_clear(&_it->it);
    Py_TYPE(_it)->tp_free((PyObject *)_it);
}

static PyObject *Iterator_next(Iterator *_it)
{
    u_int64_t next_prime = primesieve_next_prime(&_it->it);
    return PyLong_FromLongLong(next_prime);
};

static PyObject *Iterator_prev(Iterator *_it)
{
    u_int64_t next_prime = primesieve_prev_prime(&_it->it);
    return PyLong_FromLongLong(next_prime);
};

static PyObject *Iterator_jump_to(Iterator *_it, PyObject *args)
{   
    u_int64_t jump_to;
    if(PyTuple_Size(args) != 1)
    {
        PyErr_SetString(PyExc_TypeError, "jump_to method takes one argumnt");
        return NULL;
    }
    if(!PyArg_ParseTuple(args, "K", &jump_to))
    {
        PyErr_SetString(PyExc_TypeError, "jump_to method takes one argumnt");
        return NULL;
    }

    primesieve_jump_to(&_it->it, jump_to, primesieve_get_max_stop());

    Py_DECREF(Py_None);
    return Py_None;
}

static PyMethodDef Iterator_methods[] = {
    {"next_prime", (PyCFunction)Iterator_next, METH_NOARGS, "Get the next prime."},
    {"prev_prime", (PyCFunction)Iterator_prev, METH_NOARGS, "Get the previous prime."},
    {"jump_to", (PyCFunction)Iterator_jump_to, METH_VARARGS, "Get the previous prime."},
    {NULL, NULL, 0, NULL}};

static PyTypeObject IteratorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "Iterator",
    .tp_basicsize = sizeof(Iterator),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)Iterator_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Iterator doc",
    .tp_new = Iterator_new,
    .tp_methods = Iterator_methods,
};

int get_bit_size(u_int64_t integer)
{
    if (integer <= 65535)
    {
        return 16;
    }
    else if (integer <= 4294967295)
    {
        return 32;
    }
    else if (integer <= 18446744073709551615)
    {
        return 64;
    }
    return 64;
}

int get_np_type(int bit_size)
{
    if (bit_size == 16)
    {
        return NPY_UINT16;
    }
    else if (bit_size == 32)
    {
        return NPY_UINT32;
    }
    else if (bit_size == 64)
    {
        return NPY_UINT64;
    }
    return NPY_UINT64;
}

int get_primesieve_type(int bit_size)
{
    if (bit_size == 16)
    {
        return UINT16_PRIMES;
    }
    else if (bit_size == 32)
    {
        return UINT32_PRIMES;
    }
    else if (bit_size == 64)
    {
        return UINT64_PRIMES;
    }
    return UINT64_PRIMES;
}

static PyObject *generate_primes(PyObject *self, PyObject *args)
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    u_int64_t start, stop;
    size_t size;

    if (PyTuple_Size(args) == 1)
    {
        start = 2;
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid argument");
            PyGILState_Release(gstate);
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            PyGILState_Release(gstate);
            return NULL;
        }
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Invalid number of arguments");
        PyGILState_Release(gstate);
        return NULL;
    }

    int bit_size = get_bit_size((u_int64_t)stop);
    int np_int_type = get_np_type(bit_size);
    int ps_int_type = get_primesieve_type(bit_size);
    size = primesieve_count_primes(start,stop);

    npy_intp dims[1] = {size};

    PyObject *array = PyArray_SimpleNew(1, dims, np_int_type);

    if (array == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Failed to create NumPy array");
        PyGILState_Release(gstate);
        return NULL;
    }

    Py_INCREF(array);


    void *data = PyArray_DATA((PyArrayObject *)array);

    primesieve_iterator it;
    primesieve_init(&it);
    primesieve_jump_to(&it,start,stop);

    if (bit_size == 16)
    {
        for (int i = 0; i < (int)size; i++)
            ((u_int16_t *)data)[i] = primesieve_next_prime(&it);
    }
    else if (bit_size == 32)
    {
        for (int i = 0; i < (int)size; i++)
            ((u_int32_t *)data)[i] = primesieve_next_prime(&it);
    }
    else if (bit_size == 64)
    {
        for (int i = 0; i < (int)size; i++)
            ((u_int64_t *)data)[i] = primesieve_next_prime(&it);
    }
    // never forget these also
    primesieve_free_iterator(&it);

    Py_DECREF(array);
    PyGILState_Release(gstate);
    return array;
};

// Module Initialization

static PyMethodDef PandaPrimes_methods[] = {
    {"generate_primes", (PyCFunction)generate_primes, METH_VARARGS, "generate numpy array of primes"},
    {NULL, NULL, 0, NULL}};

static PyModuleDef PandaPrimes_module = {
    PyModuleDef_HEAD_INIT,
    "PaNDaPrime",
    "Deal with primes faster than the normal ways.",
    -1,
    PandaPrimes_methods,
};

PyMODINIT_FUNC PyInit_PandaPrimes(void)
{
    PyObject *module;
    module = PyModule_Create(&PandaPrimes_module);

    if (module == NULL)
        return NULL;

    if (PyType_Ready(&primes_rangeType) < 0)
        return NULL;
    if (PyType_Ready(&IteratorType) < 0)
        return NULL;
    import_array()
    Py_INCREF(&primes_rangeType);
    PyModule_AddObject(module, "primes_range", (PyObject *)&primes_rangeType);
    PyModule_AddObject(module, "Iterator", (PyObject *)&IteratorType);

    return module;
}

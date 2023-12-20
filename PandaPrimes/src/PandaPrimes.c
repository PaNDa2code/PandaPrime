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
        PyErr_SetString(PyExc_TypeError, "jump_to method takes one argument");
        return NULL;
    }
    if(!PyArg_ParseTuple(args, "K", &jump_to))
    {
        PyErr_SetString(PyExc_TypeError, "jump_to method takes one argument");
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

typedef struct
{
    int numpy_int_type, primesieve_int_type;
} Pint;

Pint get_int_types(u_int64_t integer)
{
    Pint m;
    if (integer <= UINT16_MAX)
    {
        m.numpy_int_type = NPY_UINT16;
        m.primesieve_int_type = UINT16_PRIMES;
    }
    else if (integer <= UINT32_MAX)
    {
        m.numpy_int_type = NPY_UINT32;
        m.primesieve_int_type = UINT32_PRIMES;
    }
    else if (integer <= UINT64_MAX)
    {
        m.numpy_int_type = NPY_UINT64;
        m.primesieve_int_type = UINT64_PRIMES;
    }
    return m;
};

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

    Pint _MyInt = get_int_types((u_int64_t)stop);

    void *primes = primesieve_generate_primes(start, stop, &size, _MyInt.primesieve_int_type);

    npy_intp dims[1] = {size};

    PyObject *array = PyArray_SimpleNewFromData(1, dims, _MyInt.numpy_int_type, primes);


    if (array == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Failed to create NumPy array");
        PyGILState_Release(gstate);
        return NULL;
    }

    // make numpy to free the memory in the array for you when it's garbage collected
    PyArray_ENABLEFLAGS((PyArrayObject *)array, NPY_ARRAY_OWNDATA);
    
    // never forget these also

    PyGILState_Release(gstate);
    return array;
};

static PyObject *generate_n_primes(PyObject *self, PyObject *args)
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    u_int64_t start;
    size_t n;
    if (PyTuple_Size(args) == 1)
    {
        start = 2;
        if (!PyArg_ParseTuple(args, "n", &n))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid argument ==> arguments should be int type");
            PyGILState_Release(gstate);
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "nK", &n, &start))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments ==> arguments should be int type");
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


    void *primes = primesieve_generate_n_primes(n, start, UINT64_PRIMES);

    npy_intp dims[1] = {n};

    PyObject *array = PyArray_SimpleNewFromData(1, dims, NPY_UINT64, primes);

    if (array == NULL)
    {   
        PyErr_SetString(PyExc_MemoryError, "Failed to create NumPy array");
        PyGILState_Release(gstate);
        return NULL;
    }
    
    // make numpy to free the memory in the array for you when it's garbage collected
    PyArray_ENABLEFLAGS((PyArrayObject *)array, NPY_ARRAY_OWNDATA);

    // never forget these also

    PyGILState_Release(gstate);
    return array;
}

static PyObject *get_nth_prime(PyObject *self,PyObject *args)
{
    u_int64_t n, start, nth_prime;
    if(PyTuple_Size(args) == 2)
    {
        if(!PyArg_ParseTuple(args, "KK", &n, &start ))
        {
            PyErr_SetString(PyErr_BadArgument,"Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if(PyTuple_Size(args) == 1)
    {
        if(!PyArg_ParseTuple(args, "K", &n))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function only takes two arguments."); /* code */
        return NULL;
    }


    
    nth_prime = primesieve_nth_prime(n, start);

    return PyLong_FromUnsignedLongLong(nth_prime);

}

// Module Initialization

static PyMethodDef PandaPrimes_methods[] = {
    {"generate_primes", (PyCFunction)generate_primes, METH_VARARGS, "generate numpy array of primes"},
    {"generate_n_primes", (PyCFunction)generate_n_primes, METH_VARARGS, "generate numpy array of primes"},
    {"get_nth_prime", (PyCFunction)get_nth_prime, METH_VARARGS, "Get the n^th prime"},
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

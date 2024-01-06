#include <Python.h>
#include <primesieve.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <stdio.h>


#include "primes-range.h"
#include "iterator.h"
#include "generate-primes.h"




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
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }


    
    nth_prime = primesieve_nth_prime(n, start);

    return PyLong_FromUnsignedLongLong(nth_prime);

}

static PyObject *count_primes(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, primes_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }

    primes_count = primesieve_count_primes(start, stop);

    return PyLong_FromUnsignedLongLong(primes_count);
}

PyObject *count_twins(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, twins_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }
    twins_count = primesieve_count_twins(start, stop);

    return PyLong_FromUnsignedLongLong(twins_count);
}

PyObject *count_triplets(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, twins_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }
    twins_count = primesieve_count_triplets(start, stop);

    return PyLong_FromUnsignedLongLong(twins_count);
}

PyObject *count_sextuplets(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, twins_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }
    twins_count = primesieve_count_sextuplets(start, stop);

    return PyLong_FromUnsignedLongLong(twins_count);
}

PyObject *count_quintuplets(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, twins_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }
    twins_count = primesieve_count_quintuplets(start, stop);

    return PyLong_FromUnsignedLongLong(twins_count);
}

PyObject *count_quadruplets(PyObject *self, PyObject *args)
{
    u_int64_t start, stop, twins_count;
    if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
        start = 0;
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes two arguments.");
        return NULL;
    }
    twins_count = primesieve_count_quadruplets(start, stop);

    return PyLong_FromUnsignedLongLong(twins_count);
}

PyObject *get_max_stop(PyObject *self)
{
    return PyLong_FromUnsignedLongLong(primesieve_get_max_stop());
}

PyObject *is_prime(PyObject *self, PyObject *args)
{   
    
    u_int64_t number;
    if (PyTuple_Size(args) == 1)
    {
        if (!PyArg_ParseTuple(args, "K", &number))
        {
            PyErr_SetString(PyErr_BadArgument, "Invalid arguments ==> should int type.");
            return NULL;
        }
    }
    else
    {
        PyErr_SetString(PyErr_BadArgument, "Invalid number of arguments ==> function takes 1 arguments.");
        return NULL;
    }
    long number_is_prime = primesieve_count_primes(number, number);

    // void *primes = primesieve_generate_primes(number, number, &number_is_prime, UINT64_PRIMES);
    // primesieve_free(primes);

    // number_is_prime = primesieve_nth_prime(0, number);

    PyBool_FromLong(number_is_prime);
}
// Module Initialization

static PyMethodDef PandaPrimes_methods[] = {
    {"generate_primes", (PyCFunction)generate_primes, METH_VARARGS, "generate numpy array of primes"},
    {"generate_n_primes", (PyCFunction)generate_n_primes, METH_VARARGS, "generate numpy array of primes"},
    {"get_nth_prime", (PyCFunction)get_nth_prime, METH_VARARGS, "Get the n^th prime"},
    {"count_primes", (PyCFunction)count_primes, METH_VARARGS, "Count primes"},
    {"count_twins", (PyCFunction)count_twins, METH_VARARGS, "Count twins primes"},
    {"count_triplets", (PyCFunction)count_triplets, METH_VARARGS, "count_triplets"},
    {"count_sextuplets", (PyCFunction)count_sextuplets, METH_VARARGS, "count_sextuplets"},
    {"count_quadruplets", (PyCFunction)count_quadruplets, METH_VARARGS, "count_quadruplets"},
    {"count_quintuplets", (PyCFunction)count_quintuplets, METH_VARARGS, "count_quintuplets"},
    {"get_max_stop", (PyCFunction)get_max_stop, METH_NOARGS, "Get the max prime"},
    {"is_prime", (PyCFunction)is_prime, METH_VARARGS, "Get the max prime"},
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

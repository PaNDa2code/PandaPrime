#include <Python.h>
#include <primesieve.h>
#include <stdio.h>


#include "primes-range.h"


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
    PyObject *Primes_Py_Object = (PyObject *)gen;
    // PyObject_SetAttrString(Primes_Py_Object, "start", Py_BuildValue("K",start));
    // PyObject_SetAttrString(Primes_Py_Object, "end", Py_BuildValue("K",end));
    gen->start = start;
    gen->end = end;
    primesieve_init(&gen->it);
    primesieve_jump_to(&gen->it, start, end);

    return Primes_Py_Object;
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

PyTypeObject primes_rangeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "primes_range",
    .tp_basicsize = sizeof(primes_range),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)primes_range_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "`primes_range` object is an object like Python's built-in range but iterates over prime numbers.",
    .tp_iter = (getiterfunc)primes_range_iter,
    .tp_iternext = (iternextfunc)primes_range_next,
    .tp_methods = primes_range_methods,
    .tp_new = primes_range_new,
};

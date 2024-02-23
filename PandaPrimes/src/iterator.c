#include <Python.h>
#include <primesieve.h>
#include <stdio.h>

// #include "iterator.h"

#define u_int64_t unsigned long long

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
    if (PyTuple_Size(args) != 1)
    {
        PyErr_SetString(PyExc_TypeError, "jump_to method takes one argument");
        return NULL;
    }
    if (!PyArg_ParseTuple(args, "K", &jump_to))
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

PyTypeObject IteratorType = {
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
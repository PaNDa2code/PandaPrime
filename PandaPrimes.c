#include <Python.h>
#include <primesieve.h>

typedef struct
{
    PyObject_HEAD
        primesieve_iterator it;
    size_t start, end;
} primes_range;

static PyObject *PrimeGen_new(PyTypeObject *type, PyObject *args, PyObject *kwarg)
{
    int start, end;
    if (PyTuple_Size(args) == 2)
    {
        // Attempt to parse two integers
        if (!PyArg_ParseTuple(args, "ii", &start, &end))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid argument types. Expected two integers.");
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 1)
    {
        // Attempt to parse a single integer
        if (!PyArg_ParseTuple(args, "i", &end))
        {
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

    gen->start = (size_t)start;
    gen->end = (size_t)end;
    primesieve_init(&gen->it);
    primesieve_jump_to(&gen->it,start,end);

    return (PyObject *)gen;
}

static void PrimeGen_dealloc(primes_range *gen)
{
    primesieve_clear(&gen->it);
    Py_TYPE(gen)->tp_free((PyObject *)gen);
}

static PyObject *PrimeGen_next(primes_range *gen)
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

static PyObject *PrimeGen_iter(primes_range *gen)
{
    Py_INCREF(gen);
    return (PyObject *)gen;
}

static PyMethodDef PrimeGen_methods[] = {
    {"next_prime", (PyCFunction)PrimeGen_next, METH_NOARGS, "Get the next prime in the range."},
    {NULL, NULL, 0, NULL}};

static PyTypeObject PrimeGenType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "primes_range",
    .tp_basicsize = sizeof(primes_range),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)PrimeGen_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "`primes_range` object is an object like Python's built-in range but iterates only over prime numbers.",
    .tp_iter = (getiterfunc)PrimeGen_iter,
    .tp_iternext = (iternextfunc)PrimeGen_next,
    .tp_methods = PrimeGen_methods,
    .tp_new = PrimeGen_new,
};

static PyModuleDef primes_module = {
    PyModuleDef_HEAD_INIT,
    "PaNDaPrime",
    "Deal with primes faster than the normal ways.",
    -1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};

PyMODINIT_FUNC PyInit_PandaPrimes(void)
{
    PyObject *module;
    module = PyModule_Create(&primes_module);

    if (module == NULL)
        return NULL;

    if (PyType_Ready(&PrimeGenType) < 0)
        return NULL;

    Py_INCREF(&PrimeGenType);
    PyModule_AddObject(module, "primes_range", (PyObject *)&PrimeGenType);

    return module;
}

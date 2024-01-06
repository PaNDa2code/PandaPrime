#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <primesieve.h>


#include "generate-primes.h"

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

PyObject *generate_primes(PyObject *self, PyObject *args)
{
    // PyGILState_STATE gstate = PyGILState_Ensure();

    u_int64_t start, stop;
    size_t size;

    if (PyTuple_Size(args) == 1)
    {
        start = 2;
        if (!PyArg_ParseTuple(args, "K", &stop))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid argument");
            // PyGILState_Release(gstate);
            return NULL;
        }
    }
    else if (PyTuple_Size(args) == 2)
    {
        if (!PyArg_ParseTuple(args, "KK", &start, &stop))
        {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            // PyGILState_Release(gstate);
            return NULL;
        }
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Invalid number of arguments");
        // PyGILState_Release(gstate);
        return NULL;
    }

    import_array()
    Pint _MyInt = get_int_types((u_int64_t)stop);

    void *primes;
    Py_BEGIN_ALLOW_THREADS
    primes = primesieve_generate_primes(start, stop, &size, _MyInt.primesieve_int_type);
    Py_END_ALLOW_THREADS
    
    npy_intp dims[1] = {size};
    PyObject *array = PyArray_SimpleNewFromData(1, dims, _MyInt.numpy_int_type, primes);
    if (array == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Failed to create NumPy array");
        // PyGILState_Release(gstate);
        return NULL;
    }
    // make numpy to free the memory in the array for you when it's garbage collected
    PyArray_ENABLEFLAGS((PyArrayObject *)array, NPY_ARRAY_OWNDATA);

    // never forget these also

    // PyGILState_Release(gstate);

    return array;
};

PyObject *generate_n_primes(PyObject *self, PyObject *args)
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
    void * primes;
    Py_BEGIN_ALLOW_THREADS
    primes = primesieve_generate_n_primes(n, start, UINT64_PRIMES);
    Py_END_ALLOW_THREADS

    import_array()
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
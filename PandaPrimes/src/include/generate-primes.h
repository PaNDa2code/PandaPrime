#ifndef GENERATE_PRIMES_H
#define GENERATE_PRIMES_H

#include <Python.h>

PyObject *generate_primes(PyObject *self, PyObject *args);
PyObject *generate_n_primes(PyObject *self, PyObject *args);

#endif
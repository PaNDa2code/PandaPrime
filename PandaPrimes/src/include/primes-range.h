#ifndef PRIMES_RANGE_H
#define PRIMES_RANGE_H

#include <Python.h>
#include <primesieve.h>

typedef struct
{
    PyObject_HEAD
        primesieve_iterator it;
    size_t start, end;
} primes_range;

extern PyTypeObject primes_rangeType;

#endif

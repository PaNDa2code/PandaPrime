#ifndef ITERATOR_H
#define ITERATOR_H

#include <Python.h>
#include <primesieve.h>

typedef struct
{
    PyObject_HEAD
        primesieve_iterator it;
} Iterator;

extern PyTypeObject IteratorType;

#endif


from setuptools import Extension, setup
import os

class get_numpy_include(object):
    def __str__(self):
        import numpy
        return numpy.get_include()

setup(
    setup_requires = ["numpy"],
    install_requires=['setuptools',
                      'numpy>=1.26.0'],
    packages=["PandaPrimes"],
    package_dir={'PandaPrimes': 'PandaPrimes'},
     ext_modules=[
        Extension(
            name="PandaPrimes.PandaPrimes",
             sources=["PandaPrimes/src/PandaPrimes.c"],
             libraries=["primesieve"],
             include_dirs=[get_numpy_include()],
             
        )
    ]
)

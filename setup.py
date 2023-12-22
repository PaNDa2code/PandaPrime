
from setuptools import Extension, setup
import os

class get_numpy_include(object):
    def __str__(self):
        import numpy
        return numpy.get_include()


with open("README.md", "r") as readme_file:
    README = readme_file.read()

PandaPrimes_ext = Extension(
            name="PandaPrimes.PandaPrimes",
             sources=["PandaPrimes/src/PandaPrimes.c"],
             libraries=["primesieve"],
             include_dirs=[get_numpy_include()],            
        )

setup(
    version="0.0.0",
    setup_requires = ["numpy"],
    install_requires=['setuptools',
                      'numpy>=1.26.0'],
    packages=["PandaPrimes"],
    package_dir={'PandaPrimes': 'PandaPrimes'},
    ext_modules=[PandaPrimes_ext],
    project_urls={
        "Source": "https://github.com/PaNDa2code/PandaPrimes",
    },
    author="PaNDa2code",
    author_email="moaaz0688@gmail.com",
    description="A Python extension module for finding primes using C",
    long_description=README,
    long_description_content_type="text/markdown",
)

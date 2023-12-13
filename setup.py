from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="PandaPrimes",
            sources=["PandaPrimes.c"],
            libraries=["primesieve"],  # Specify the library to link against
        )
    ]
)

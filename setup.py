from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
from platform import system
from build_primesieve import PrimesieveBuilder

debug = False
platform = system()

class Build_ext(build_ext):
    def run(self):
        builder = PrimesieveBuilder()
        dirs = builder.build_primesieve()
        primesieve_lib = dirs["primesieve_lib"]
        primesieve_include = dirs["include"]
        primesieve_path = dirs["lib"]

        if platform.startswith("Windows"):
            primesieve_lib = primesieve_lib.replace("/","\\")
            primesieve_include = primesieve_include.replace("/","\\")
            primesieve_path = primesieve_path.replace("/","\\")


        print(primesieve_lib)
        print(primesieve_include)
        print(primesieve_path)

        from numpy import get_include

        self.include_dirs.extend([primesieve_include, get_include()])
        
        assert self.extensions[0].name == "PandaPrimes.PandaPrimes", "PandaPrimes exaction is not defined"
        
        self.extensions[0].extra_objects.append(primesieve_lib)

        super().run()

        # Delete primesieve
        # if not debug: shutil.rmtree(dirs["primesieve"])


with open("README.md", "r") as readme_file:
    README = readme_file.read()

sources = ["PandaPrimes/src/" + c for c in os.listdir("PandaPrimes/src") if c.endswith(".c")]

PandaPrimes_ext = Extension(
    name="PandaPrimes.PandaPrimes",
    sources=sources,
    libraries=["stdc++"] if platform == "Linux" else [],
    include_dirs=["PandaPrimes/src/include"],
    language="c",
    extra_compile_args=[],
)

setup(
    name="PandaPrimes",
    version="0.0.6",
    setup_requires=["numpy", "cmake"],
    install_requires=['setuptools', 'numpy', 'cmake'],
    packages=["PandaPrimes"],
    package_dir={'PandaPrimes': 'PandaPrimes'},
    ext_modules=[PandaPrimes_ext],
    package_data= {"PandaPrimes": ["src/include/*.h"]},
    project_urls={
        "Source": "https://github.com/PaNDa2code/PandaPrimes",
    },
    author="PaNDa2code",
    author_email="moaaz0688@gmail.com",
    description="A Python extension module for finding primes using C",
    long_description=README,
    long_description_content_type="text/markdown",
    cmdclass={"build_ext":Build_ext},
)

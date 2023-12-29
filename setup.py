from setuptools import Extension, setup
import os
import zipfile
import io
import subprocess
import shutil
from urllib import request

def install_dependencies():
    subprocess.run(["pip", "install", "setuptools", "numpy>=1.26.0", "cmake"])

install_dependencies()

def get_numpy_include():
    from numpy import get_include
    return get_include()

class PrimesieveBuilder:
    def download_primesieve(self):
        url = "https://codeload.github.com/kimwalisch/primesieve/zip/refs/heads/master"
        try:
            with request.urlopen(url) as response:
                return response.read()
        except Exception as e:
            print(f"Error downloading primesieve: {e}")
            return None
        
    def unzip_file(self, zip_content, extract_to):
        with zipfile.ZipFile(io.BytesIO(zip_content), "r") as zip_ref:
            common_prefix = os.path.commonprefix(zip_ref.namelist())
            zip_ref.extractall(extract_to)

        return os.path.join(os.getcwd(), extract_to, common_prefix)

    def build_primesieve(self):
        if os.path.isfile("/home/panda/code/c-python-api/PandaPrime/primesieve/primesieve-master/lib/libprimesieve.a"):
            return {
            "libprimesieve.a":"/home/panda/code/c-python-api/PandaPrime/primesieve/primesieve-master/lib/libprimesieve.a",
            "include":os.path.join("/home/panda/code/c-python-api/PandaPrime/primesieve/primesieve-master/","include"),
            "lib":"/home/panda/code/c-python-api/PandaPrime/primesieve/primesieve-master/lib/",
        }
        cmake_build_args = ["--parallel"]
        cmake_config_args = ["-DCMAKE_POSITION_INDEPENDENT_CODE=ON","-DBUILD_PRIMESIEVE=OFF", "-DBUILD_SHARED_LIBS=OFF"]
        path = os.getcwd()
        primesieve_path = self.unzip_file(self.download_primesieve(), "primesieve")
        lib_path = os.path.join(primesieve_path, "lib")
        shutil.rmtree(lib_path, ignore_errors=True)
        os.makedirs(lib_path)

        config_command = ["cmake", f"-B {lib_path}", f"-S {primesieve_path}"] + cmake_config_args
        subprocess.run(config_command)

        build_command = ["cmake", "--build", lib_path] + cmake_build_args
        subprocess.run(build_command)

        libprimesieve_a = os.path.join(lib_path, "libprimesieve.a")
        assert os.path.isfile(libprimesieve_a)
        return {
            "libprimesieve.a":libprimesieve_a,
            "include":os.path.join(primesieve_path,"include"),
            "lib":lib_path,
        }



builder = PrimesieveBuilder()
dirs = builder.build_primesieve()
libprimesieve_a = dirs["libprimesieve.a"]
primesieve_include = dirs["include"]
libprimesieve_path = dirs["lib"]

with open("README.md", "r") as readme_file:
    README = readme_file.read()

panda_primes_ext = Extension(
    name="PandaPrimes.PandaPrimes",
    sources=["PandaPrimes/src/PandaPrimes.c"],
    libraries=["stdc++",],
    include_dirs=[get_numpy_include(),primesieve_include],  
    extra_objects=[libprimesieve_a],
    language="c",
    extra_compile_args=["-w"]
    
)

setup(
    version="0.0.2",
    setup_requires=[
        "setuptools",
        "numpy>=1.26.0",
        "cmake",
    ],
    packages=["PandaPrimes"],
    package_dir={"PandaPrimes": "PandaPrimes"},
    ext_modules=[panda_primes_ext],
    project_urls={
        "Source": "https://github.com/PaNDa2code/PandaPrimes",
    },
    author="PaNDa2code",
    author_email="moaaz0688@gmail.com",
    description="A Python extension module for finding primes using C",
    long_description=README,
    long_description_content_type="text/markdown",
)

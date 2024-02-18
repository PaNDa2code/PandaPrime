from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import subprocess
from urllib import request
import zipfile
import io
import shutil

debug = False

class PrimesieveBuilder:
    def download_primesieve(self):
        url = "https://codeload.github.com/kimwalisch/primesieve/zip/refs/heads/master"
        try:
            with request.urlopen(url) as response:
                return response.read()
        except Exception as e:
            print("Error downloading primesieve:{}".format(e))
            return None
        
    def unzip_file(self, zip_content, extract_to):
        with zipfile.ZipFile(io.BytesIO(zip_content), "r") as zip_ref:
            common_prefix = os.path.commonprefix(zip_ref.namelist())
            zip_ref.extractall(extract_to)

        return os.path.join(os.getcwd(), extract_to, common_prefix)

    def build_primesieve(self):
        if os.path.isdir(os.path.join(os.path.curdir, "primesieve-master")) and debug:
            return {
                "libprimesieve.a": "primesieve-master/lib/libprimesieve.a",
                "include": "primesieve-master/include",
                "lib": "primesieve-master/include",
                "primesieve": "primesieve-master",
            }
        # from cmake import CMAKE_BIN_DIR

        # cmake_bin_dir = CMAKE_BIN_DIR
        os_type = os.uname()[0]
        
        cmake_executable = "cmake.exe" if os_type == "Windows" else "cmake"
        # cmake_path = os.path.join(cmake_bin_dir, cmake_executable)
        cmake_path = "cmake"
        
        # assert os.path.isfile(cmake_path), f"Couldn't find {cmake_path}"

        cmake_build_args = ["--parallel"]
        cmake_config_args = ["-DCMAKE_POSITION_INDEPENDENT_CODE=ON", "-DBUILD_PRIMESIEVE=OFF", "-DBUILD_SHARED_LIBS=OFF"]

        current_path = os.getcwd()
        primesieve_path = self.unzip_file(self.download_primesieve(), current_path)
        subprocess.run("ls")
        assert os.path.isdir(primesieve_path), f"The directory '{primesieve_path}' does not exist or is not a valid directory."
        lib_path = os.path.join(primesieve_path, "lib")
        
        shutil.rmtree(lib_path, ignore_errors=True)
        os.makedirs(lib_path)

        config_command = [cmake_path, f"-B {lib_path}", f"-S {primesieve_path}"] + cmake_config_args
        print(config_command)
        subprocess.run(config_command)

        build_command = [cmake_path, "--build", lib_path] + cmake_build_args
        print(build_command)
        subprocess.run(build_command)

        libprimesieve_a = os.path.join(lib_path, "libprimesieve.a")
        assert os.path.isfile(libprimesieve_a), f"Couldn't find {libprimesieve_a}"

        return {
            "libprimesieve.a": libprimesieve_a,
            "include": os.path.join(primesieve_path, "include"),
            "lib": lib_path,
            "primesieve": primesieve_path,
        }


class Build_ext(build_ext):
    def run(self):
        builder = PrimesieveBuilder()
        dirs = builder.build_primesieve()
        libprimesieve_a = dirs["libprimesieve.a"]
        primesieve_include = dirs["include"]
        libprimesieve_path = dirs["lib"]

        from numpy import get_include

        self.include_dirs.extend([primesieve_include, get_include()])
        
        assert self.extensions[0].name == "PandaPrimes.PandaPrimes", "PandaPrimes exaction is not defined"
        
        # Add `libprimesieve.a` path to PandaPrimes_ext extra_objects list avoiding 
        self.extensions[0].extra_objects.append(libprimesieve_a)

        super().run()

        # Delete primesieve
        if not debug: shutil.rmtree(dirs["primesieve"])


with open("README.md", "r") as readme_file:
    README = readme_file.read()

sources = ["PandaPrimes/src/" + c for c in os.listdir("PandaPrimes/src") if c.endswith(".c")]

PandaPrimes_ext = Extension(
    name="PandaPrimes.PandaPrimes",
    sources=sources,
    libraries=["stdc++", "pthread"],
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

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import subprocess
from urllib import request
import zipfile
import io
import shutil
from platform import system

debug = False
platform = system()

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
                "primesieve_lib": os.path.join("primesieve-master", "lib", "Release", "primesieve.lib" if platform == "Windows" else "primesieve.a"),
                "include": os.path.join("primesieve-master", "include"),
                "lib": os.path.join("primesieve-master", "include"),
                "primesieve": "primesieve-master",
            }
        
        from cmake import CMAKE_BIN_DIR as cmake_bin_dir

        if platform == 'Windows':
            cmake_executable = "cmake.exe"
        else:
            cmake_executable = "cmake"

        cmake_path = os.path.join(cmake_bin_dir, cmake_executable) if platform == "Linux" else cmake_executable
                
        assert os.path.isfile(cmake_path), f"Couldn't find {cmake_path}"

        cmake_path = os.path("/user/bin/cmake") if os.path.isfile("/user/bin/cmake") else cmake_path

        cmake_build_args = ["--parallel", "--config", "Release"] if platform == 'Windows' else ["--parallel"]
        cmake_config_args = ["-DCMAKE_POSITION_INDEPENDENT_CODE=ON", "-DBUILD_PRIMESIEVE=OFF", "-DBUILD_SHARED_LIBS=OFF"]
        
        current_path = os.getcwd()
        primesieve_path = self.unzip_file(self.download_primesieve(), current_path)
        lib_path = os.path.join(primesieve_path, "lib")
        shutil.rmtree(lib_path, ignore_errors=True)
        os.makedirs(lib_path)
        config_command = [cmake_path, f"-B{lib_path}", f"-S{primesieve_path}"] + cmake_config_args
        subprocess.run(config_command)

        build_command = [cmake_path, "--build", lib_path] + cmake_build_args
        subprocess.run(build_command)
        
        if platform == 'Windows':
            primesieve_lib = os.path.join(lib_path, "Release", "primesieve.lib")
        else:
            primesieve_lib = os.path.join(lib_path, "libprimesieve.a")
                
        assert os.path.isfile(primesieve_lib), f"Couldn't find {primesieve_lib}"

        return {
            "primesieve_lib": primesieve_lib,
            "include": os.path.join(primesieve_path, "include"),
            "lib": lib_path,
            "primesieve": primesieve_path,
        }


class Build_ext(build_ext):
    def run(self):
        builder = PrimesieveBuilder()
        dirs = builder.build_primesieve()
        primesieve_lib = dirs["primesieve_lib"]
        primesieve_include = dirs["include"]
        primesieve_path = dirs["lib"]

        if platform == "Windows":
            primesieve_lib = primesieve_lib.replace("/","\\")
            primesieve_include = primesieve_include.replace("/","\\")
            primesieve_path = primesieve_path.replace("/","\\")


        print(primesieve_lib)
        print(primesieve_include)
        print(primesieve_path)

        from numpy import get_include

        self.include_dirs.extend([primesieve_include, get_include()])
        
        assert self.extensions[0].name == "PandaPrimes.PandaPrimes", "PandaPrimes exaction is not defined"
        
        # Add `libprimesieve.a` path to PandaPrimes_ext extra_objects list avoiding 
        self.extensions[0].extra_objects.append(primesieve_lib)

        super().run()

        # Delete primesieve
        if not debug: shutil.rmtree(dirs["primesieve"])


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

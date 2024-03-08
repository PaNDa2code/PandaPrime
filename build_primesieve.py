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
        url = "https://github.com/kimwalisch/primesieve/archive/refs/tags/v12.0.zip"
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
                
        # nasty trick but it works on windows
        # cmake = ["python", "-c", "import cmake;cmake.cmake()"]

        cmake = "cmake"

        cmake_build_args = ["--parallel", "--config", "Release"]
        cmake_config_args = ["-DCMAKE_POSITION_INDEPENDENT_CODE=ON", "-DBUILD_PRIMESIEVE=OFF", "-DBUILD_SHARED_LIBS=OFF"]
        
        current_path = os.getcwd()
        primesieve_path = self.unzip_file(self.download_primesieve(), current_path)
        primesieve_path = os.path.join(current_path, "primesieve-12.0")#.replace(" ", "\ ")
        lib_path = os.path.join(primesieve_path, "lib")#.replace(" ", "\ ")
        shutil.rmtree(lib_path, ignore_errors=True)
        os.makedirs(lib_path)
        config_command = [cmake, f"-B{lib_path}", f"-S{primesieve_path}"] + cmake_config_args
        print(f"running: {' '.join(config_command)}")
        subprocess.run(config_command, shell=True if platform == "Windows" else False)

        build_command = [cmake, "--build", lib_path] + cmake_build_args
        print(f"running: {' '.join(build_command)}")
        subprocess.run(build_command, shell=True if platform == "Windows" else False)
        
        if platform.startswith("Windows"):
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
from setuptools import Extension, setup
from setuptools.command.build import build
import subprocess
from io import BytesIO
import os
import shutil
from zipfile import ZipFile
import requests
# from numpy import get_include

def download_github_directory(url, directory_path):
    response = requests.get(url + "/archive/master.zip")
    print("installing primesieve")
    with ZipFile(BytesIO(response.content)) as zip_file:
        # Extract only the specified directory
        zip_file.extractall(directory_path)

def extract_include_directory():
    if os.path.exists("include") and os.path.isdir("include"):
        return
    repo_url="https://github.com/kimwalisch/primesieve"
    target_directory="."
    # Ensure target_directory exists
    if not os.path.exists(target_directory):
        os.makedirs(target_directory)

    # Temporary directory for extracting the entire repository
    temp_directory = os.path.join(target_directory, "temp_extraction")

    # Download and extract the entire repository
    download_github_directory(repo_url, temp_directory)

    # Move only the 'include' directory to the target location
    source_include = os.path.join(temp_directory, f"{os.path.basename(repo_url)}-master", "include")
    shutil.move(source_include, target_directory)

    # Clean up the temporary directory
    shutil.rmtree(temp_directory)

# Download the header files before building
class CustomBuild(build):
    def run(self):
        print("The custom bulid is running")
        extract_include_directory()
        build.run(self)

setup(
    setup_requires = ["numpy"],
    install_requires=['setuptools',
                      'requests',
                      'numpy>=1.26.0'],
    packages=["PandaPrimes"],
    package_dir={'PandaPrimes': 'PandaPrimes'},
     ext_modules=[
        Extension(
            name="PandaPrimes.PandaPrimes",
             sources=["PandaPrimes/src/PandaPrimes.c"],
             libraries=["primesieve"],  # Specify the library to link against
             include_dirs=["include"],
        )
    ],
    cmdclass={
        'build':CustomBuild,
       } 
)
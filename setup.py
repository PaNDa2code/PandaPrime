from setuptools import Extension, setup
from setuptools.command.install import install
from setuptools.command.build import build
from setuptools.command.build_ext import build_ext
import subprocess

# Download the header files before installing or building
class CustomInstall(install):
    def run(self):
        print("The custom install is running")
        # Run your script here before the installation
        subprocess.run(['python', 'post_setup.py'])
        # Continue with the default installation process
        install.run(self)
class CustomBuild(build):
    def run(self):
        print("The custom bulid is running")
        subprocess.run(['python', 'post_setup.py'])
        build.run(self)
class CustomBuild_ext(build_ext):
    def run(self):
        print("The custom bulid_ext is running")
        subprocess.run(['python', 'post_setup.py'])
        build_ext.run(self)

setup(
    setup_requires=['setuptools','requests'],
    ext_modules=[
        Extension(
            name="PandaPrimes",
            sources=["PandaPrimes.c"],
            libraries=["primesieve"],  # Specify the library to link against
            include_dirs=["include"],
        )
    ],
    cmdclass={
        'build':CustomBuild,
        'install':CustomInstall,
        'build_ext':CustomBuild_ext,
        }
)

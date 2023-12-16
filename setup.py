from setuptools import Extension, setup
from setuptools.command.build import build
import subprocess

# Download the header files before building
class CustomBuild(build):
    def run(self):
        print("The custom bulid is running")
        subprocess.run(['python', 'post_setup.py'])
        build.run(self)

setup(
    setup_requires=['setuptools','requests'],
    requires=['numpy'],
    packages=["PandaPrimes"],
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

# Use an official Python runtime as a parent image
FROM quay.io/pypa/manylinux_2_24_x86_64:latest

# Set the working directory to /PandaPrimes
WORKDIR /PandaPrimes

# Copy the current directory contents into the container at /PandaPrimes
COPY . /PandaPrimes


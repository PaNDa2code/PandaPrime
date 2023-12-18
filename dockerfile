# Use an official Python runtime as a parent image
FROM python:3.11

# Set the working directory to /PandaPrimes
WORKDIR /PandaPrimes

# Copy the current directory contents into the container at /PandaPrimes
COPY . /PandaPrimes

RUN apt update
RUN apt install cmake -y

# Install any needed packages specified in requirements.txt
RUN pip install PandaPrimes


# Use an official Python runtime as a parent image
FROM python:3.11

# Set the working directory to /PandaPrimes
WORKDIR /PandaPrimes

# Copy the current directory contents into the container at /PandaPrimes
COPY . /PandaPrimes

# Install dependencies and packages
RUN apt update && \
    apt install -y cmake && \
    pip install --upgrade pip && \
    pip install ./dist/PandaPrimes-0.0.4.tar.gz

# Set any additional configuration or environment variables if needed

# Specify the default command to run on container start
CMD ["bin/bash"]

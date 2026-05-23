# Dockerfile to build the kexp compilation environment
FROM ubuntu:24.04

# Install build dependencies
RUN apt-get update && apt-get install -y \
    clang-18 \
    lld-18 \
    make \
    libc6-dev-amd64-cross \
    && rm -rf /var/lib/apt/lists/*

# Set up Clang 18 as default
RUN ln -s /usr/bin/clang-18 /usr/bin/clang && \
    ln -s /usr/bin/clang++-18 /usr/bin/clang++ && \
    ln -s /usr/bin/ld.lld-18 /usr/bin/ld.lld && \
    ln -s /usr/bin/llvm-objcopy-18 /usr/bin/llvm-objcopy

# Set the working directory
WORKDIR /src

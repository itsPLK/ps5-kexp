#!/usr/bin/env bash
# Automated Build Script via Docker

IMAGE_NAME="kexp-builder"

function check_docker() {
    if ! command -v docker &> /dev/null; then
        echo "!!! docker command not found. Please install Docker first."
        exit 1
    fi
}

function build_image() {
    echo "--- Building Docker image $IMAGE_NAME... ---"
    docker build -t $IMAGE_NAME -f Dockerfile .
    if [ $? -ne 0 ]; then
        echo "!!! Docker image build FAILED!"
        exit 1
    fi
    echo "--- Docker image built successfully. ---"
}

function rebuild_image() {
    echo "--- Rebuilding Docker image (forcing no-cache)... ---"
    docker build --no-cache -t $IMAGE_NAME -f Dockerfile .
    if [ $? -ne 0 ]; then
        echo "!!! Docker image build FAILED!"
        exit 1
    fi
    echo "--- Docker image rebuilt successfully. ---"
}

function clean_project() {
    check_docker
    if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
        build_image
    fi
    echo "--- Cleaning build directory ---"
    docker run --rm -v "$(pwd)":/src -w /src $IMAGE_NAME make clean
}

function build_project() {
    check_docker
    if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
        build_image
    fi

    echo "--- Building kexp via Docker ---"
    docker run --rm -v "$(pwd)":/src -w /src $IMAGE_NAME make clean all

    if [ $? -ne 0 ]; then
        echo "!!! Build FAILED!"
        exit 1
    fi

    echo "--- Build successful! build/kexp.bin generated. ---"
}

# Command routing
case "$1" in
    rebuild-image)
        rebuild_image
        ;;
    clean)
        clean_project
        ;;
    *)
        build_project
        ;;
esac

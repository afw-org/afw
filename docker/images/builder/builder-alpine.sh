#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build 
#     [-e TAR_VERSION=<version>] 
#     [-e BUILD_TARGET=<target>]
#     -f docker/images/builder/Dockerfile.alpine
#     -t ghcr.io/afw-org/afw-builder:alpine .
#
# The TAR_VERSION env var can be specified at run-time to name 
#   the output file.
#

if [ ! -f /src/afw-package.json ]; then
    echo "** You need to map the source directory into /src in order to build."
    exit 1
fi

# change to source directory
cd /src

# use what was passed in, defaulting to afwdev
TAR_VERSION=${TAR_VERSION:=`./afwdev --version-string`}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`

echo "Building ${TAR_VERSION}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."

    # build the source
    ./afwdev build --prefix /usr/local --package
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    # copy to /
    cp build/cmake/afw-${TAR_VERSION}_${ARCH}.tar.gz /afw-${TAR_VERSION}-alpine.${ARCH}.tar.gz
    
    echo ""
    echo "File afw-${TAR_VERSION}-alpine.${ARCH}.tar.gz created."
fi

if [ "$BUILD_TARGET" == "js" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building JS code..."

    cd /src && \
    ./afwdev build --js --docs && \
    cd /src/build/js && \
    tar cf /afw-apps-${TAR_VERSION}.tar apps
    cd /src/build && \
    tar cf /afw-docs-${TAR_VERSION}.tar docs

    echo ""
    echo "File /afw-apps-${TAR_VERSION}.tar created."   
    echo "File /afw-docs-${TAR_VERSION}.tar created." 
fi

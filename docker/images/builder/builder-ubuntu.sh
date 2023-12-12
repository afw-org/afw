#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build
#     [-e DEB_VERSION=<version>] 
#     [-e BUILD_TARGET=<target>]
#     -t ghcr.io/afw-org/afw-builder:ubuntu .
#
# The DEB env var can be specified at run-time to name the output file.
#

if [ ! -f /src/afw-package.json ]; then
    echo "** You need to map the source directory into /src in order to build."
    exit 1
fi

# change to source directory
cd /src

# use what was passed in, defaulting to afwdev
DEB_VERSION=${DEB_VERSION:=`./afwdev --version-string`}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`

echo "Building ${DEB_VERSION}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."    

    # build for default location of /usr/local
    cd /src && ./afwdev build --prefix /usr/local --package
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    # copy to /    
    cp build/cmake/afw-${DEB_VERSION}_${ARCH}.deb /afw-${DEB_VERSION}-ubuntu_${ARCH}.deb
fi

if [ "$BUILD_TARGET" == "js" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building JS code..."

    cd /src && \
    ./afwdev build --js --docs && \
    cd /src/build/js && \
    tar cf /afw-apps-${DEB_VERSION}.tar apps
    cd /src/build/ && \
    tar cf /afw-docs-${DEB_VERSION}.tar docs

    echo ""
    echo "File afw-apps-${DEB_VERSION}.tar created."
    echo "File afw-docs-${DEB_VERSION}.tar created."
fi

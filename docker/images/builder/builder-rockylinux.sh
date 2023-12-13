#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build
#     [-e RPM_VERSION=<version>] 
#     [-e BUILD_TARGET=<target>]
#     -f docker/images/builder/Dockerfile.rockylinux
#     -t ghcr.io/afw-org/afw-dev-base:rockylinux .
#
# The RPM_VERSION and RPM_RELEASE env vars can be specified at run-time to 
#   name the output file.
#

if [ ! -f /src/afw-package.json ]; then
    echo "** You need to map the source directory into /src in order to build."
    exit 1
fi

# change to source directory
cd /src

# use what was passed in, defaulting to afwdev
RPM_VERSION=${RPM_VERSION:=`./afwdev --version-string`}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`

echo "Building ${RPM_VERSION}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."

    # build the source
    cd /src && ./afwdev build --prefix /usr/local --package
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    # copy to /    
    cp build/cmake/afw-${RPM_VERSION}_${ARCH}.rpm /afw-${RPM_VERSION}-rockylinux.${ARCH}.rpm
fi

if [ "$BUILD_TARGET" == "js" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building JS code..."

    cd /src && \
    ./afwdev build --js --docs && \
    cd build/js && \
    tar cf /afw-apps-${RPM_VERSION}.tar apps
    cd /src/build && \
    tar cf /afw-docs-${RPM_VERSION}.tar docs

    echo ""
    echo "File /afw-apps-${RPM_VERSION}.tar created."
    echo "File /afw-docs-${RPM_VERSION}.tar created."
fi

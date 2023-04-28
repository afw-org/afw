#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build 
#     [-e TAR_VERSION=<version>] 
#     [-e TAR_RELEASE=<release>] 
#     [-e BUILD_TARGET=<target>]
#     -f docker/images/builder/Dockerfile.alpine
#     -t ghcr.io/afw-org/afw-builder:alpine .
#
# The TAR_VERSION and TAR_RELEASE env vars can be specified at run-time to name 
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
TAR_RELEASE=${TAR_RELEASE:='0'}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`

echo "Building ${TAR_VERSION}-${TAR_RELEASE}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."
    mkdir -p ${HOME}/dist-${TAR_VERSION}

    # build the source
    ./afwdev build --prefix ${HOME}/dist-${TAR_VERSION}/usr/local --install
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    # Create a TAR file from the build
    cd ${HOME}/dist-${TAR_VERSION} && tar cvf afw-${TAR_VERSION}-${TAR_RELEASE}-alpine.${ARCH}.tar * 

    # copy to /
    cp afw-${TAR_VERSION}-${TAR_RELEASE}-alpine.${ARCH}.tar /
    
    echo ""
    echo "File afw-${TAR_VERSION}-${TAR_RELEASE}-alpine.${ARCH}.tar created."
fi

if [ "$BUILD_TARGET" == "js" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building JS code..."

    cd /src && \
    ./afwdev build --docs && \
    ./afwdev build --js -j && \
    mv build/docs build/js/apps/admin && \
    cd src/afw_app/admin && \
    mv admin afw_app && \
    tar cf admin-app-${TAR_VERSION}-${TAR_RELEASE}.tar afw_app

    # copy to /
    cp admin-app-${TAR_VERSION}-${TAR_RELEASE}.tar /    

    echo ""
    echo "File admin-app-${TAR_VERSION}-${TAR_RELEASE}.tar created."    
fi

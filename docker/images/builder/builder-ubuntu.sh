#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build
#     [-e DEB_VERSION=<version>] 
#     [-e DEB_RELEASE=<release>] 
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
DEB_RELEASE=${DEB_RELEASE:='0'}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`
VER_REL_ARCH=${DEB_VERSION}-${DEB_RELEASE}_${ARCH}

echo "Building ${DEB_VERSION}-${DEB_RELEASE}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."    

    # build the source
    mkdir -p ${HOME}/afw-${VER_REL_ARCH}/usr/local
    mkdir -p ${HOME}/afw-${VER_REL_ARCH}/DEBIAN

    # dynamically create our control file
    cat << EOF > ${HOME}/afw-${VER_REL_ARCH}/DEBIAN/control
Package: afw
Maintainer: info@adaptiveframework.org
Architecture: ${ARCH}
Version: ${DEB_VERSION}-${DEB_RELEASE}
Description: Adaptive Framework is a set of interfaces for developing servers, requests, object access, etc. in an adaptive way, along with implementations, language bindings, commands, and administrative applications.
EOF

    cd /src && ./afwdev build --prefix ${HOME}/afw-${VER_REL_ARCH}/usr/local --install
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    cd ${HOME}
    dpkg-deb --build --root-owner-group afw-${VER_REL_ARCH}

    # copy to /    
    cp afw-${DEB_VERSION}-${DEB_RELEASE}_${ARCH}.deb /afw-${DEB_VERSION}-${DEB_RELEASE}-ubuntu_${ARCH}.deb
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
    tar cf admin-app-${DEB_VERSION}-${DEB_RELEASE}.tar afw_app

    # copy to /    
    cp admin-app-${DEB_VERSION}-${DEB_RELEASE}.tar /    

    echo ""
    echo "File admin-app-${DEB_VERSION}-${DEB_RELEASE}.tar created."
fi

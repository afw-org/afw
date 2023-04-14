#!/usr/bin/env bash

# This script is executed when building the Docker afw-builder image.
#
#   docker build
#     [-e RPM_VERSION=<version>] 
#     [-e RPM_RELEASE=<release>] 
#     [-e BUILD_TARGET=<target>]
#     -f docker/images/builder/Dockerfile.rockylinux
#     -t ghcr.io/afw-org/afw-dev-base:rockylinux .
#
# The RPM_VERSION and RPM_RELEASE env vars can be specified at run-time to 
#   name the output file.
#

mkdir -p ${HOME}/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS,tmp}

if [ ! -f /src/afw-package.json ]; then
    echo "** You need to map the source directory into /src in order to build."
    exit 1
fi

# change to source directory
cd /src

# use what was passed in, defaulting to afwdev
RPM_VERSION=${RPM_VERSION:=`./afwdev --version-string`}
RPM_RELEASE=${RPM_RELEASE:='0'}
BUILD_TARGET=${BUILD_TARGET:='c'}
ARCH=`arch`

echo "Building ${RPM_VERSION}-${RPM_RELEASE}, target: ${BUILD_TARGET}"

if [ "$BUILD_TARGET" == "c" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building C code..."

    cp /afw.spec ${HOME}/rpmbuild/SPECS/

    # build the source
    cd /src && ./afwdev build --prefix ${HOME}/rpmbuild/SOURCES/afw-${RPM_VERSION}/usr/local --install
    if [ $? -ne 0 ]; then
        echo "** Core Build failed!"
        exit 1
    fi

    cd ${HOME}/rpmbuild/SOURCES 
    tar czf afw-${RPM_VERSION}-${RPM_RELEASE}.tar.gz afw-${RPM_VERSION}

    cd ${HOME}/rpmbuild && rpmbuild --define "debug_package %{nil}" --define "version ${RPM_VERSION}" --define "release ${RPM_RELEASE}" -ba SPECS/afw.spec

    # copy to /    
    cp /root/rpmbuild/RPMS/${ARCH}/afw-${RPM_VERSION}-${RPM_RELEASE}.${ARCH}.rpm /afw-${RPM_VERSION}-${RPM_RELEASE}-rockylinux.${ARCH}.rpm
fi

if [ "$BUILD_TARGET" == "js" ] || [ "$BUILD_TARGET" == "all" ]; 
then
    echo "Building JS code..."

    cd /src && \
    ./afwdev build --docs && \
    ./afwdev build --js -j && \
    mv build/doc build/js/apps/admin && \
    cd build/js/apps && \
    mv admin afw_app && \
    tar cf admin-app-${RPM_VERSION}-${RPM_RELEASE}.tar afw_app

    # copy to /    
    cp admin-app-${RPM_VERSION}-${RPM_RELEASE}.tar /    

    echo ""
    echo "File admin-app-${RPM_VERSION}-${RPM_RELEASE}.tar created."
fi

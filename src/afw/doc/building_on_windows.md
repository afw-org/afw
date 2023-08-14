# Building for Windows
(tested on Windows 11, 64-bit)

*********** This is a work in progress and is not complete ***********

There are multiple ways to build Adaptive Framework on a Windows system. If you use wsl or wsl2, you can build using instructions from 

## Source Build Dependencies

    Microsoft vcpkg can be used to install dependencies. If you don't already have vcpkg installed, see the README.md at https://github.com/microsoft/vcpkg.

## Fetch Source

    # Fetch source for Adaptive Framework, using the 'develop' branch
    git clone https://github.com/afw-org/afw.git    

## AdaptiveFramework Core and Base dependencies

    # apr and apr-util
    vcpkg install apr:x64-windows apr-util:x64-windows

    # fastcgi
    vcpkg install fastcgi:x64-windows

    # icu for Unicode support
    vcpkg install icu:x64-windows

    # libxml2
    vcpkg install libxml2:x64-windows

    # libxml2
    vcpkg install libxml2:x64-windows

    # libyaml
    vcpkg install libyaml:x64-windows

    # lmdb
    vcpkg install lmdb:x64-windows

    # pkgconf
    vcpkg install pkgconf:x64-windows

## Building Adaptive Framework

    cd afw
    afwdev build --cdev --cmake

## TL;DR

    vcpkg install apr:x64-windows apr-util:x64-windows fastcgi:x64-windows icu:x64-windows libxml2:x64-windows libyaml:x64-windows lmdb:x64-windows pkgconf:x64-windows
    cd afw
    afwdev build --cdev --cmake

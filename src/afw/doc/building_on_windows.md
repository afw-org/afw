# Building for Windows

** Windows is not currently supported **

If you do need to run afw on Windows, use WSL, Docker, or a VM to run it under
Linux.

The const data produced by `afwdev generate` accesses external data. This is
not supported by Windows DLLs. To access external symbols in DLLs,
dll import/export is needed and those symbols can not be accessed externally
by static const defines. This is needed to access things like **inf** and
**data type** instances.

Adaptive Framework originally supported resolving of these references by using
additional code that run when extensions were loaded. That became
increasingly complex and the support was removed.

If support is added back for Windows, cmake will need to support static builds
and software on Windows that uses afw will need to statically link in afw and
all extensions that will be accessed. This may be safer anyways because of the
way DLLs are located.


** Old information that may be useful if support is added **

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

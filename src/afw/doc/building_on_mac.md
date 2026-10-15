# Building for MacOS
(tested on MacOS Catalina Version 10.15.6)

## XCode and Homebrew

This build process relies on CLang for MacOS, provided by XCode, along with Homebrew for additional package libraries and headers.

## Homebrew dependencies

    brew install pkg-config
    brew install apache2
    brew install mysql
    brew install icu4c
    brew link icu4c
    brew install libyaml
    brew install openssl
    brew install openldap
    brew install lmdb
    # Optional: interactive line editing for the `afw` command (often already present on macOS)
    brew install libedit

## No apr or apr-util

Adaptive Framework does not use apr or apr-util, so there is nothing to
download or build for them. The LDAP extension uses the OpenLDAP API
directly (`brew install openldap` above).

## Setup paths for pkg-config

The pkg-config tool is a system tool that gives Makefiles a convenient way to get the header paths, library paths and compiler flags required for each installed library.  In order to allow for it to locate ICU and openssl, we need to:

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/icu4c/lib/pkgconfig:/usr/local/opt/openssl/lib/pkgconfig

## Building Adaptive Framework

    cd afw
    ./afwdev build

## Building and Installing Adaptive Framework

    ./afwdev build --install
# Building for MacOS
(tested on MacOS Catalina Version 10.15.6)

## XCode and Homebrew

This build process relies on CLang for MacOS, provided by XCode, along with Homebrew for additional package libraries and headers.

## Rules to run at install time. Homebrew dependencies

    brew install autoconf
    brew install automake
    brew install pkg-config
    brew install apache2
    brew install mysql
    brew install icu4c
    brew link icu4c
    brew install libyaml
    brew install openssl
    brew install berkeley-db
    brew install openldap
    brew install lmdb

## Download and Build apr and apr-util

Homebrew will allow you to install apr and apr-util, but without ldap support, as far as I can tell.  In order to build apr-util with LDAP support, you need to pass --with-ldap to the configure.  However, this is not enough.  The openldap API's for MacOS expects LDAP_SET_REBIND_PROC_THREE to be defined in order to call the right function.  From what I've encountered, the default configure script won't define this constant, even if you tell it, so the only recourse was to modify apr_ldap_rebind.c and `#define` it.  Somewhere out the apache community may offer a better solution.

## Setup paths for pkg-config

The pkg-config tool is a system tool that gives Makefiles a convenient way to get the header paths, library paths and compiler flags required for each installed library.  In order to allow for it to locate ICU and openssl, we need to:

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/icu4c/lib/pkgconfig:/usr/local/opt/openssl/lib/pkgconfig

## Building Adaptive Framework

    cd afw
    ./afwdev build

## Rules to run at install time. Adaptive Framework

    ./afwdev build --install
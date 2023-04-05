# Building for Ubuntu Linux
(tested on Ubuntu 20.04 LTS, 64-bit)

## Source Build Dependencies

    sudo apt-get install git
    sudo apt-get install autotools-dev autoconf libtool pkg-config

## Fetch Source

    # Fetch source for Adaptive Framework, using the 'develop' branch
    git clone https://github.com/afw-org/afw.git    

## Rules to run at install time. AdaptiveFramework Core dependencies

    # Apache Run Time and Apache2 Dev
    sudo apt-get install libapr1 libapr1-dev apache2-dev

    # libxml2
    sudo apt-get install libxml2 libxml2-dev

    # libicu for Unicode support
    sudo apt-get install libicu-dev

    # libunwind for stack trace
    sudo apt-get install libunwind-setjmp0-dev

    # libdw for ELF symbols
    sudo apt-get install elfutils libdw-dev

## Rules to run at install time. AdaptiveFramework Adaptor dependencies

Eventually, these adaptors will not be required, but rather enabled/disabled via the configure script.

    # MySQL Adaptor
    sudo apt-get install libmysqlclient-dev

    # BerkeleyDB Adaptor
    sudo apt-get install libdb-dev

    # LightningDB Adaptor
    sudo apt-get install liblmdb-dev

    # need OpenSSL and CURL for DynamoDB adaptor dependencies
    sudo apt-get install libcurl4-openssl-dev libssl-dev

    # Drivers for APR DBD interface
    sudo apt-get install libaprutil1-dbd-mysql 
    sudo apt-get install libaprutil1-dbd-sqlite3 
    sudo apt-get install libaprutil1-dbd-odbc
    sudo apt-get install libaprutil1-dbd-pgsql

## Rules to run at install time. dependencies for content-types

    # YAML
    sudo apt-get install libyaml-dev

## Rules to run at install time. HTTP stack, used by AdaptiveFramework 

    # FastCGI library
    sudo apt-get install libfcgi-dev

    # NGNIX
    sudo apt-get install nginx

## Development tools (optional)

    # debugging
    sudo apt-get install gdb
    sudo apt-get install valgrind
    sudo apt-get install lsof

    # python stuff
    pip install -r python-requirements.txt

## Building Adaptive Framework

    cd afw
    ./afwdev build    
    make

## Rules to run at install time. Adaptive Framework

    ./afwdev build --install


## TL;DR

    sudo apt-get install git autotools-dev autoconf libtool pkg-config libapr1 libapr1-dev apache2-dev libxml2 libxml2-dev libicu-dev libmysqlclient-dev libdb-dev liblmdb-dev libcurl4-openssl-dev libssl-dev libyaml-dev libfcgi-dev nginx libunwind-setjmp0-dev elfutils libdw-dev
    git clone https://github.com/afw-org/afw.git    
    cd afw
    ./afwdev build --install


# Building on RockyLinux

## Rules to run at install time. software dependencies

    dnf install -y epel-release && \
    dnf --enablerepo=PowerTools install -y autoconf make libtool \
        apr-devel apr-util-mysql apr-util-ldap \
        libxml2-devel libicu-devel \
        elfutils elfutils-devel libunwind-devel \
        httpd-devel mariadb mariadb-devel libdb-devel lmdb lmdb-devel \
        libcurl-devel libyaml libyaml-devel \
        python3-pip python3-devel \
        fcgi fcgi-devel wget git \
        ruby-devel rpm-build rubygems \
        openssl openssl-devel

## Rules to run at install time. python packages

    pip3.6 install requests inflection lxml watchdog

## Rules to run at install time. FPM (for creating RPMS)

    gem install --no-ri --no-rdoc fpm

## Building Adaptive Framework

    cd afw
    ./afwdev build

## Rules to run at install time. Adaptive Framework

    ./afwdev build --install

## TL;DR

    dnf install -y epel-release && \
    dnf --enablerepo=PowerTools install -y autoconf make libtool \
        apr-devel apr-util-mysql apr-util-ldap \
        libxml2-devel libicu-devel \
        elfutils elfutils-devel libunwind-devel \
        httpd-devel mariadb mariadb-devel libdb-devel lmdb lmdb-devel \
        libcurl-devel libyaml libyaml-devel \
        python3-pip python3-devel \
        fcgi fcgi-devel wget git \
        ruby-devel rpm-build rubygems \
        openssl openssl-devel
    git clone https://github.com/afw-org/afw.git    
    cd afw
    ./afwdev build --install


# Building on OpenSUSE Leap
(tested on Leap 15.3)

## Rules to run at install time. dependencies

    zypper install gcc9 gcc9-c++ automake autoconf libtool apache2-devel \
        libapr1 libapr1-devel libxml2 libxml2-devel icu libicu-devel \
        libunwind libunwind-devel elfutils libdw1 libdw-devel libyaml-devel \
        FastCGI FastCGI-devel libmysqlclient-devel libdb-4_8 libdb-4_8-devel \
        libcurl-devel lmdb lmdb-devel doxygen rpm-build python3

## Building Adaptive Framework

    ./afwdev build --install

# Building Docker image

You may find it more useful to use the Dockerfile in the project root to create a docker container to run or develop your instance of Adaptive Framework.  To get started, make sure you have docker installed on your system, then from the root of the Git repository, build an Adaptive Framework docker image, using:

    docker build -t afwfcgi -f docker/images/afwfcgi/Dockerfile .

This uses CentOS 7 as the base image, and installs all necessary dependencies, used to build Adaptive Framework.  It will take a while to build everything.  Once finished, you will have a docker image from which you can make container(s) from.  

## Run afw command from docker

When running a simple afw command, such as afw, you typically don't want a persistent container.  So, use the --rpm argument to have run the built image inside a new container, and have it automatically removed after the command ends.

    docker run --rm afwfcgi afw -x 'add(1,2,3)'

## Get a shell to the docker container

If you want to launch a container from the docker image and get shell access, while persisting changes to the container, you need to create a new container, start it, and attach to it.  First, create a new container, which will launch an interactive shell.  The -it argument makes it interactive, and /bin/sh is what's executed:

    docker container create -p 8080:8080 -it --name afwfcgi afw /bin/sh

Now, attach to the container we named "afw":

    docker attach afwfcgi

From here, we can change code, run builds and even startup afw_service_fcgi or nginx.  Changes will persist in this container across restarts.


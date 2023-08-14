# Building for Ubuntu Linux
(tested on Ubuntu 22.04 LTS, 64-bit)

## Source Build Dependencies

    sudo apt-get install git

## Fetch Source

    # Fetch source for Adaptive Framework, using the 'develop' branch
    git clone https://github.com/afw-org/afw.git    

## Install AdaptiveFramework Core dependencies

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

## AdaptiveFramework Adaptor dependencies

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

## Dependencies for content-types

    # YAML
    sudo apt-get install libyaml-dev

## HTTP stack, used by AdaptiveFramework 

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

## Adaptive Framework

    ./afwdev build --install


## TL;DR

    sudo apt-get install -y git libtool pkg-config clang-tools \
        libapr1 libapr1-dev apache2-dev libxml2 libxml2-dev python3-pip python3-lxml \
        libicu-dev libunwind-setjmp0-dev elfutils libdw-dev cmake \
        libmysqlclient-dev libdb-dev liblmdb-dev libcurl4-openssl-dev libssl-dev \
        libaprutil1-dbd-mysql libaprutil1-dbd-sqlite3 libaprutil1-dbd-odbc libaprutil1-dbd-pgsql \
        libyaml-dev libfcgi-dev nginx gdb valgrind lsof default-jre doxygen \
        nodejs npm tmux vim locales bash-completion docker.io
    git clone https://github.com/afw-org/afw.git    
    cd afw
    ./afwdev build --install


# Building on RockyLinux

## Software dependencies

    dnf install -y epel-release && \
    dnf install -y 'dnf-command(config-manager)' && \
    dnf install -y glibc-langpack-en && \
        yum config-manager --set-enabled powertools && \
        dnf group install "Development Tools" -y && \
        dnf --enablerepo=powertools install -y \
        make libtool cmake clang-analyzer \
        nginx gdb gdb-gdbserver valgrind valgrind-devel \
        apr-devel apr-util-mysql apr-util-ldap \
        libxml2-devel libxslt-devel libicu-devel \
        elfutils elfutils-devel libunwind-devel \
        httpd-devel mariadb mariadb-devel libdb-devel lmdb lmdb-devel \
        libcurl-devel libyaml libyaml-devel \
        python3-pip python3-devel \
        fcgi fcgi-devel wget git \
        rpm-build npm \
        openssl openssl-devel doxygen

## Python packages

    pip3.6 install python-requirements.txt

## Building Adaptive Framework

    cd afw
    ./afwdev build

## Adaptive Framework

    ./afwdev build --install

## TL;DR

    dnf install -y epel-release && \
    dnf --enablerepo=PowerTools install -y make libtool \
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
(tested on Leap 15.5)

## Install Dependencies

    zypper ref -s && \
    zypper -n --non-interactive in gcc9 gcc9-c++ \
        libtool apache2-devel cmake clang-tools \
        libapr1 libapr1-devel libxml2 libxml2-devel \
        icu libicu-devel libunwind libunwind-devel \
        elfutils libdw1 libdw-devel libyaml-devel \
        FastCGI FastCGI-devel libmysqlclient-devel \
        libdb-4_8 libdb-4_8-devel libcurl-devel git \
        lmdb lmdb-devel doxygen rpm-build valgrind lato-fonts \
        python3 python3-pip java-9-openjdk nodejs18 npm18

## Change compiler symlinks

    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 10 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 20 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/gcc-9 20 && \
    update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30 && \
    update-alternatives --set cc /usr/bin/gcc && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30 && \
    update-alternatives --set c++ /usr/bin/g++

## Python packages

    pip3 install python-requirements.txt

## Building Adaptive Framework

    ./afwdev build --install

# Building Docker image

You may find it more useful to use the Dockerfile in the project root to create a docker container to run or develop your instance of Adaptive Framework.  To get started, make sure you have docker installed on your system, then from the root of the Git repository, build an Adaptive Framework docker image, using:

    docker build -t afwfcgi -f docker/images/afwfcgi/Dockerfile .

This uses Alpine Linux as the base image, and installs all necessary dependencies, used to build Adaptive Framework.  It will take a while to build everything.  Once finished, you will have a docker image from which you can make container(s) from.  

## Run afw command from docker

When running a simple afw command, such as afw, you typically don't want a persistent container.  So, use the --rpm argument to have run the built image inside a new container, and have it automatically removed after the command ends.

    docker run --rm afwfcgi afw -x 'add(1,2,3)'

## Get a shell to the docker container

If you want to launch a container from the docker image and get shell access, while persisting changes to the container, you need to create a new container, start it, and attach to it.  First, create a new container, which will launch an interactive shell.  The -it argument makes it interactive, and /bin/sh is what's executed:

    docker container create -p 8080:8080 -it --name afwfcgi afw /bin/sh

Now, attach to the container we named "afw":

    docker attach afwfcgi

From here, we can change code, run builds and even startup afw_service_fcgi or nginx.  Changes will persist in this container across restarts.


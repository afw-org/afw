# Building on Linux

The package lists below are for Ubuntu 24.04. For the other distributions
the project builds on (Rocky Linux 10, AlmaLinux 9, openSUSE Leap 16.0,
Alpine Linux 3.24), the maintained list of packages is the
`docker/images/afw-dev-base/Dockerfile.<distribution>` file that CI uses;
install the packages it names, then continue at
[Python packages](#python-packages).

## Ubuntu 24.04

### Fetch source

    sudo apt-get install -y git
    git clone https://github.com/afw-org/afw.git

### Build tools

    sudo apt-get install -y cmake pkg-config python3-venv

### Core dependencies

    # libxml2
    sudo apt-get install -y libxml2 libxml2-dev

    # libicu for Unicode support
    sudo apt-get install -y libicu-dev

    # libunwind and libdw for stack traces
    sudo apt-get install -y libunwind-setjmp0-dev elfutils libdw-dev

### Extension dependencies

Each extension is optional; leave out the packages of an extension you do
not build.

    # LMDB adapter (afw_lmdb)
    sudo apt-get install -y liblmdb-dev

    # LDAP adapter (afw_ldap)
    sudo apt-get install -y libldap2-dev

    # curl functions (afw_curl) and crypto functions (afw_crypto)
    sudo apt-get install -y libcurl4-openssl-dev libssl-dev

    # YAML content type (afw_yaml)
    sudo apt-get install -y libyaml-dev

### afw command interactive line editing (optional)

    # NetBSD Editline (BSD-licensed). Enables line editing and history when
    # running `afw` interactively on a terminal. History is stored in
    # ~/.afw_history. Build succeeds without this package; editing is disabled.
    sudo apt-get install -y libedit-dev

### HTTP stack

    # FastCGI library, for afwfcgi
    sudo apt-get install -y libfcgi-dev

    # Nginx, to serve afwfcgi and the admin app
    sudo apt-get install -y nginx

### Development tools (optional)

    sudo apt-get install -y gdb valgrind lsof doxygen

    # Node.js and npm, to build the admin app and Javascript packages
    sudo apt-get install -y nodejs npm

## Python packages

`afwdev`, which generates and builds Adaptive Framework, needs the packages
in `python-requirements.txt`. Install them in a virtual environment (Ubuntu
24.04 refuses system-wide `pip install`) and keep it on `PATH`:

    python3 -m venv ~/afw-venv
    . ~/afw-venv/bin/activate
    pip install -r afw/python-requirements.txt

## Building Adaptive Framework

    cd afw

    # Generate, build and install the C libraries and commands, and afwdev
    ./afwdev build --cdev

    # Everything: C, Javascript (admin app) and documentation
    ./afwdev build --fulldev

## TL;DR

    sudo apt-get install -y git cmake pkg-config python3-venv \
        libxml2 libxml2-dev libicu-dev libunwind-setjmp0-dev elfutils libdw-dev \
        liblmdb-dev libldap2-dev libcurl4-openssl-dev libssl-dev libyaml-dev \
        libedit-dev libfcgi-dev nginx
    git clone https://github.com/afw-org/afw.git
    python3 -m venv ~/afw-venv && . ~/afw-venv/bin/activate
    pip install -r afw/python-requirements.txt
    cd afw
    ./afwdev build --cdev

# Docker

Rather than installing packages, you can build or develop inside a container
made from the `afw-dev-base` image, or run the published images. See
`docker/images/README.md` for the images and the order to build them in.

## Run the afw command from docker

When running a simple afw command, you typically don't want a persistent
container. Use `--rm` so the container is removed when the command ends:

    docker run --rm ghcr.io/afw-org/afw afw -x 'add(1,2,3)'

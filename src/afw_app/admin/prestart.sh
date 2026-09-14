#!/bin/sh

# Shell script that is executed before app is built
#

# create .env
version=`afwdev --version-string`
echo "Configuring for version: $version"
echo "VITE_VERSION=$version" > .env

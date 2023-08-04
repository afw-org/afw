#!/bin/sh

# Shell script that is executed before app is built
#

# create .env
version=`afwdev --version-string`
echo "Configuring for version: $version"
echo "REACT_APP_VERSION=$version" > .env

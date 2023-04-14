#!/bin/sh

# Shell script to build afw_app
#
# This script will first build all javascript package dependencies
# before finally building the admin app itself.

if [ ! -d "../../../node_modules" ]; then
  echo "Installing package dependencies.."  
  (cd ../../.. && npm install --no-optional --loglevel warn)
  if [ $? -ne 0 ]; then
    echo " ** Failed to install common node packages."
    exit 1
  fi
fi

# build all of the packages first
echo "Building packages.."
(cd ../../../ && npm run build-p:packages)
if [ $? -ne 0 ]; then
  echo "  ** Failed to build react packages."
  exit 1
fi

# finally, build app
npm run build
if [ $? -ne 0 ]; then
   echo "** Build Failed **"
   exit 1
fi

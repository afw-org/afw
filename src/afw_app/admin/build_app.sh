#!/bin/sh

# Shell script to build afw_app
#
# Sibling workspace packages (@afw/react, @afw/client, etc.) are consumed
# as source directly by Vite, so this just builds the admin app itself.

if [ ! -d "../../../node_modules" ]; then
  echo "Installing package dependencies.."
  (cd ../../.. && npm install --no-optional --loglevel warn)
  if [ $? -ne 0 ]; then
    echo " ** Failed to install common node packages."
    exit 1
  fi
fi

# build app (sibling workspace packages are consumed as source by Vite,
# no separate prebuild step needed)
npm run build
if [ $? -ne 0 ]; then
   echo "** Build Failed **"
   exit 1
fi

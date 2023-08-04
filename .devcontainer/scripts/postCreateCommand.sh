#!/bin/bash

# grab the working directory
workspace=$(dirname `pwd`)
workspaceRootPattern=`pwd | sed 's/\//\\\\\//g'`

if [ ! -e /afw/afw.conf ]; then
  echo "Initializing persistent volume at /afw"

  cp -r .devcontainer/afw/* /afw

  # change the directory path for the 'dev' adaptor
  sed -i "s/\/workspace/$workspaceRootPattern/" /afw/config/_AdaptiveServiceConf_/adaptor-dev.json 
fi

# If user persisted any custom container instructions, run them here
if [ -x ${workspace}/container-custom.sh ]; then 
  echo "Running custom container instructions at ${workspace}/container-custom.sh"
  ${workspace}/container-custom.sh 
fi

echo ""
echo "Container created."
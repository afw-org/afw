#!/usr/bin/env sh

# setup /afw for coredumps
sysctl -w kernel.core_pattern="/afw/core.%e.%p.%t"

# create /run/afw for running afwfcgi
if [ ! -d /run/afw ]; then
  mkdir -p /run/afw
  chown -R nginx /run/afw
fi

if [ ${BOOTSTRAP} = "false" ]; then
    # if they don't want to boostrap, then exit
    echo "No bootstrap, exiting"
    exit
fi

# check if /afw/afw.conf exists
if [ ! -f /afw/afw.conf ]; then
    # The /afw/afw.conf file should be built into the default image.
    #
    # Since it's not here, our assumption is that the user has intentionally 
    # mapped a volume over top of /afw.
    #
    # If this is the case, we'll assume that the volume is empty and hasn't 
    # been primed yet with a default configuration to persist in their 
    # volume.
    echo "No afw.conf found, assuming volume is empty and needs to be primed."
    echo ""
    echo "If you would like to disable this process, set BOOTSTRAP=false"
    echo "in your container environment."
    echo ""
    
    (cd /; tar xvf /afw.tar)
fi
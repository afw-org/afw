#!/bin/sh

# use NUM_THREADS from environment, otherwise default to 25
if [ -z "$NUM_THREADS" ]; then
  NUM_THREADS=25
fi

if [ ! -f /afw/afw.conf ]; then
  # assume the directory is newly mounted volume that we should prime
  (cd /; tar xvf /afw.tar)
fi

# capture coredumps, if we get them
echo '/afw/core.%h.%e.%t' > /proc/sys/kernel/core_pattern
ulimit -c unlimited

# Start afw_server_fcgi
if [ "$VALGRIND" = "1" ]; then
  valgrind --leak-=check=full afwfcgi -f /afw/afw.conf -n $NUM_THREADS 2>&1
else
  afwfcgi -f /afw/afw.conf -n $NUM_THREADS
fi

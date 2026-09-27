#!/bin/bash
# Seven hours, two processes, two temp directories.
# Does not loop afwdev test -j. Edit the argument to change the length.
exec "$(dirname "$0")/practice.sh" 25200

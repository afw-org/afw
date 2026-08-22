# Common gdb settings for issue-2 leak hunts.
# Loaded by _tools/gdb-run.sh and gdb-attach.sh.

set pagination off
set confirm off
set print pretty on
set print object on
set unwindonsignal on

handle SIGPIPE nostop noprint pass
# afw sets env->terminating on SIGINT; keep it for gdb unless you `handle SIGINT pass`.
handle SIGINT stop print nopass

python
import os
import sys
_gdb_dir = os.environ.get("AFW_ISSUE2_GDB_DIR")
if not _gdb_dir:
    try:
        _gdb_dir = os.path.dirname(gdb.current_objfile().filename)
    except Exception:
        _gdb_dir = None
if _gdb_dir and _gdb_dir not in sys.path:
    sys.path.insert(0, _gdb_dir)
try:
    import afw_gdb
    afw_gdb.register()
except Exception as _e:
    gdb.write("issue-2 gdb helpers not loaded: %s\n" % _e)
end

define afw-help
  echo issue-2 gdb helpers:\n
  echo   afw-rss              - print /proc/pid/status RSS\n
  echo   afw-heap             - evaluation_heap bytes_allocated (if xctx in frame)\n
  echo   afw-bt               - backtrace\n
  echo   afw-breaks           - try useful breakpoints (slot_store, integer create, heap)\n
  echo   afw-watch [seconds]  - continue, interrupt every N s, print RSS+heap\n
  echo   afw-help             - this text\n
end

echo Type afw-help for issue-2 leak helpers.\n

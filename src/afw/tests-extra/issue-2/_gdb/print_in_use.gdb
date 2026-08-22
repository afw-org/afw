# Print env->pool_bytes_in_use for a stopped afw. No inferior calls.
set pagination off
set confirm off
python
import gdb
frame = gdb.newest_frame()
xctx = None
while frame is not None:
    try:
        xctx = frame.read_var("xctx")
        break
    except Exception:
        pass
    try:
        x = frame.read_var("x")
        xctx = x["xctx"]
        break
    except Exception:
        pass
    frame = frame.older()
if xctx is None:
    gdb.write("IN_USE NA\n")
else:
    try:
        n = int(xctx["env"]["pool_bytes_in_use"])
        gdb.write("IN_USE %s\n" % n)
    except Exception as e:
        gdb.write("IN_USE NA %s\n" % e)
end

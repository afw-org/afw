# gdb Python helpers for issue-2 leak hunts. Loaded from afw.gdb.

from __future__ import print_function

import time

import gdb


_BREAK_CANDIDATES = (
    "afw_value_slot_store",
    "afw_value_add_reference",
    "afw_value_release",
    "afw_value_create_unmanaged_integer",
    "afw_pool_heap_create",
    "afw_pool_heap_tracker_create",
    "impl_afw_pool_malloc",
    "impl_afw_pool_calloc",
)


def _pid():
    inf = gdb.selected_inferior()
    if inf is None or inf.pid <= 0:
        raise gdb.GdbError("no running inferior")
    return int(inf.pid)


def _read_rss_kib(pid):
    path = "/proc/%d/status" % pid
    out = {}
    with open(path) as fd:
        for line in fd:
            if line.startswith("VmRSS:") or line.startswith("VmSize:") or \
                    line.startswith("VmData:") or line.startswith("VmPeak:") or \
                    line.startswith("VmHWM:"):
                parts = line.split()
                out[parts[0].rstrip(":")] = int(parts[1])
    return out


def _find_xctx():
    frame = gdb.newest_frame()
    while frame is not None:
        try:
            val = frame.read_var("xctx")
            if val is not None:
                return val, frame
        except Exception:
            pass
        try:
            val = frame.read_var("x")
            if val is not None:
                try:
                    xctx = val["xctx"]
                    if xctx is not None:
                        return xctx, frame
                except Exception:
                    pass
        except Exception:
            pass
        frame = frame.older()
    return None, None


def _heap_bytes(xctx):
    try:
        heap = xctx["evaluation_heap"]
    except Exception as e:
        return "evaluation_heap unreadable: %s" % e
    if heap == 0:
        return "evaluation_heap is NULL"
    try:
        ty = gdb.lookup_type("afw_pool_internal_self_t").pointer()
        self = heap.cast(ty)
        n = int(self["bytes_allocated"])
        rc = int(self["reference_count"])
        num = int(self["pool_number"])
        return (
            "evaluation_heap %s  pool_number=%s  reference_count=%s  "
            "bytes_allocated=%s (%s MiB)"
            % (heap, num, rc, n, "%.2f" % (n / 1024.0 / 1024.0))
        )
    except Exception as e:
        return "evaluation_heap %s (could not cast to afw_pool_internal_self_t: %s)" % (
            heap, e)


def _env_pool_and_maxrss(xctx):
    bits = []
    try:
        env = xctx["env"]
        n = int(env["pool_bytes_allocated"])
        bits.append("env->pool_bytes_allocated=%s (%s MiB)" % (
            n, "%.2f" % (n / 1024.0 / 1024.0)))
    except Exception as e:
        bits.append("env->pool_bytes_allocated unreadable: %s" % e)
    # Prefer /proc: inferior-call of afw_os_get_rss() after SIGSTOP can
    # abort the process. VmRSS is current; VmHWM/Peak is ru_maxrss.
    try:
        st = _read_rss_kib(_pid())
        bits.append("VmRSS=%s kB  VmHWM/Peak=%s/%s kB" % (
            st.get("VmRSS"), st.get("VmHWM"), st.get("VmPeak")))
    except Exception as e:
        bits.append("maxrss unreadable: %s" % e)
    return "  ".join(bits)


class AfwRss(gdb.Command):
    """Print /proc RSS for the afw inferior."""

    def __init__(self):
        super(AfwRss, self).__init__("afw-rss", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        pid = _pid()
        st = _read_rss_kib(pid)
        gdb.write(
            "pid=%s  VmRSS=%s kB  VmData=%s kB  VmSize=%s kB  VmPeak=%s kB\n"
            % (pid, st.get("VmRSS"), st.get("VmData"),
               st.get("VmSize"), st.get("VmPeak"))
        )
        xctx, _frame = _find_xctx()
        if xctx is not None:
            gdb.write(_env_pool_and_maxrss(xctx) + "\n")


class AfwHeap(gdb.Command):
    """Print evaluation_heap bytes_allocated plus env pool_bytes / maxrss."""

    def __init__(self):
        super(AfwHeap, self).__init__("afw-heap", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        xctx, frame = _find_xctx()
        if xctx is None:
            raise gdb.GdbError(
                "no xctx in the stack; interrupt during evaluate() first")
        gdb.write("frame %s\n" % frame.name())
        gdb.write(_heap_bytes(xctx) + "\n")
        gdb.write(_env_pool_and_maxrss(xctx) + "\n")


class AfwBt(gdb.Command):
    """Backtrace of the current thread."""

    def __init__(self):
        super(AfwBt, self).__init__("afw-bt", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        gdb.execute("bt 20")


class AfwBreaks(gdb.Command):
    """Try issue-2 breakpoints; skip names this libafw does not export."""

    def __init__(self):
        super(AfwBreaks, self).__init__("afw-breaks", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        bound = []
        missing = []
        for name in _BREAK_CANDIDATES:
            try:
                gdb.Breakpoint(name)
                bound.append(name)
            except Exception:
                missing.append(name)
        gdb.write("bound: %s\n" % (", ".join(bound) or "(none)"))
        if missing:
            gdb.write(
                "missing (older libafw or static): %s\n" % ", ".join(missing))
        gdb.write(
            "tip: `break afw_value.c:104` / `break afw_pool_heap.c:909` "
            "if DWARF is present but the symbol is not exported\n")


class AfwWatch(gdb.Command):
    """Continue; every N seconds interrupt and print RSS + heap. Ctrl-C stops."""

    def __init__(self):
        super(AfwWatch, self).__init__("afw-watch", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        interval = 5.0
        if arg and arg.strip():
            interval = float(arg.split()[0])
        pid = _pid()
        gdb.write(
            "watching pid %s every %.1fs — interrupt the process each tick\n"
            % (pid, interval))
        gdb.write(
            "more reliable: another terminal, sample-rss.sh, or /proc/%s/status\n"
            % pid)
        try:
            gdb.execute("set target-async on", to_string=True)
        except Exception:
            pass
        n = 0
        try:
            while True:
                n += 1
                try:
                    gdb.execute("continue &", to_string=True)
                except gdb.error as e:
                    if "already" not in str(e).lower():
                        gdb.write("%s\n" % e)
                time.sleep(interval)
                try:
                    gdb.execute("interrupt", to_string=True)
                except Exception as e:
                    gdb.write("interrupt: %s\n" % e)
                time.sleep(0.15)
                try:
                    st = _read_rss_kib(pid)
                    gdb.write(
                        "tick %s  VmRSS=%s kB  VmData=%s kB\n"
                        % (n, st.get("VmRSS"), st.get("VmData")))
                except Exception as e:
                    gdb.write("rss: %s\n" % e)
                    break
                xctx, _frame = _find_xctx()
                if xctx is not None:
                    gdb.write(_heap_bytes(xctx) + "\n")
        except KeyboardInterrupt:
            gdb.write("afw-watch stopped\n")


def register():
    AfwRss()
    AfwHeap()
    AfwBt()
    AfwBreaks()
    AfwWatch()
    gdb.write(
        "issue-2 gdb: afw-rss, afw-heap, afw-bt, afw-breaks, afw-watch, afw-help\n"
    )

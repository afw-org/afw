#! /usr/bin/env python3
##
# @file run_dir.py
# @brief One work directory per afwdev test run, kept a few days.
#
# Each run gets <tmpdir>/afwdev-runs/<MMDD-HHMMSS>-<mode>/ with:
#   lock          PID of the live run; pruning and --clear-temps skip it
#   failures.log  failure detail (failure_log.py), only when one failed
#   tmp/          TMPDIR and tempfile.tempdir for tests and their children
#   <leaf>/       per test-group work directories (afwfcgi logs, diag/)
# <tmpdir>/afwdev-runs/latest links to the newest run. Parallel runs
# never share a directory. At the start of a run, each env mode keeps
# its newest test_keep_runs (afwdev-settings.json, default 10) failed
# runs (those with a failures.log) and, separately, its newest
# test_keep_runs other runs, counting the new one. Many passing runs,
# such as an overnight loop, never push out a failure. A run still in
# use is never removed.
# Names stay short: Unix socket paths under a run are limited to
# SOCKET_PATH_MAX bytes.
#

import errno
import glob
import os
import shutil
import tempfile
import time

from _afwdev.common import msg

RUNS_DIRNAME = "afwdev-runs"
LATEST_NAME = "latest"
LOCK_NAME = "lock"
SCRATCH_NAME = "tmp"
FAILURES_NAME = "failures.log"
KEEP_RUNS = 10
# sizeof(sockaddr_un.sun_path) is 108 on Linux, including the NUL.
SOCKET_PATH_MAX = 107
# The work directory before per-run directories.
LEGACY_DIRNAME = "afwdev_test_output"
# Directories tests left in --tmpdir before scratch lived in the run:
# Python test mkdtemp prefixes and C-probe build directories. Exact
# patterns, so other afw* directories someone made by hand survive.
LEGACY_SCRATCH_GLOBS = (
    "afwfcgi_sig_*",
    "afw_clen_parse_*",
    "afw_env_ext_*",
    "afw_hostile_req_*",
    "afw_issue15_*",
    "afw-issue2-readln-*",
    "afw_ldap_url_*",
    "afw_pool_scope_churn_*",
    "afw_prime_c_probe_*",
    "afw_prune_headers_*",
    "afw_python_run_raises_*",
    "afw_req_body_*",
    "afw_req_pool_*",
    "afw_yaml_conf_*",
    "afw_yaml_file_*",
    "afw_c_probe_good_*",
    "afw_c_probe_stale_*",
    "afw_*_probe_*",
    "afwdev_build_tree_test_*",
    "afwdev_sanitize_*",
)


def runs_root(options):
    """<tmpdir>/afwdev-runs."""
    tmpdir = (options or {}).get("tmpdir") or "/tmp"
    return os.path.join(tmpdir, RUNS_DIRNAME)


def current(options):
    """This run's directory, or None before create()."""
    return (options or {}).get("_run_dir")


def keep_runs(options):
    """Run directories kept per env mode, including the current run."""
    settings = (options or {}).get("afwdev_settings") or {}
    value = settings.get("test_keep_runs")
    try:
        count = int(value)
    except (TypeError, ValueError):
        return KEEP_RUNS
    return count if count >= 1 else KEEP_RUNS


def _mode_of(path):
    """mode from <MMDD-HHMMSS>-<mode>[-N], or None."""
    parts = os.path.basename(path).split("-")
    return parts[2] if len(parts) >= 3 else None


def _pid_alive(pid):
    try:
        os.kill(pid, 0)
    except OSError as e:
        return e.errno == errno.EPERM
    return True


def in_use(path):
    """True while the run that owns path is alive."""
    try:
        with open(os.path.join(path, LOCK_NAME), "r") as fd:
            pid = int(fd.read().strip() or 0)
    except (OSError, ValueError):
        return False
    return pid > 0 and _pid_alive(pid)


def _run_dirs(root):
    if not os.path.isdir(root):
        return []
    out = []
    for name in os.listdir(root):
        path = os.path.join(root, name)
        if name == LATEST_NAME or os.path.islink(path):
            continue
        if os.path.isdir(path):
            out.append(path)
    return out


def _remove(path):
    shutil.rmtree(path, ignore_errors=True)


def _mtime(path):
    try:
        return os.path.getmtime(path)
    except OSError:
        return 0


def _failed(path):
    return os.path.isfile(os.path.join(path, FAILURES_NAME))


def prune(options, mode):
    """Before a new run: keep the newest of mode, failed runs apart.

    keep_runs failed runs, and keep_runs - 1 others (the new run is the
    last one). Runs still in use count toward the number but are never
    removed.
    """
    keep = keep_runs(options)
    same = [p for p in _run_dirs(runs_root(options)) if _mode_of(p) == mode]
    same.sort(key=_mtime, reverse=True)
    failed = [p for p in same if _failed(p)]
    others = [p for p in same if not _failed(p)]
    removed = 0
    for path in failed[keep:] + others[keep - 1:]:
        if in_use(path):
            continue
        _remove(path)
        removed += 1
    return removed


def clear(options):
    """--clear-temps: every run directory not in use, plus legacy leftovers."""
    removed = 0
    for path in _run_dirs(runs_root(options)):
        if in_use(path):
            continue
        _remove(path)
        removed += 1
    tmpdir = (options or {}).get("tmpdir") or "/tmp"
    legacy = {os.path.join(tmpdir, LEGACY_DIRNAME)}
    for pattern in LEGACY_SCRATCH_GLOBS:
        legacy.update(glob.glob(os.path.join(tmpdir, pattern)))
    for path in sorted(legacy):
        if os.path.isdir(path) and not os.path.islink(path):
            _remove(path)
            removed += 1
    _update_latest(runs_root(options))
    msg.highlighted_info(
        "Removed {n} temporary director{y} under {d}".format(
            n=removed, y="y" if removed == 1 else "ies", d=tmpdir))
    return removed


def _update_latest(root, target=None):
    """Point latest at target, or at the newest remaining run."""
    link = os.path.join(root, LATEST_NAME)
    if target is None:
        dirs = _run_dirs(root)
        if not dirs:
            if os.path.islink(link):
                os.remove(link)
            return
        target = max(dirs, key=os.path.getmtime)
    tmp_link = "{}.{}".format(link, os.getpid())
    try:
        if os.path.lexists(tmp_link):
            os.remove(tmp_link)
        os.symlink(os.path.basename(target), tmp_link)
        os.replace(tmp_link, link)
    except OSError:
        pass


def _new_dir(root, mode):
    base = "{}-{}".format(time.strftime("%m%d-%H%M%S"), mode)
    for n in range(1, 1000):
        name = base if n == 1 else "{}-{}".format(base, n)
        path = os.path.join(root, name)
        try:
            os.mkdir(path)
            return path
        except FileExistsError:
            continue
    raise OSError("no free run directory name under " + root)


def create(options, mode):
    """Prune, then make this run's directory, lock, latest, and scratch.

    TMPDIR and tempfile.tempdir point at the run's tmp/ so mkdtemp in
    tests (and in processes they start) is removed with the run.
    """
    root = runs_root(options)
    os.makedirs(root, exist_ok=True)
    prune(options, mode)
    path = _new_dir(root, mode)
    with open(os.path.join(path, LOCK_NAME), "w") as fd:
        fd.write(str(os.getpid()))
    scratch = os.path.join(path, SCRATCH_NAME)
    os.mkdir(scratch)
    os.environ["TMPDIR"] = scratch
    tempfile.tempdir = scratch
    _update_latest(root, path)
    options["_run_dir"] = path
    return path


def release(options):
    """Drop the lock at the end of the run. The directory is kept."""
    path = current(options)
    if not path:
        return
    try:
        os.remove(os.path.join(path, LOCK_NAME))
    except OSError:
        pass


def socket_path_error(path):
    """Message when a Unix socket path is too long, else None."""
    n = len(os.fsencode(path))
    if n <= SOCKET_PATH_MAX:
        return None
    return (
        "Unix socket path is {n} bytes, over the {m} byte limit: {p}. "
        "Use a shorter --tmpdir.".format(n=n, m=SOCKET_PATH_MAX, p=path))

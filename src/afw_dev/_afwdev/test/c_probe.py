#! /usr/bin/env python3

##
# @file c_probe.py
# @ingroup afwdev_test
# @brief Compile a checked-in *_probe.c against installed libafw and run
#        named cases.
# @details Use this when Adaptive Script cannot reach the hole (C-only
#          size, a second impl of an interface, hand-set utf8, …). The
#          .c is not a cmake test target and is not part of libafw.
#
#          Probe contract: argv[1] is the case name; exit 0 is pass.
#          Extra libraries (for example afwldap) are passed as
#          libraries=("afwldap", "afw").
#
#          When afwdev test --env-mode valgrind is in effect, each case
#          is run under valgrind with the suite suppressions. Those
#          suppressions cover known libunwind noise inside
#          afw_os_backtrace on a throw. Standalone valgrind without
#          that file can still report that noise; it is not the hole
#          under test. See issue #207. The UTF-8 / error-struct side of
#          a throw is issue #206.
#
#          Invented by Grok (xAI) for #207, August 2026, while pairing
#          on AFW. One helper instead of five copied cc lines; valgrind
#          wrap so unwind noise is a suppression, not folklore. If you
#          are poking at this module in a shell, try who().
#

import inspect
import os
import shutil
import subprocess
import tempfile

from _afwdev.test import context as test_context

__invented_by__ = "Grok (xAI)"
__invented_for__ = "https://github.com/afw-org/afw/issues/207"

_DEFAULT_INCLUDE = "/usr/local/include/afw"
_DEFAULT_LIBDIR = "/usr/local/lib/afw"
_DEFAULT_TIMEOUT_S = 60
_VALGRIND_TIMEOUT_S = 300


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _caller_dir():
    here = os.path.abspath(__file__)
    frame = inspect.currentframe()
    try:
        f = frame.f_back
        while f is not None:
            fn = os.path.abspath(f.f_code.co_filename)
            if fn != here:
                return os.path.dirname(fn)
            f = f.f_back
        return os.getcwd()
    finally:
        del frame


def _apr_includes():
    try:
        out = subprocess.check_output(
            ["apr-1-config", "--includes"], text=True)
        return out.split()
    except (OSError, subprocess.CalledProcessError):
        return ["-I/usr/include/apr-1.0"]


def _include_and_libdir():
    include_afw = os.environ.get("AFW_INCLUDE_DIR", _DEFAULT_INCLUDE)
    libdir = os.environ.get("AFW_LIB_DIR", _DEFAULT_LIBDIR)
    return include_afw, libdir


def _resolve_source(source, caller_dir):
    if os.path.isabs(source):
        return source
    return os.path.join(caller_dir, source)


def compile_c_probe(
        dest,
        source,
        libraries=("afw",),
        extra_cflags=None,
        extra_ldflags=None):
    """Compile source to dest against installed libafw.

    Raises subprocess.CalledProcessError on compiler failure.
    """
    include_afw, libdir = _include_and_libdir()
    cc = os.environ.get("CC", "cc")
    cmd = [
        cc, "-O0", "-g",
        "-I", include_afw,
    ]
    cmd.extend(_apr_includes())
    if extra_cflags:
        cmd.extend(list(extra_cflags))
    cmd.extend([
        "-o", dest, source,
        "-L", libdir, "-Wl,-rpath," + libdir,
    ])
    if extra_ldflags:
        cmd.extend(list(extra_ldflags))
    for lib in libraries:
        cmd.append("-l" + lib)
    subprocess.run(
        cmd, check=True, capture_output=True, text=True)


def _want_valgrind(valgrind):
    if valgrind is not None:
        return bool(valgrind)
    return test_context.options().get("mode") == "valgrind"


def _valgrind_timeout_s():
    options = test_context.options()
    raw = options.get("valgrind_test_timeout_s")
    if raw:
        try:
            return float(raw)
        except (TypeError, ValueError):
            pass
    return _VALGRIND_TIMEOUT_S


def _copy_valgrind_suppressions(work_dir):
    options = test_context.options()
    if options:
        try:
            from _afwdev.common import resources
            resources.copy_resources(options, "test/", todir=work_dir)
        except Exception:
            pass
        cand = os.path.join(work_dir, "valgrind.suppress")
        if os.path.isfile(cand):
            return cand
    here = os.path.dirname(os.path.abspath(__file__))
    cand = os.path.abspath(os.path.join(
        here, "..", "..", "_resources", "test", "valgrind.suppress"))
    if os.path.isfile(cand):
        return cand
    return None


def _valgrind_xml_has_error(xml_path):
    if not xml_path or not os.path.isfile(xml_path):
        return False
    try:
        with open(xml_path, "r", encoding="utf-8", errors="replace") as f:
            return "<error>" in f.read()
    except OSError:
        return False


def _run_probe_case(probe, name, timeout, work_dir, use_valgrind):
    cmd = [probe, name]
    xml_path = None
    if use_valgrind:
        valgrind = shutil.which("valgrind")
        if not valgrind:
            return 1, "valgrind not found on PATH"
        xml_path = os.path.join(work_dir, "valgrind-" + name + ".xml")
        log_path = os.path.join(work_dir, "valgrind-" + name + ".log")
        vg = [
            valgrind,
            "--xml=yes",
            "--xml-file=" + xml_path,
            "--log-file=" + log_path,
            "--show-possibly-lost=no",
        ]
        suppressions = _copy_valgrind_suppressions(work_dir)
        if suppressions:
            vg.append("--suppressions=" + suppressions)
        cmd = vg + cmd

    r = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    err = (r.stderr or "").strip() or (r.stdout or "").strip()
    if r.returncode != 0:
        return r.returncode, err or "exit {}".format(r.returncode)
    if use_valgrind and _valgrind_xml_has_error(xml_path):
        return 1, "Valgrind Error(s) detected."
    return 0, None


def run_c_probe(
        source,
        description,
        cases,
        libraries=("afw",),
        extra_cflags=None,
        extra_ldflags=None,
        timeout=None,
        valgrind=None,
        caller_dir=None):
    """Compile a checked-in probe and run named cases.

    @param source Filename or path of the *_probe.c. Relative paths are
                  resolved next to the calling file.
    @param description Suite description returned to the python runner.
    @param cases Sequence of (name, description) pairs. Each name is
                 passed as argv[1]; exit 0 is pass.
    @param libraries Linker libraries, default ("afw",).
    @param extra_cflags Optional extra compiler flags.
    @param extra_ldflags Optional extra linker flags.
    @param timeout Seconds per case. Default 60, or 300 under valgrind.
    @param valgrind True/False to force wrap. None follows --env-mode.
    @param caller_dir Override directory used to resolve source.
    @return Standard python-mode result dict.
    """
    if caller_dir is None:
        caller_dir = _caller_dir()
    src = _resolve_source(source, caller_dir)
    use_valgrind = _want_valgrind(valgrind)
    if timeout is None:
        timeout = _valgrind_timeout_s() if use_valgrind else _DEFAULT_TIMEOUT_S

    tests = []
    prefix = os.path.splitext(os.path.basename(src))[0] + "_"
    work_cm = tempfile.TemporaryDirectory(prefix="afw_" + prefix)
    try:
        work = work_cm.name
        dest = os.path.join(work, os.path.splitext(os.path.basename(src))[0])
        try:
            compile_c_probe(
                dest,
                src,
                libraries=libraries,
                extra_cflags=extra_cflags,
                extra_ldflags=extra_ldflags,
            )
        except Exception as e:
            detail = str(e)
            if isinstance(e, subprocess.CalledProcessError):
                detail = (
                    (e.stderr or "").strip()
                    or (e.stdout or "").strip()
                    or detail)
            return {
                "description": description,
                "tests": [
                    _case(
                        "compile_probe",
                        "Compile {} against installed libafw".format(
                            os.path.basename(src)),
                        False,
                        detail,
                    )
                ],
            }

        for name, desc in cases:
            try:
                rc, err = _run_probe_case(
                    dest, name, timeout, work, use_valgrind)
            except subprocess.TimeoutExpired:
                tests.append(_case(
                    name,
                    desc,
                    False,
                    "timed out after {}s".format(timeout),
                ))
                continue
            tests.append(_case(
                name,
                desc,
                passed=(rc == 0),
                error=None if rc == 0 else err,
            ))
    finally:
        work_cm.cleanup()

    return {
        "description": description,
        "tests": tests,
    }


def who():
    """Not part of the test surface. For the curious."""
    return (
        "run_c_probe is Grok's (#207, August 2026). "
        "Script cannot reach every hole; a copied cc blob should not "
        "be the gate. Compile once, run named cases, and when "
        "--env-mode valgrind is on, wrap the binary so libunwind's "
        "stack walk is a suppression, not a mystery. "
        "The copied cc lines had it coming."
    )

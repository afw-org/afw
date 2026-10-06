#! /usr/bin/env python3

##
# @defgroup afwdev_test_modes modes
# @brief This module defines the different test modes.
# @details Tests can be run in one or more different "modes", which provides 
#          different environments settings. For example, running from the 
#          command-line, running under FastCGI, or running under Valgrind.
#
#          Running tests under different modes exercises both the code being 
#          tested, as well as the environment expected to execute the code.
#
#          In addition, modes provides alternate ways to run tests that may be 
#          more convenient for the test writer. Some tests are easier to write 
#          in Adaptive Script, while others are easier to write in Python.
# @ingroup afwdev_test
#

## 
# @file test.py
# @ingroup afwdev_test
# @brief This file contains the main entry point for the "test" subcommand.
# @details Selects matching srcdirs, then list, watch, javascript, or the
#          runner. Modes such as afw and valgrind live under test/modes/.
#

import os
import sys
import time
import fnmatch
import resource

from _afwdev.common import msg, nfc, package
from _afwdev.test import watch, runner, js
from _afwdev.test.common import (
    find_test_groups, load_test_group_config, test_group_matches_tags,
    print_failure_digest, normalize_tests_paths, write_results_summary,
    clip_detail, format_xctx_bytes, want_error_detail)
from _afwdev.test import failure_log
from _afwdev.test import run_dir
from _afwdev.test import baseline as test_baseline
from _afwdev.test import family
from _afwdev.test import history as test_history
from _afwdev.test import sanitize as test_sanitize
from _afwdev.test import build_tree as test_build_tree


##
# @brief List matching tests without running them
#
def _list_tests(options, srcdirs):
    count = 0
    for srcdir, srcdirPath, _, manual_tests in srcdirs:
        if not os.path.exists(manual_tests):
            continue
        for testGroup in find_test_groups(options, srcdir, manual_tests):
            _, root, tests = testGroup
            testGroupConfig = load_test_group_config(root)
            if not test_group_matches_tags(options, testGroupConfig):
                continue
            for test in tests:
                msg.highlighted_info(os.path.relpath(test))
                count += 1
    msg.highlighted_info(str(count) + ' test(s) listed')
    sys.exit(0)


# --env-mode values. python and commands are per-file modes, not these.
ENV_MODES = ("afw", "afwfcgi", "actions", "valgrind", "asan")


## 
# @brief The main entry point for the "test" subcommand
# @details This routine is the main entry point for the "test" subcommand. 
#          Depending on its arguments, it can run tests in a variety of ways, 
#          depending on the options:
#
#          "watch" will wait for file system changes and run the test that has 
#          changed.
#
#          "run" (default) will simply run all requested tests.
#
#          "list" will list matching tests and exit.
#
#          "javascript" will run javascript tests.
#
#          This routine also collects some stats from test runs to report to the 
#          user.
# @param options The options dictionary.
# 
def run(options):

    command_start = time.time()
    mode = test_history.env_mode(options)
    if mode not in ENV_MODES:
        msg.error_exit(
            "Unknown --env-mode '{m}'. Use one of: {all}.".format(
                m=mode, all=", ".join(ENV_MODES)))

    total_passed = 0
    total_failed = 0
    total_skipped = 0
    total_tests = 0
    
    total_srcdirs = 0
    srcdirs_passed = 0
    srcdirs_failed = 0
    srcdirs_skipped = 0   

    srcdirs = []
    tests_paths = normalize_tests_paths(options.get('tests_path'))

    if tests_paths:
        # Opt-in roots only (e.g. tests-extra/) — exclusive, not package tests/
        for ap in tests_paths:
            try:
                label = os.path.relpath(ap)
            except ValueError:
                label = ap
            # srcdirPath used for environments / python path; root is the tree
            srcdir_path = ap if ap.endswith(os.sep) else ap + os.sep
            srcdirs.append(
                (
                    label,
                    srcdir_path,
                    None,
                    ap,
                )
            )
        total_srcdirs = len(srcdirs)
        if options.get('output') != '-':
            msg.highlighted_info(
                "Using --tests-path (exclusive): " + ", ".join(tests_paths))
    else:
        for srcdir in package.get_afw_package(options)['srcdirs']:
            if not fnmatch.fnmatch(srcdir, options['srcdir_pattern']):
                continue

            total_srcdirs += 1
            package.set_options_from_existing_package_srcdir(
                options, srcdir, set_all=True)

            objects_dir = options['srcdir_path'] + 'generate/objects/'
            manual_tests = options['srcdir_path'] + 'tests'

            srcdirs.append(
                (
                    srcdir,
                    options['srcdir_path'],
                    objects_dir,
                    manual_tests
                )
            )

    if options.get('list'):
        _list_tests(options, srcdirs)

    if options.get('javascript'):

        js.run(options, srcdirs)

    elif options.get('watch'):

        watch.run(options, srcdirs)

    else:

        # Reports and housekeeping never run tests.
        want_trend = options.get('trend') not in (False, None)
        if want_trend or _wants_housekeeping(options):
            try:
                _do_housekeeping(options)
                if want_trend:
                    _run_trend(options)
            except (ValueError, OSError) as e:
                msg.error_exit(str(e))
            sys.exit(0)

        if options.get('baseline') and _narrowed(options):
            msg.error_exit(
                "--baseline needs the whole suite: drop --test-pattern, "
                "--srcdir-pattern, --tags, and -T.")

        # --build-tree: run against the mode's cmake tree, not the
        # install. Sanitizer pairing: asan runs against build/asan;
        # valgrind cannot run a sanitizer build.
        # asan always uses its tree (it is never installed system-wide).
        if options.get('build_tree') or \
                test_history.env_mode(options) == 'asan':
            test_build_tree.prepare(options)
        if test_history.env_mode(options) == 'asan':
            test_sanitize.prepare_asan_environment(options)
        elif test_history.env_mode(options) == 'valgrind':
            test_sanitize.refuse_sanitized_lib_for_valgrind()

        try:
            run_dir.create(options, test_history.env_mode(options))
        except OSError as e:
            msg.error_exit("Can not create the run directory: " + str(e))
        try:
            compare = _prepare_compare(options)
        except (ValueError, OSError) as e:
            msg.error_exit(str(e))
        failure_log.begin(options)
        try:
            start = time.time()
            results, failures, max_xctx_bytes, file_records = runner.run(
                options, srcdirs)
            max_xctx_chunk_bytes = test_history.max_file_metric(
                file_records, "xctx_chunk_bytes")
            end = time.time()

            # iterate over results dict and print results
            for srcdir, stats in results.items():            
                passed, skipped, failed = stats

                total_passed += passed
                total_skipped += skipped
                total_failed += failed
                total_tests += passed + skipped + failed

                if failed > 0:
                    srcdirs_failed += 1

            srcdirs_passed = total_srcdirs - (srcdirs_failed + srcdirs_skipped)
            elapsed = round(time.time() - command_start, 2)
            cpu_seconds = _cpu_seconds()
            fam = None
            if compare["run"] is not None:
                fam = family.evaluate(
                    file_records, compare["run"], compare["last"],
                    compare["settings"])
                fam["label"] = compare["label"]

            # When --output is '-', keep stdout clean for the machine summary
            summary_to_stdout = (options.get('output') == '-')

            if not summary_to_stdout:
                # Print human summary
                msg.highlighted_info("")
                msg.highlighted_info("Source Dirs:   ", end="")
                if srcdirs_failed > 0:
                    msg.error("{} failed".format(srcdirs_failed), end="")
                    msg.highlighted_info(", ", end="")
                if srcdirs_skipped > 0:
                    msg.warn("{} skipped".format(srcdirs_skipped), end="")
                    msg.highlighted_info(", ", end="")
                if srcdirs_passed > 0:
                    msg.success("{} passed".format(srcdirs_passed), end="")
                    msg.highlighted_info(", ", end="")

                msg.highlighted_info("{} total".format(total_srcdirs))

                msg.highlighted_info("Tests:         ", end="")
                if total_failed > 0:
                    msg.error("{} failed".format(total_failed), end="")
                    msg.highlighted_info(", ", end="")
                if total_skipped > 0:
                    msg.warn("{} skipped".format(total_skipped), end="")
                    msg.highlighted_info(", ", end="")
                if total_passed > 0:
                    msg.success("{} passed".format(total_passed), end="")
                    msg.highlighted_info(", ", end="")

                msg.highlighted_info("{} total".format(total_tests))
                msg.highlighted_info("Elapsed:       {}   CPU: {}".format(
                    _fmt_duration(elapsed), _fmt_duration(cpu_seconds)))
                if max_xctx_bytes or max_xctx_chunk_bytes:
                    parts = []
                    if max_xctx_bytes:
                        parts.append("{} xctx".format(
                            format_xctx_bytes(max_xctx_bytes)))
                    if max_xctx_chunk_bytes:
                        parts.append("{} chunk".format(
                            format_xctx_bytes(max_xctx_chunk_bytes)))
                    msg.highlighted_info("Memory:        max " + ", ".join(parts))
                family.print_summary(fam)
                if want_error_detail(options):
                    family.print_detail(fam)

                # Console-only digest so parallel -j runs still end with greppable paths
                print_failure_digest(failures)

            summary = {
                'srcdirs': {
                    'passed': srcdirs_passed,
                    'failed': srcdirs_failed,
                    'skipped': srcdirs_skipped,
                    'total': total_srcdirs,
                },
                'tests': {
                    'passed': total_passed,
                    'failed': total_failed,
                    'skipped': total_skipped,
                    'total': total_tests,
                },
                'time_seconds': elapsed,
                'narrowed': _narrowed(options),
                'cpu_seconds': cpu_seconds,
                'family': family.summary_record(fam),
                'max_xctx_bytes': max_xctx_bytes or 0,
                'max_xctx_chunk_bytes': max_xctx_chunk_bytes or 0,
                'mode': test_history.env_mode(options),
                'git': test_history.git_meta(),
                'files': sorted(
                    file_records or [], key=lambda r: r.get('path') or ''),
                'by_srcdir': {
                    srcdir: {
                        'passed': stats[0],
                        'skipped': stats[1],
                        'failed': stats[2],
                    }
                    for srcdir, stats in results.items()
                },
                'failures': [
                    {
                        'test': f.get('test'),
                        'detail': clip_detail(f.get('detail')),
                        'group': f.get('group'),
                        'srcdir': f.get('srcdir'),
                    }
                    for f in (failures or [])
                ],
            }
            write_results_summary(options, summary, tool_label='test')

            if test_history.should_write_history(options):
                written = test_history.write_history(summary, options)
                if options.get('baseline'):
                    test_baseline.replace_own(options, written)

            # Machine summary (--output -) stays free of these lines.
            if not summary_to_stdout:
                _print_run_location(options)
            if total_failed > 0:
                sys.exit(1)
            if family.strict_failure(fam, compare["settings"]):
                msg.error(
                    "Memory out of family and test_family_strict is set.")
                sys.exit(1)
            sys.exit(0)
        finally:
            failure_log.finish(options)
            run_dir.release(options)


def _print_run_location(options):
    msg.highlighted_info("Run:           {p}   (keeps the last {n} {m} runs and {n} failed)".format(
        p=run_dir.current(options), n=run_dir.keep_runs(options),
        m=test_history.env_mode(options)))
    failures = failure_log.path_if_failed(options)
    if failures:
        msg.highlighted_info("Failures:      " + failures)


def _wants_housekeeping(options):
    return bool(options.get('clear_temps') or options.get('clear_history'))


def _do_housekeeping(options):
    if options.get('clear_temps'):
        run_dir.clear(options)
    if options.get('clear_history'):
        test_history.clear_history(options)


def _narrowed(options):
    """True when the run is not the whole suite."""
    return bool(
        options.get('tests_path')
        or (options.get('test-pattern') or '.*') != '.*'
        or (options.get('test_tags') or '.*') != '.*'
        or (options.get('srcdir_pattern') or '*') not in ('*', '\\*'))


def _prepare_compare(options):
    """Prune history, pick what this run compares against, share it.

    Returns {run, last, label, settings}. run is None when there is
    nothing to compare against.
    """
    settings = family.settings_from(options)
    out = {"run": None, "last": None, "label": None, "settings": settings}
    if options.get('baseline'):
        options['_history_label'] = test_baseline.label_for_branch(
            test_history.git_meta().get('branch'))
    if test_history.should_write_history(options):
        test_baseline.prune(options, test_baseline.select_baseline(options))
    path, label = test_baseline.resolve(options)
    out["label"] = label
    if path:
        run = test_history.load_run(path)
        if (run.get('mode') or 'afw') != test_history.env_mode(options):
            raise ValueError("--compare-to {p} is a {m} run".format(
                p=path, m=run.get('mode')))
        out["run"] = run
        last = test_baseline.last_run(options)
        if last and last != path:
            out["last"] = test_history.load_run(last)
        family.set_context(
            test_history.files_by_path(run), label, settings)
    return out


def _cpu_seconds():
    """CPU of afwdev and every process it waited for (tests, workers)."""
    me = resource.getrusage(resource.RUSAGE_SELF)
    kids = resource.getrusage(resource.RUSAGE_CHILDREN)
    return round(me.ru_utime + me.ru_stime + kids.ru_utime + kids.ru_stime, 2)


def _fmt_duration(seconds):
    seconds = float(seconds or 0)
    if seconds < 60:
        return "{:.1f}s".format(seconds)
    minutes, sec = divmod(int(round(seconds)), 60)
    if minutes < 60:
        return "{}m{:02d}s".format(minutes, sec)
    hours, minutes = divmod(minutes, 60)
    return "{}h{:02d}m".format(hours, minutes)


def _run_trend(options):
    options['trend_metric'] = options.get('trend') or 'bytes'
    test_history.print_baselines(options)
    runs = test_history.resolve_trend_runs(options)
    test_history.print_trend(
        test_history.trend_runs(runs, options),
        show_all=bool(options.get('show_all')))

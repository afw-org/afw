#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""afwdev build --sanitize: accepted variant, refused combinations, install."""

import contextlib
import io

from _afwdev.build.build import (
    apply_build_profile_flags,
    apply_sanitize_options,
)


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _apply(options):
    """Same order as build.run(): note explicit --install, then profiles."""
    options['build_install_explicit'] = bool(options.get('build_install'))
    apply_build_profile_flags(options)
    apply_sanitize_options(options)
    return options


def _exits(options):
    """True if applying options exits (msg.error_exit), quietly."""
    try:
        with contextlib.redirect_stdout(io.StringIO()), \
                contextlib.redirect_stderr(io.StringIO()):
            _apply(options)
    except SystemExit:
        return True
    return False


def run():
    tests = []

    plain = _apply({'build_sanitize': 'address'})
    tests.append(_case(
        "address",
        "--sanitize address: address + undefined into build/asan, "
        "cmake only, no install",
        passed=(
            plain.get('build_sanitizers') == ('address', 'undefined')
            and plain.get('build_sanitize_dir') == 'asan'
            and plain.get('build_cmake') is True
            and plain.get('build_install') is False
        ),
    ))

    cdev = _apply({'build_sanitize': 'address', 'build_cdev': True})
    tests.append(_case(
        "cdev-no-install",
        "--cdev --sanitize address: --cdev's implied --install is dropped",
        passed=(
            cdev.get('build_install') is False
            and cdev.get('build_clean') is True
            and cdev.get('build_generate') is True
        ),
    ))

    explicit = _apply({'build_sanitize': 'address', 'build_install': True})
    tests.append(_case(
        "explicit-install",
        "--sanitize address --install still installs",
        passed=explicit.get('build_install') is True,
    ))

    normal = _apply({'build_cdev': True})
    tests.append(_case(
        "no-sanitize-untouched",
        "without --sanitize, --cdev still installs and sets no sanitizers",
        passed=(
            normal.get('build_install') is True
            and not normal.get('build_sanitizers')
        ),
    ))

    tests.append(_case(
        "variants-refused",
        "--sanitize thread / memory / bogus exit with an error",
        passed=all(_exits({'build_sanitize': v})
            for v in ('thread', 'memory', 'bogus')),
    ))

    refused = ('build_fulldev', 'build_all', 'build_docs', 'build_js',
        'build_docker', 'build_package', 'build_scan')
    not_refused = [flag for flag in refused
        if not _exits({'build_sanitize': 'address', flag: True})]
    tests.append(_case(
        "combinations-refused",
        "--sanitize with --fulldev, --all, --docs, --js, --docker, "
        "--package or --scan exits with an error",
        passed=not not_refused,
        error=("not refused: " + ", ".join(not_refused))
            if not_refused else None,
    ))

    return {
        "description": "afwdev build --sanitize option handling",
        "tests": tests,
    }

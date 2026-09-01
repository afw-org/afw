#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""--cdev and --fulldev both enable --install; leftover header prune."""

import os
import tempfile
import shutil

from _afwdev.build.build import apply_build_profile_flags
from _afwdev.build.cmake import (
    is_leftover_installed_header,
    prune_leftover_installed_headers,
)


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def run():
    tests = []

    cdev = {"build_cdev": True}
    apply_build_profile_flags(cdev)
    tests.append(_case(
        "cdev-install",
        "--cdev enables --install, --generate, --clean, and cmake",
        passed=(
            cdev.get("build_install") is True
            and cdev.get("build_generate") is True
            and cdev.get("build_clean") is True
            and cdev.get("build_cmake") is True
            and cdev.get("build_docs") is not True
        ),
    ))

    fulldev = {"build_fulldev": True}
    apply_build_profile_flags(fulldev)
    tests.append(_case(
        "fulldev-install",
        "--fulldev enables --install, --generate, --clean, --all, --scan",
        passed=(
            fulldev.get("build_install") is True
            and fulldev.get("build_generate") is True
            and fulldev.get("build_clean") is True
            and fulldev.get("build_all") is True
            and fulldev.get("build_scan") is True
            and fulldev.get("build_cmake") is True
            and fulldev.get("build_docs") is True
        ),
    ))

    all_only = {"build_all": True}
    apply_build_profile_flags(all_only)
    tests.append(_case(
        "all-does-not-install",
        "--all does not enable --install or --generate",
        passed=(
            all_only.get("build_install") is not True
            and all_only.get("build_generate") is not True
            and all_only.get("build_cmake") is True
        ),
    ))

    keepers = (
        "afw_pool.h",
        "afw_strings.h",
        "afw_data_type_bindings.h",
        "afw_associative_array_template.h",
    )
    leftovers = (
        "afw_pool_internal.h",
        "afw_declare_helpers.h",
        "afw_function_bindings.h",
        "afw_const_objects.h",
        "afw_generated.h",
        "afw_log_deprecated.h",
        "afw_log_deprecated_interface.h",
        "afw_model_location.h",
        "afw_array_template.h",
        "skeleton_adapter.h",
        "afw_curl_declare_helpers.h",
    )
    classified = (
        is_leftover_installed_header("afw_pool.h") is False
        and is_leftover_installed_header("afw_data_type_bindings.h") is False
        and is_leftover_installed_header("afw_pool_internal.h") is True
        and is_leftover_installed_header("afw_function_bindings.h") is True
        and is_leftover_installed_header("afw_declare_helpers.h") is True
        and is_leftover_installed_header("skeleton_adapter.h") is True
    )
    tests.append(_case(
        "classify-leftover-headers",
        "denylist and old generated names are leftovers; current public are not",
        passed=classified,
    ))

    work = tempfile.mkdtemp(prefix="afw_prune_headers_")
    try:
        for name in keepers + leftovers:
            with open(os.path.join(work, name), "w", encoding="utf-8") as f:
                f.write("/* " + name + " */\n")
        removed = prune_leftover_installed_headers(work)
        removed_names = {os.path.basename(p) for p in removed}
        kept = all(os.path.isfile(os.path.join(work, n)) for n in keepers)
        gone = all(not os.path.isfile(os.path.join(work, n)) for n in leftovers)
        tests.append(_case(
            "prune-leftover-headers",
            "install prune removes denylist and old generated names, keeps public",
            passed=(
                kept
                and gone
                and removed_names == set(leftovers)
            ),
        ))
    finally:
        shutil.rmtree(work, ignore_errors=True)

    return {
        "description":
            "--cdev/--fulldev install flags and leftover installed header prune",
        "tests": tests,
    }

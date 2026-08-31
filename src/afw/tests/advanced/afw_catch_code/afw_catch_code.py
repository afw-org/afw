#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
AFW_CATCH(code) matches the thrown error code (issue #270).

The macro expansion cannot be exercised from Adaptive Script - it is a
property of the AFW_TRY/AFW_CATCH/AFW_ENDTRY C preprocessor macros - so
this is a C probe over the public afw_error.h contract.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "afw_catch_code_probe.c",
        "AFW_CATCH(code) matches the thrown error code",
        [
            (
                "matched",
                "AFW_CATCH(not_found) catches a thrown not_found",
            ),
            (
                "unmatched_falls_through",
                "AFW_CATCH(memory) does not catch not_found; "
                "it propagates to the outer try",
            ),
            (
                "second_catch_matches",
                "a later AFW_CATCH(memory) catches when an earlier "
                "AFW_CATCH(not_found) does not",
            ),
        ],
    )

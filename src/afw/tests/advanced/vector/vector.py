#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afw_vector grow, push/pop, insert/remove, copy.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "vector_probe.c",
        "afw_vector typed overlay behaves like apr_array",
        [
            (
                "push_grow",
                "push past initial capacity keeps values",
            ),
            (
                "insert_remove",
                "insert and remove memmove and preserve order",
            ),
            (
                "pop_clear",
                "pop returns last; clear keeps capacity",
            ),
            (
                "copy",
                "copy has independent storage",
            ),
            (
                "copy_entries",
                "copy-out is an exact-sized block",
            ),
            (
                "copy_entries_and_release",
                "copy-out then release the work vector",
            ),
            (
                "underflow",
                "pop of empty throws",
            ),
            (
                "bad_index",
                "insert/remove past end throw",
            ),
        ],
    )

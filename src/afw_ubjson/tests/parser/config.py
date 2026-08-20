#!/usr/bin/env python3

import glob
import os
import struct

# test configuration settings
Environment = "ubjson-parser"

# UBJSON marker bytes (see src/afw_ubjson/afw_ubjson.h).
MARKER_INT8 = b"i"
MARKER_STRING = b"S"
MARKER_ARRAY_OPEN = b"["
MARKER_ARRAY_CLOSE = b"]"
MARKER_OBJECT_OPEN = b"{"
MARKER_OBJECT_CLOSE = b"}"

# Depth chosen to match cJSON's CJSON_NESTING_LIMIT default (see
# AFW_UBJSON_MAX_DEPTH in afw_ubjson_to_value.c).
MAX_DEPTH = 1000


def ubjson_key(name):
    """Encode a UBJSON object key: <int8 length><raw bytes>, no marker."""
    raw = name.encode("utf-8")
    return MARKER_INT8 + struct.pack(">b", len(raw)) + raw


def ubjson_string(value):
    """Encode a UBJSON string value: S<int8 length><raw bytes>."""
    raw = value.encode("utf-8")
    return MARKER_STRING + MARKER_INT8 + struct.pack(">b", len(raw)) + raw


def write_fixture(name, body):
    path = os.path.join("objects", "Test", name + ".ubjson")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(body)


def build_fixtures():
    # valid: a well-formed { "a": "hi" } object -- baseline sanity, would
    # pass before and after the fix.
    valid = (
        MARKER_OBJECT_OPEN
        + ubjson_key("a")
        + ubjson_string("hi")
        + MARKER_OBJECT_CLOSE
    )
    write_fixture("valid", valid)

    # u1_truncated: the same object with the closing '}' cut off. Once the
    # parser's cursor reaches the (now shorter) end of input, the pre-fix
    # `cursor > size` check let cursor == size through and read one byte
    # past the buffer instead of throwing.
    write_fixture("u1_truncated", valid[:-1])

    # u2_negative_length: object with one key "a" whose string value's
    # length marker is INT8 = -1. Pre-fix, this sign-extends to a huge
    # size_t, the cursor+len bounds check overflows back into range, and
    # afw_utf8_create() is called with len close to SIZE_MAX -- effectively
    # "read to the end of the address space".
    u2 = (
        MARKER_OBJECT_OPEN
        + ubjson_key("a")
        + MARKER_STRING
        + MARKER_INT8
        + struct.pack(">b", -1)
    )
    write_fixture("u2_negative_length", u2)

    # u3_deep_nesting: { "a": [[[[...]]]] } with well beyond MAX_DEPTH
    # levels of array nesting. Pre-fix, parse_value/parse_array recurse
    # with no depth limit at all and exhaust the C stack.
    levels = MAX_DEPTH * 5
    u3 = (
        MARKER_OBJECT_OPEN
        + ubjson_key("a")
        + MARKER_ARRAY_OPEN * levels
        + MARKER_ARRAY_CLOSE * levels
        + MARKER_OBJECT_CLOSE
    )
    write_fixture("u3_deep_nesting", u3)


def cleanup():
    for f in glob.glob(os.path.join("objects", "Test", "*.ubjson")):
        os.remove(f)


def before_all():
    cleanup()
    build_fixtures()


def after_all():
    cleanup()

#!/usr/bin/env python3

##
# @file printf_scan.py
# @ingroup afwdev_build
# @brief Typed AFW printf format/argument check via libclang.
# @details Walks compile_commands.json, finds AFW printf-family calls
#          (after macro expansion), rebuilds literal formats, and checks
#          argument count and types. Intended for `afwdev build --scan`.
#

import ctypes
import json
import os
import shlex
import time

from _afwdev.common import msg

# CallExpr / StringLiteral (clang-c/Index.h).
_CXCursor_CallExpr = 103
_CXCursor_StringLiteral = 109
_CXChildVisit_Break = 0
_CXChildVisit_Recurse = 2
_CXTranslationUnit_KeepGoing = 0x200

# format_index, first_vararg_index (0-based).
_FUNCS = {
    "afw_utf8_printf_as": (2, 4),
    "afw_utf8_z_printf_as": (2, 4),
    "afw_utf8_snprintf_as": (3, 5),
    "afw_utf8_z_snprintf_as": (3, 5),
    "afw_utf8_printf_len_as": (1, 3),
    "afw_utf8_z_printf_len_as": (1, 3),
    "afw_error_set_fz": (3, 4),
    "afw_error_rv_set_fz": (5, 6),
    "afw_error_message": (1, 2),
    "afw_trace_write_fz": (4, 5),
    "afw_log_write_fz": (4, 5),
    "afw_debug_write_fz": (3, 4),
}


class _CXString(ctypes.Structure):
    _fields_ = [("data", ctypes.c_void_p), ("private_flags", ctypes.c_uint)]


class _CXCursor(ctypes.Structure):
    _fields_ = [
        ("kind", ctypes.c_int),
        ("xdata", ctypes.c_int),
        ("data", ctypes.c_void_p * 3),
    ]


class _CXType(ctypes.Structure):
    _fields_ = [("kind", ctypes.c_int), ("data", ctypes.c_void_p * 2)]


class _CXUnsaved(ctypes.Structure):
    _fields_ = [
        ("Filename", ctypes.c_char_p),
        ("Contents", ctypes.c_char_p),
        ("Length", ctypes.c_ulong),
    ]


class _CXSourceLocation(ctypes.Structure):
    _fields_ = [("ptr_data", ctypes.c_void_p * 2), ("int_data", ctypes.c_uint)]


_CXCursorVisitor = ctypes.CFUNCTYPE(
    ctypes.c_int, _CXCursor, _CXCursor, ctypes.c_void_p)


def _load_libclang():
    names = (
        "libclang-14.so.1",
        "libclang.so.1",
        "libclang.so",
        "libclang-15.so.1",
        "libclang-16.so.1",
        "libclang-17.so.1",
        "libclang-18.so.1",
    )
    last = None
    for name in names:
        try:
            return ctypes.CDLL(name)
        except OSError as e:
            last = e
    raise last


def _bind(lib):
    lib.clang_createIndex.restype = ctypes.c_void_p
    lib.clang_createIndex.argtypes = [ctypes.c_int, ctypes.c_int]
    lib.clang_parseTranslationUnit.restype = ctypes.c_void_p
    lib.clang_parseTranslationUnit.argtypes = [
        ctypes.c_void_p, ctypes.c_char_p,
        ctypes.POINTER(ctypes.c_char_p), ctypes.c_int,
        ctypes.POINTER(_CXUnsaved), ctypes.c_uint, ctypes.c_uint,
    ]
    lib.clang_disposeTranslationUnit.argtypes = [ctypes.c_void_p]
    lib.clang_disposeIndex.argtypes = [ctypes.c_void_p]
    lib.clang_getTranslationUnitCursor.restype = _CXCursor
    lib.clang_getTranslationUnitCursor.argtypes = [ctypes.c_void_p]
    lib.clang_visitChildren.restype = ctypes.c_uint
    lib.clang_visitChildren.argtypes = [
        _CXCursor, _CXCursorVisitor, ctypes.c_void_p]
    lib.clang_getCursorKind.argtypes = [_CXCursor]
    lib.clang_getCursorKind.restype = ctypes.c_int
    lib.clang_getCursorSpelling.restype = _CXString
    lib.clang_getCursorSpelling.argtypes = [_CXCursor]
    lib.clang_getCString.restype = ctypes.c_char_p
    lib.clang_getCString.argtypes = [_CXString]
    lib.clang_disposeString.argtypes = [_CXString]
    lib.clang_getCursorReferenced.restype = _CXCursor
    lib.clang_getCursorReferenced.argtypes = [_CXCursor]
    lib.clang_Cursor_getNumArguments.restype = ctypes.c_int
    lib.clang_Cursor_getNumArguments.argtypes = [_CXCursor]
    lib.clang_Cursor_getArgument.restype = _CXCursor
    lib.clang_Cursor_getArgument.argtypes = [_CXCursor, ctypes.c_uint]
    lib.clang_getCursorType.restype = _CXType
    lib.clang_getCursorType.argtypes = [_CXCursor]
    lib.clang_getTypeSpelling.restype = _CXString
    lib.clang_getTypeSpelling.argtypes = [_CXType]
    lib.clang_getCanonicalType.restype = _CXType
    lib.clang_getCanonicalType.argtypes = [_CXType]
    lib.clang_getPointeeType.restype = _CXType
    lib.clang_getPointeeType.argtypes = [_CXType]
    lib.clang_getCursorLocation.restype = _CXSourceLocation
    lib.clang_getCursorLocation.argtypes = [_CXCursor]
    lib.clang_getSpellingLocation.argtypes = [
        _CXSourceLocation,
        ctypes.POINTER(ctypes.c_void_p),
        ctypes.POINTER(ctypes.c_uint),
        ctypes.POINTER(ctypes.c_uint),
        ctypes.POINTER(ctypes.c_uint),
    ]
    lib.clang_getFileName.restype = _CXString
    lib.clang_getFileName.argtypes = [ctypes.c_void_p]
    return lib


def _cstr(lib, cx):
    p = lib.clang_getCString(cx)
    s = p.decode("utf-8", "replace") if p else ""
    lib.clang_disposeString(cx)
    return s


def decode_c_string_spelling(spelling):
    """Decode a clang StringLiteral spelling (`\"foo\\n\"`) to bytes-as-str."""
    if not spelling or spelling[0] != '"':
        return None
    out = []
    i = 1
    while i < len(spelling):
        c = spelling[i]
        if c == '"':
            break
        if c != "\\":
            out.append(c)
            i += 1
            continue
        i += 1
        if i >= len(spelling):
            break
        e = spelling[i]
        i += 1
        if e == "n":
            out.append("\n")
        elif e == "t":
            out.append("\t")
        elif e == "r":
            out.append("\r")
        elif e in '\\"\'':
            out.append(e)
        elif e == "x":
            hexv = ""
            while i < len(spelling) and spelling[i] in "0123456789abcdefABCDEF":
                hexv += spelling[i]
                i += 1
                if len(hexv) == 2:
                    break
            if hexv:
                out.append(chr(int(hexv, 16)))
        elif e.isdigit():
            octv = e
            while i < len(spelling) and spelling[i].isdigit() and len(octv) < 3:
                octv += spelling[i]
                i += 1
            out.append(chr(int(octv, 8)))
        else:
            out.append(e)
    return "".join(out)


def parse_format(fmt):
    """Return list of expected slots: ('ku',), ('int',), ('cstr',), ...

    Mirrors impl_format_content conversions. Raises ValueError on a
    broken spec.
    """
    slots = []
    i = 0
    n = len(fmt)
    while i < n:
        if fmt[i] != "%":
            i += 1
            continue
        if i + 1 < n and fmt[i + 1] == "%":
            i += 2
            continue
        # AFW_UTF8_FMT: %.*s copies n bytes (int + pointer).
        if fmt[i:i + 4] == "%.*s":
            slots.append("int")
            slots.append("cstr")
            i += 4
            continue
        i += 1
        while i < n and fmt[i] in "-+ #0'":
            i += 1
        star_w = False
        star_p = False
        if i < n and fmt[i] == "*":
            star_w = True
            i += 1
        else:
            while i < n and fmt[i].isdigit():
                i += 1
        if i < n and fmt[i] == ".":
            i += 1
            if i < n and fmt[i] == "*":
                star_p = True
                i += 1
            else:
                while i < n and fmt[i].isdigit():
                    i += 1
        if i < n and fmt[i] == "$":
            raise ValueError("positional $ is not supported")
        if i < n and fmt[i] == "h":
            i += 1
            if i < n and fmt[i] == "h":
                i += 1
        elif i < n and fmt[i] == "l":
            i += 1
            if i < n and fmt[i] == "l":
                i += 1
        elif i < n and fmt[i] in "Lztjq":
            i += 1
        elif i < n and fmt[i] == "w":
            raise ValueError("%w length modifiers are not supported")
        if i >= n:
            raise ValueError("incomplete conversion")
        conv = fmt[i]
        i += 1
        if star_w:
            slots.append("int")
        if star_p:
            slots.append("int")
        if conv == "k":
            if i >= n:
                raise ValueError("%k alone is not a conversion")
            kind = fmt[i]
            i += 1
            if kind == "u":
                slots.append("utf8_ptr")
            elif kind == "m":
                slots.append("memory_ptr")
            elif kind == "s":
                slots.append("cstr")
            else:
                raise ValueError("unknown %k kind")
            continue
        if conv in "nSC":
            raise ValueError("unsupported conversion %" + conv)
        if conv == "m":
            raise ValueError("unsupported conversion %m")
        if conv == "s":
            slots.append("cstr")
        elif conv == "c":
            slots.append("int")
        elif conv == "p":
            slots.append("ptr")
        elif conv in "di":
            slots.append("int")
        elif conv in "uoxX":
            slots.append("uint")
        elif conv in "fFeEgGaA":
            slots.append("float")
        else:
            raise ValueError("unknown conversion %" + conv)
    return slots


def _norm_type(spelling):
    s = spelling.replace("const ", " ").replace("volatile ", " ")
    s = s.replace("restrict ", " ").replace("_Atomic ", " ")
    return " ".join(s.split())


def _pointee_name(spelling):
    s = _norm_type(spelling)
    if not s.endswith("*"):
        return None
    return s[:-1].strip()


def _is_null_type(spelling):
    s = _norm_type(spelling)
    return s in ("void *", "void*", "const void *")


def _is_ptr_to(spelling, type_name):
    p = _pointee_name(spelling)
    if p is None:
        return False
    # "struct afw_utf8_s" or "afw_utf8_t"
    return p == type_name or p.endswith(type_name) or type_name in p.split()


def _is_cstr(spelling):
    p = _pointee_name(spelling)
    if p is None:
        return False
    if _is_null_type(spelling):
        return True
    return p in (
        "char", "unsigned char", "signed char",
        "afw_utf8_z_t", "afw_utf8_octet_t",
    ) or p.endswith("char")


def _is_intish(spelling):
    s = _norm_type(spelling)
    if "*" in s:
        return False
    keys = (
        "int", "long", "short", "char", "bool", "_Bool",
        "size_t", "ssize_t", "ptrdiff_t", "intptr_t", "uintptr_t",
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "afw_integer_t", "afw_size_t", "afw_byte_t",
        "apr_size_t", "apr_int64_t", "apr_uint64_t",
        "enum",
    )
    return any(k in s for k in keys)


def _is_floatish(spelling):
    s = _norm_type(spelling)
    if "*" in s:
        return False
    return any(k in s for k in ("float", "double"))


def _is_any_ptr(spelling):
    return "*" in _norm_type(spelling) or _is_null_type(spelling)


def type_ok(slot, spelling):
    if slot == "utf8_ptr":
        return _is_null_type(spelling) or _is_ptr_to(spelling, "afw_utf8_t")
    if slot == "memory_ptr":
        return _is_null_type(spelling) or _is_ptr_to(spelling, "afw_memory_t")
    if slot == "cstr":
        return _is_cstr(spelling) or _is_null_type(spelling)
    if slot == "int":
        return _is_intish(spelling)
    if slot == "uint":
        return _is_intish(spelling)
    if slot == "float":
        return _is_floatish(spelling)
    if slot == "ptr":
        return _is_any_ptr(spelling)
    return False


def _compile_args(entry):
    parts = shlex.split(entry["command"])
    parts = parts[1:]
    src = os.path.abspath(entry["file"])
    out = []
    skip_next = False
    for p in parts:
        if skip_next:
            skip_next = False
            continue
        if p == "-c":
            continue
        if p == "-o":
            skip_next = True
            continue
        if os.path.abspath(p) == src:
            continue
        out.append(p)
    return out


def _spelling_loc(lib, cur):
    file_p = ctypes.c_void_p()
    line = ctypes.c_uint()
    col = ctypes.c_uint()
    off = ctypes.c_uint()
    lib.clang_getSpellingLocation(
        lib.clang_getCursorLocation(cur),
        ctypes.byref(file_p), ctypes.byref(line),
        ctypes.byref(col), ctypes.byref(off))
    name = ""
    if file_p:
        name = _cstr(lib, lib.clang_getFileName(file_p))
    return name, int(line.value), int(col.value)


def _extract_string(lib, cur):
    kind = lib.clang_getCursorKind(cur)
    if kind == _CXCursor_StringLiteral:
        return decode_c_string_spelling(
            _cstr(lib, lib.clang_getCursorSpelling(cur)))
    box = []

    @_CXCursorVisitor
    def vis(c, p, d):
        if lib.clang_getCursorKind(c) == _CXCursor_StringLiteral:
            s = decode_c_string_spelling(
                _cstr(lib, lib.clang_getCursorSpelling(c)))
            if s is not None:
                box.append(s)
        return _CXChildVisit_Recurse

    lib.clang_visitChildren(cur, vis, None)
    if not box:
        return None
    return "".join(box)


def scan_compile_commands(cdb_path, src_root):
    """Return (issues, files_parsed, seconds). issues are dicts."""
    lib = _bind(_load_libclang())
    with open(cdb_path) as f:
        cdb = json.load(f)
    index = lib.clang_createIndex(0, 0)
    issues = []
    seen = set()
    files_parsed = 0
    t0 = time.time()
    src_root = os.path.abspath(src_root)

    for entry in cdb:
        path = entry.get("file") or ""
        if not path.endswith(".c"):
            continue
        args = _compile_args(entry)
        arr = (ctypes.c_char_p * len(args))(
            *[a.encode("utf-8") for a in args])
        tu = lib.clang_parseTranslationUnit(
            index, path.encode("utf-8"), arr, len(args),
            None, 0, _CXTranslationUnit_KeepGoing)
        if not tu:
            issues.append({
                "file": path,
                "line": 0,
                "message": "libclang failed to parse translation unit",
            })
            continue
        files_parsed += 1
        state = {"lib": lib, "issues": issues, "seen": seen,
                 "src_root": src_root}

        @_CXCursorVisitor
        def visitor(cur, parent, data):
            return _visit(state, cur)

        root = lib.clang_getTranslationUnitCursor(tu)
        lib.clang_visitChildren(root, visitor, None)
        lib.clang_disposeTranslationUnit(tu)

    lib.clang_disposeIndex(index)
    return issues, files_parsed, time.time() - t0


def _visit(state, cur):
    lib = state["lib"]
    if lib.clang_getCursorKind(cur) != _CXCursor_CallExpr:
        return _CXChildVisit_Recurse
    ref = lib.clang_getCursorReferenced(cur)
    name = _cstr(lib, lib.clang_getCursorSpelling(ref))
    spec = _FUNCS.get(name)
    if spec is None:
        return _CXChildVisit_Recurse
    fmt_i, var_i = spec
    nargs = lib.clang_Cursor_getNumArguments(cur)
    if nargs <= fmt_i:
        return _CXChildVisit_Recurse
    fmt_cur = lib.clang_Cursor_getArgument(cur, fmt_i)
    fmt = _extract_string(lib, fmt_cur)
    file, line, col = _spelling_loc(lib, cur)
    if not file or file.startswith("/usr/"):
        return _CXChildVisit_Recurse
    key_prefix = (file, line, name, fmt)
    if fmt is None:
        return _CXChildVisit_Recurse
    try:
        slots = parse_format(fmt)
    except ValueError as e:
        key = key_prefix + ("parse", str(e))
        if key not in state["seen"]:
            state["seen"].add(key)
            state["issues"].append({
                "file": file,
                "line": line,
                "message": "%s format: %s" % (name, e),
            })
        return _CXChildVisit_Recurse
    have = nargs - var_i
    if have < 0:
        have = 0
    need = len(slots)
    if have != need:
        key = key_prefix + ("count", have, need)
        if key not in state["seen"]:
            state["seen"].add(key)
            state["issues"].append({
                "file": file,
                "line": line,
                "message": "%s format %r needs %d arg(s), call has %d" % (
                    name, fmt, need, have),
            })
        return _CXChildVisit_Recurse
    for j, slot in enumerate(slots):
        arg = lib.clang_Cursor_getArgument(cur, var_i + j)
        ty = _cstr(lib, lib.clang_getTypeSpelling(
            lib.clang_getCursorType(arg)))
        if type_ok(slot, ty):
            continue
        key = key_prefix + ("type", j, slot, ty)
        if key not in state["seen"]:
            state["seen"].add(key)
            state["issues"].append({
                "file": file,
                "line": line,
                "message": "%s arg %d: format wants %s, have %s" % (
                    name, j + 1, slot, ty),
            })
    return _CXChildVisit_Recurse


def run_printf_scan(options):
    """Run the scan; msg.error_exit on issues or missing libclang."""
    pkg = options.get("afw_package_dir_path") or os.getcwd()
    cdb = os.path.join(pkg, "build", "cmake", "compile_commands.json")
    if not os.path.isfile(cdb):
        msg.error_exit(
            "printf scan needs build/cmake/compile_commands.json "
            "(cmake configure with CMAKE_EXPORT_COMPILE_COMMANDS)")
    try:
        _load_libclang()
    except OSError:
        msg.error_exit(
            "printf scan needs libclang (e.g. libclang-14.so.1)")
    msg.highlighted_info("Running AFW printf format/argument scan")
    issues, nfiles, dt = scan_compile_commands(cdb, pkg)
    msg.info("printf scan: %d translation units in %.1fs" % (nfiles, dt))
    if not issues:
        msg.success("printf scan: no format/argument mismatches")
        return
    for iss in issues:
        rel = iss["file"]
        if rel.startswith(pkg):
            rel = os.path.relpath(rel, pkg)
        msg.info("%s:%d: %s" % (rel, iss["line"], iss["message"]))
    msg.error_exit("printf scan failed. Number of bugs detected: %d" % (
        len(issues),))

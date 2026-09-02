#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##
# @file valgrind_report.py
# @ingroup afwdev_test
# @brief Turn valgrind XML into a short console report.
# @details --env-mode valgrind used to fail with only
#          "Valgrind Error(s) detected." The XML was on stderr and only
#          printed under --debug. summarize_valgrind_xml() is the default
#          fail text (kind + top frames). Full XML stays --debug.
#

import re
import xml.etree.ElementTree as ET


def _local(tag):
    if not tag:
        return ""
    if "}" in tag:
        return tag.rsplit("}", 1)[-1]
    return tag


def _text(el):
    if el is None or el.text is None:
        return ""
    return el.text.strip()


def _child(el, name):
    for child in list(el):
        if _local(child.tag) == name:
            return child
    return None


def _frame_line(frame):
    fn = _text(_child(frame, "fn")) or "?"
    src = _text(_child(frame, "file"))
    line = _text(_child(frame, "line"))
    obj = _text(_child(frame, "obj"))
    where = ""
    if src and line:
        where = " ({}:{})".format(src, line)
    elif src:
        where = " ({})".format(src)
    elif obj:
        where = " ({})".format(obj)
    return "    at {}{}".format(fn, where)


def _error_block(err, max_frames):
    kind = _text(_child(err, "kind")) or "unknown"
    what = _text(_child(err, "what"))
    if not what:
        xwhat = _child(err, "xwhat")
        what = _text(_child(xwhat, "text")) if xwhat is not None else ""
    lines = ["Valgrind {}: {}".format(kind, what or kind)]
    stack = _child(err, "stack")
    if stack is None:
        return lines
    n = 0
    for frame in list(stack):
        if _local(frame.tag) != "frame":
            continue
        lines.append(_frame_line(frame))
        n += 1
        if n >= max_frames:
            break
    return lines


def summarize_valgrind_xml(text, max_errors=2, max_frames=8):
    """Short kind + stack from valgrind XML. None if there is no <error>."""
    if not text or "<error>" not in text:
        return None
    blocks = []
    try:
        # Valgrind XML is a document; stderr may have noise around it.
        start = text.find("<?xml")
        if start < 0:
            start = text.find("<valgrindoutput")
        xml_text = text[start:] if start >= 0 else text
        root = ET.fromstring(xml_text)
        count = 0
        for el in root.iter():
            if _local(el.tag) != "error":
                continue
            blocks.extend(_error_block(el, max_frames))
            count += 1
            if count >= max_errors:
                break
    except (ET.ParseError, UnicodeError, ValueError):
        blocks = _summarize_valgrind_xml_loose(text, max_errors, max_frames)
    if not blocks:
        return "Valgrind Error(s) detected."
    return "\n".join(blocks)


def _summarize_valgrind_xml_loose(text, max_errors, max_frames):
    blocks = []
    errors = re.findall(r"<error>(.*?)</error>", text, flags=re.DOTALL)
    for raw in errors[:max_errors]:
        kind_m = re.search(r"<kind>([^<]+)</kind>", raw)
        kind = kind_m.group(1) if kind_m else "unknown"
        what_m = re.search(r"<what>([^<]+)</what>", raw)
        what = what_m.group(1) if what_m else kind
        lines = ["Valgrind {}: {}".format(kind, what)]
        frames = re.findall(
            r"<frame>.*?<fn>([^<]*)</fn>(?:.*?<file>([^<]*)</file>)?"
            r"(?:.*?<line>([^<]*)</line>)?",
            raw,
            flags=re.DOTALL,
        )
        for fn, src, line in frames[:max_frames]:
            where = ""
            if src and line:
                where = " ({}:{})".format(src, line)
            elif src:
                where = " ({})".format(src)
            lines.append("    at {}{}".format(fn or "?", where))
        blocks.extend(lines)
    return blocks


def valgrind_error_message(xml_text):
    """Error string for a test fail. Always names valgrind if XML has <error>."""
    summary = summarize_valgrind_xml(xml_text)
    if summary:
        return summary
    return "Valgrind Error(s) detected."

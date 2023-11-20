#!/usr/bin/env python3

##
# @file nfc.py
# @ingroup afwdev_common
# @brief This file contains some common functions for normalizing unicode.
#

import json
import unicodedata
import builtins

def open(path, mode='r'):
    # If not binary mode, open with utf-8 encoding and newline=''.
    if 'b' not in mode:
        fd = builtins.open(path, mode=mode, encoding='utf-8', newline='')
    else:
        fd = builtins.open(path, mode=mode)
    return fd

# Normalize the strings in a list, dictionary, or string to NFC.
def normalize_value(val):
    if isinstance(val, list):
        return [normalize_value(x) for x in val]
    elif isinstance(val, dict):
        return {normalize_value(k): normalize_value(v) for k, v in val.items()}
    elif isinstance(val, str):
        return unicodedata.normalize('NFC', val)
    else:
        return val
    
def json_dump(obj, fp, *args, **kwargs):
    obj_normalized = normalize_value(obj)
    json.dump(obj_normalized, fp, *args, **kwargs)

def json_dumps(obj, *args, **kwargs):
    obj_normalized = normalize_value(obj)
    return json.dumps(obj_normalized, *args, **kwargs)

def json_load(fd):
    s = fd.read()
    dict = json.loads(s)
    normalized_dict = normalize_value(dict)
    return normalized_dict

def json_loads(s):
    dict = json.loads(s)
    normalized_dict = normalize_value(dict)
    return normalized_dict

def json_load_from_file(path):
    with open(path) as fd:
        dict = json.load(fd)
        normalized_dict = normalize_value(dict)
    return normalized_dict
    
def read_path(path):
    with open(path) as fd:
        s = fd.read()
        s = unicodedata.normalize('NFC', s)
        return s

def read_line(fp):
    s = fp.readline()
    if s:
        s = unicodedata.normalize('NFC', s)
    return s

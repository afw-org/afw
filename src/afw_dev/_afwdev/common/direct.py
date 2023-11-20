#! /usr/bin/env python3

##
# @file direct.py
# @ingroup afwdev_common
# @brief This file contains some common functions for direct file object 
#        retrieval.
#

import os
import fnmatch
from _afwdev.common import msg, nfc

def retrieve_objects_direct(dir, objectType = None):
    list = []

    parts = dir.replace('\\', '/').split('/')
    count = len(parts);
    if count > 0:
        if parts[count-1] != "":
            objectType = parts[count-1]
        else:
            if count > 1:
                objectType = parts[count-2]

    for file in sorted(os.listdir(dir)):
        if fnmatch.fnmatch(file, '*.json'):
            with nfc.open(dir + file, 'r') as fd:
                content = fd.read()
                try:
                    obj = nfc.json_loads(content)
                except:
                    print("Unable to load JSON content from: " + dir + file)
                    raise
                obj['_meta_'] = {}
                obj['_meta_']['objectId'] = file.replace('.json', '')
                if objectType is not None:
                    obj['_meta_']['objectType'] = objectType
                list.append(obj)

    return list

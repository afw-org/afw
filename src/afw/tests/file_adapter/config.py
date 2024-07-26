#!/usr/bin/env python3

import glob
import os

# test configuration settings
Environment = "afw"

ignore = [
    "objects/_AdaptiveObjectType_/TestObjectType1.json",
    "objects/_AdaptiveObjectType_/TestObjectType2.json",
    "objects/_AdaptiveObjectType_/TestObjectType3.json",
    "objects/TestObjectType1/Test1.json"
]

def cleanup():
    # cleanup any object types
    for f in glob.glob("objects/_AdaptiveObjectType_/*.json"):    
        if not f in ignore:
            os.remove(f)

    # cleanup any other test objects
    for f in glob.glob("objects/TestObjectType1/*.json"):    
        if not f in ignore:
            os.remove(f)

def before_each():
    cleanup()

def before_all():
    cleanup()

def after_all():
    cleanup()
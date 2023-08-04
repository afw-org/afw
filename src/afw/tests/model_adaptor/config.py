#!/usr/bin/env python3

import glob
import os

Environment = "models"

ignore = [
    "objects/_AdaptiveModel_/TestModel1.json",
    "objects/_AdaptiveObjectType_/TestObjectType1.json",
    "objects/_AdaptiveObjectType_/TestObjectType2.json",
    "objects/TestObjectType1/Test1.json",
    "objects/TestObjectType2/Test1.json"
]

def cleanup():
    # cleanup any models
    for f in glob.glob("objects/_AdaptiveModel_/*.json"):  
        if not f in ignore:
            os.remove(f)

    # cleanup any object types
    for f in glob.glob("objects/_AdaptiveObjectType_/*.json"):       
        if not f in ignore:
            os.remove(f)

    # cleanup any other test objects
    for f in glob.glob("objects/TestObjectType1/*.json"):       
        if not f in ignore:
            os.remove(f)

    for f in glob.glob("objects/TestObjectType2/*.json"):        
        if not f in ignore:
            os.remove(f)

def before_each():       
    cleanup()

def before_all():
    cleanup()

def after_all():
    cleanup()
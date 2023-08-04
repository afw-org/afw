#!/usr/bin/env python3

import os

# test configuration settings
Environment = "adaptor"

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():    
    remove_file("data.mdb")    
    remove_file("lock.mdb")

def after_all():
    cleanup()

def before_all():
    cleanup()

def before_each():
    cleanup()
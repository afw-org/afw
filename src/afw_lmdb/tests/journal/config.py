#!/usr/bin/env python3

import os

# test configuration settings
Environment = "journal"

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():        
    remove_file("adaptor_db/data.mdb")
    remove_file("adaptor_db/lock.mdb")
    remove_file("journal_db/data.mdb")
    remove_file("journal_db/lock.mdb")

def after_all():    
    cleanup()

def before_all():
    cleanup()

def before_each():
    cleanup()
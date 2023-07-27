#!/usr/bin/env python3

from python_bindings.adaptor import retrieve_objects_with_uri, get_object_with_uri
from afw import Session
import sys

# These tests will perform some adaptor functions to test the 
# Python Bindings. They create a session and run through a few 
# adaptor tests by importing and executing them from the 
# generated python_bindings.
def run():    

    session = None
    response = {
        "description": "Test python bindings for adaptor",
        "tests": []
    }
    sys.stderr.write('run()..\n')

    try:    
        session = Session("local")

        # first try retrieve_objects_with_uri
        passed = False
        try: 
            objects = retrieve_objects_with_uri(session, "/afw/_AdaptiveObjectType_/")
            if len(objects) > 0:
                passed = True   

        except:
            passed = False

        finally:
            response["tests"].append({
                "description": "Test python bindings for adaptor retrieve_objects_with_uri",
                "passed": passed,
            })

        # next try get_object_with_uri
        passed = False
        try:
            object = get_object_with_uri(session, "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_")
            if object is not None:
                passed = True

        except:
            passed = False

        finally:
            response["tests"].append({
                "description": "Test python bindings for adaptor get_object_with_uri",
                "passed": passed,
            })
    finally:
        sys.stderr.write('closing session..\n')
        # always make sure to close the session, or afw will hang around
        if session:
            session.close()

    # return the test results for the afwdev test runner

    sys.stderr.write('Returning response..\n')
    return response
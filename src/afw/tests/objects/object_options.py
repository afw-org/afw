#!/usr/bin/env python3

from python_bindings.adaptor import get_object_with_uri
from afw import Session

response = {
    "description": "Tests object options through the python bindings",
    "tests": []
}

adaptiveObjectTypeUri = "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_"
testObjectType1Uri = "/file/_AdaptiveObjectType_/TestObjectType1"
testObjectType2Uri = "/file/_AdaptiveObjectType_/TestObjectType2"


def test(session, name, description, uri, objectOptions, assertFn):

    error = None
    passed = False
    try:
        result = get_object_with_uri(session, uri, objectOptions)        
        if assertFn(result):
            passed = True        
    except Exception as e:        
        passed = False
        # fixme there are other Exceptions that can occur
        error = e.args[0]
    finally:
        response['tests'].append({
            "test": name,
            "description": description,
            "passed": passed,
            "error": error
        })     


def run():

    # create a local session
    session = Session("local", config='afw.conf')

    try:    

        test(
            session,
            "objectId", 
            "Tests the objectId object option",
            adaptiveObjectTypeUri,
            { "objectId": True },
            lambda result: result["_meta_"]["objectId"] == "_AdaptiveObjectType_"
        )

        test(
            session,
            "objectType", 
            "Tests the objectType object option",
            adaptiveObjectTypeUri,
            { "objectType": True },
            lambda result: result["_meta_"]["objectType"] == "_AdaptiveObjectType_"
        )

        test(
            session,
            "path", 
            "Tests the path object option",
            adaptiveObjectTypeUri,
            { "path": True },
            lambda result: result["_meta_"]["path"] == "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_"
        )

        test(
            session,
            "pathEmbedded", 
            "Tests the pathEmbedded object option",
            adaptiveObjectTypeUri,
            { "pathEmbedded": True },
            lambda result: result["propertyTypes"]["_meta_"]["path"] == "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_/propertyTypes"
        )

        test(
            session,
            "objectTypes", 
            "Tests the objectTypes object option",
            adaptiveObjectTypeUri,
            { "objectTypes": True },
            lambda result: result["_meta_"]["objectTypes"] != None
        )

        test(
            session,
            "composite options on inherited object type", 
            "Tests the composite options: composite, inheritedFrom, resolvedParentPaths",
            testObjectType2Uri,
            { "composite": True, "inheritedFrom": True, "resolvedParentPaths": True },
            lambda result: 
                result["_meta_"]["resolvedParentPaths"][0] == testObjectType1Uri and 
                result["propertyTypes"]["_meta_"]["propertyTypes"]["TestString1"]["inheritedFrom"] == (testObjectType1Uri + "/propertyTypes") and 
                result["propertyTypes"]["TestString1"] != None and 
                result["propertyTypes"]["TestString2"] != None 
        )
    finally:    
        # close our session
        session.close()

    return response
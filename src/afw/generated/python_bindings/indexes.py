# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def index_create(session, key, adaptorId=None, value=None, objectType=None, filter=None, options=None, retroactive=None, test=None):
    """
    Create index definition

    Create an index definition.

    Args:
        adaptorId (str): Id of adaptor.

        key (str): Name of the property index to be created.

        value (str): Expression to calculate the index value(s).

        objectType (str): Object Type(s) this index may apply to.

        filter (str): Expression to determine if this index applies to a
        particular object.

        options (str): Indexing options.

        retroactive (bool): Retroactively generate indexes for existing
        objects.

        test (bool): Test create (don't actually perform).

    Returns:
        dict: Object response from the indexing process
    """

    request = session.Request()

    action = {
        "function": "index_create",
        "key": key
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    if value != None:
        action['value'] = value

    if objectType != None:
        action['objectType'] = objectType

    if filter != None:
        action['filter'] = filter

    if options != None:
        action['options'] = options

    if retroactive != None:
        action['retroactive'] = retroactive

    if test != None:
        action['test'] = test

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def index_list(session, adaptorId=None, objectType=None):
    """
    List property indexes

    List property indexes

    Args:
        adaptorId (str): Id of adaptor.

        objectType (str): Id of adaptive object type indexes

    Returns:
        dict: Object response from the index repair process
    """

    request = session.Request()

    action = {
        "function": "index_list"
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    if objectType != None:
        action['objectType'] = objectType

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def index_remove(session, key, adaptorId=None):
    """
    Remove index definition

    Remove an index definition.

    Args:
        adaptorId (str): Id of adaptor.

        key (str): The index key to be removed.

    Returns:
        dict: Object response from the indexing process
    """

    request = session.Request()

    action = {
        "function": "index_remove",
        "key": key
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


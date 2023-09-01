# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def bag_null(session, values=None):
    """
    Makes an array from values

    Takes any number of null values and returns an array of array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<null>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_null(session, value):
    """
    Returns the number of values in a bag

    This returns the integer number of values in array.

    Args:
        value (list):

    Returns:
        int:
    """

    request = session.Request()

    action = {
        "function": "bag_size<null>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_null(session, value):
    """
    Checks whether value is dataType null

    Checks whether value is dataType null and return the boolean result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<null>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def null(session, value):
    """
    Convert to data type null

    Converts value to data type null returning null result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "null",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_null(session, value):
    """
    Converts value to string

    Converts null value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Args:
        value (None): A null value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<null>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


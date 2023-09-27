# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def compile_json(session, source, listing=None):
    """
    Compile json value

    Compile json value and return either an unevaluated adaptive value or a
    string containing the compiler listing.

    Args:
        source (object): json string to compile

        listing (object): If specified, a compiler listing is produced instead
        of an unevaluated compiled value.
        
        This parameter can be an integer between 0 and 10 of a string that is
        used for indentation. If 0 is specified, no whitespace is added to the
        resulting string. If 1 through 10 is specified, that number of spaces
        is used.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "compile<json>",
        "source": source
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def json(session, value):
    """
    Convert to data type json

    Converts value to data type json returning json result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "json",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def compile_relaxed_json(session, source, listing=None):
    """
    Compile relaxed_json value

    Compile relaxed_json value and return either an unevaluated adaptive value
    or a string containing the compiler listing.

    Args:
        source (object): relaxed_json string to compile

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
        "function": "compile<relaxed_json>",
        "source": source
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def relaxed_json(session, value):
    """
    Convert to data type relaxed_json

    Converts value to data type relaxed_json returning relaxed_json result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "relaxed_json",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def compile_regexp(session, source, listing=None):
    """
    Compile regexp value

    Compile regexp value and return either an unevaluated adaptive value or a
    string containing the compiler listing.

    Args:
        source (object): regexp string to compile

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
        "function": "compile<regexp>",
        "source": source
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_regexp(session, arg1, arg2):
    """
    Checks for equal

    Determine if regexp arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eq<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_regexp(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for regexp arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eqx<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_regexp(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for regexp arg1 is greater than or equal to regexp arg2 and return
    the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_regexp(session, arg1, arg2):
    """
    Checks for greater than

    Checks for regexp arg1 is greater than regexp arg2 and return the boolean
    result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_regexp(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for regexp arg1 is less than or equal to regexp arg2 and return the
    boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_regexp(session, arg1, arg2):
    """
    Checks for less than

    Checks for regexp arg1 is less that regexp arg2 and return the boolean
    result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_regexp(session, arg1, arg2):
    """
    Checks for not equal

    Determine if regexp arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type don't
    match.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ne<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_regexp(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for regexp arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "nex<regexp>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp(session, value):
    """
    Convert to data type regexp

    Converts value to data type regexp returning regexp result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "regexp",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


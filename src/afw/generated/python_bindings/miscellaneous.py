# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def compare_uri(session, uri1, uri2, isValuePath=None, currentPath=None):
    """
    Compare URIs

    Compare two URIs.

    Args:
        uri1 (str): First URI for compare. This URI can not contain an
        asterisk ('*')

        uri2 (str): Second URI for compare. This URI can contain asterisk
        ('*') for substitution if isValuePath is true and currentPath2 is
        specified.

        isValuePath (bool): The URIs are adaptive value paths. If one of the
        URIs begins with a single slash '/', both must, and each URI will be
        parsed as an adaptive value path (example:
        /adaptorId/objectType/objectId.propertyNames).

        currentPath (str): If isValuePath is true, this is the current path
        that is used to resolve a relative path in the URIs. If isValuePath is
        not true, this parameter is ignored.

    Returns:
        bool: Result of comparison.
    """

    request = session.Request()

    action = {
        "function": "compare_uri",
        "uri1": uri1,
        "uri2": uri2
    }

    if isValuePath != None:
        action['isValuePath'] = isValuePath

    if currentPath != None:
        action['currentPath'] = currentPath

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def debug(session, value, detail=None):
    """
    Conditionally write a debug value

    Conditionally, based on the detail parameter, write a value as a string to
    the debug file descriptor (usually stderr).

    Args:
        value (object): This is the value that will be converted to its string
        representation and written. An undefined value is represented by
        `<undefined>`.

        detail (bool): If true, the string will only written if the
        debug:function_active:detail flag is on. If false or not specified,
        the string will only written if the debug:function_active flag is on.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "debug",
        "value": value
    }

    if detail != None:
        action['detail'] = detail

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def execution_start_time_local(session):
    """
    The execution context (xctx) local create dataTime

    Return local dateTime when the execution context was created. This will
    usually be the start of request time.

    Args:
    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "execution_start_time_local"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def execution_start_time_utc(session):
    """
    The execution context (xctx) UTC create dataTime

    Return UTC dateTime when the execution context was created. This will
    usually be the start of request time.

    Args:
    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "execution_start_time_utc"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def generate_uuid(session):
    """
    Generate UUID

    Generate a UUID.

    Args:
    Returns:
        str:
    """

    request = session.Request()

    action = {
        "function": "generate_uuid"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_defined(session, value):
    """
    Is defined

    Test value returning boolean True if it is not undefined.

    Args:
        value (object): Value to check

    Returns:
        bool: True if value is not undefined.
    """

    request = session.Request()

    action = {
        "function": "is_defined",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_nullish(session, value):
    """
    Is nullish

    Test value returning boolean True if it is null or undefined.

    Args:
        value (object): Value to check

    Returns:
        bool: True if value is null or undefined.
    """

    request = session.Request()

    action = {
        "function": "is_nullish",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def log(session, value):
    """
    Log value

    Evaluate and convert value to String and log it.

    Args:
        value (object): Value to log.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "log",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def now_local(session):
    """
    Local dataTime

    Return current local dateTime.

    Args:
    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "now_local"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def now_utc(session):
    """
    UTC dataTime

    Return current UTC dateTime.

    Args:
    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "now_utc"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def parse_uri(session, uri, isValuePath=None, currentPath=None):
    """
    Parse URI

    Parse a URI.

    Args:
        uri (str): URI to parse

        isValuePath (bool): The URI is an adaptive value path. If the path
        begins with a single slash '/', the URI will be parsed as an adaptive
        value path (example: /adaptorId/objectType/objectId.propertyNames)

        currentPath (str): If isValuePath is true, this is the current path
        that is used to resolve relative paths. If isValuePath is not true,
        this parameter is ignored.

    Returns:
        dict: Object with results of parse.
    """

    request = session.Request()

    action = {
        "function": "parse_uri",
        "uri": uri
    }

    if isValuePath != None:
        action['isValuePath'] = isValuePath

    if currentPath != None:
        action['currentPath'] = currentPath

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def perform(session, request):
    """
    Perform actions

    Perform actions right away.

    Args:
        request (dict): See /afw/_AdaptiveObjectType_/_AdaptiveActions_ for
        more information.

    Returns:
        dict: Response object.
    """

    request = session.Request()

    action = {
        "function": "perform",
        "request": request
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def trace(session, value, filter=None, number=None):
    """
    Write a value to the a trace log

    Write a value to a trace log.

    Args:
        value (object): This is the value that will be converted to its string
        representation and written the trace log. An undefined value is
        represented by `<undefined>`.

        filter (bool): If this optional filter is false, nothing will be
        written to the trace log. The default is true.

        number (int): This is an optional number between 1 and 8 that is
        appended to 'trace' to identify the trace log. The default is 1.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "trace",
        "value": value
    }

    if filter != None:
        action['filter'] = filter

    if number != None:
        action['number'] = number

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def variable_exists(session, name):
    """
    Determine if a variable exists

    Return the true if the named variable exists.

    Args:
        name (str): Name of variable to check. The name can optionally be
        preceded with a qualifier followed by '::'.

    Returns:
        bool: True if variable exists.
    """

    request = session.Request()

    action = {
        "function": "variable_exists",
        "name": name
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def variable_get(session, name, defaultValue=None):
    """
    Get a variable value

    Return the value of a variable. If variable is not available, return a
    default or null value.

    Args:
        name (str): Name of variable to get. The name can optionally be
        preceded with a qualifier followed by '::'.

        defaultValue (object): The default value of variable if it does not
        exist in object. If not specified, null value is the default.

    Returns:
        object: Evaluated variable value or default.
    """

    request = session.Request()

    action = {
        "function": "variable_get",
        "name": name
    }

    if defaultValue != None:
        action['defaultValue'] = defaultValue

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def variable_is_not_null(session, name):
    """
    Determine if a variable exists and is not null

    Return the true if the named variable exists and is not null.

    Args:
        name (str): Name of variable to check. The name can optionally be
        preceded with a qualifier followed by '::'.

    Returns:
        bool: True if variable exists and is not null.
    """

    request = session.Request()

    action = {
        "function": "variable_is_not_null",
        "name": name
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


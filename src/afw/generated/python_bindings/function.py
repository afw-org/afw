# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def bag_function(session, values=None):
    '''
    Makes an array from values

    Takes any number of function values and returns an array of array.

    Parameters:

        values (array):

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "bag<function>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_function(session, value):
    '''
    Returns the number of values in a bag

    This returns the integer number of values in array.

    Parameters:

        value (array):

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "bag_size<function>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_function(session, arg1, arg2):
    '''
    Checks for equal

    Determine if function arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (function):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_function(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for function arg1 is equal to the value and data type of
    arg2 then return the boolean result. Use 'eq' ('==') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (function):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def function(session, value):
    '''
    

    Converts value to data type function returning function result.

    Parameters:

        value (): Value to convert

    Returns:
    function: Converted value
    '''

    request = session.Request()

    action = {
        "function": "function",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_function(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for function arg1 is greater than or equal to function arg2 and
    return the boolean result.

    Parameters:

        arg1 (function):

        arg2 (function):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_function(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for function arg1 is greater than function arg2 and return the
    boolean result.

    Parameters:

        arg1 (function):

        arg2 (function):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_function(session, value):
    '''
    Checks whether value is dataType function

    Checks whether value is dataType function and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<function>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_function(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for function arg1 is less than or equal to function arg2 and
    return the boolean result.

    Parameters:

        arg1 (function):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_function(session, arg1, arg2):
    '''
    Checks for less than

    Checks for function arg1 is less that function arg2 and return the
    boolean result.

    Parameters:

        arg1 (function):

        arg2 (function):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_function(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if function arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (function):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_function(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for function arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (function):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<function>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


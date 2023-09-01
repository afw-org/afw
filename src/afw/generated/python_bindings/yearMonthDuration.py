# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_yearMonthDuration(session, array1, array2):
    """
    Checks for at least one value in common

    Returns boolean true if at least one value in yearMonthDuration array1 is
    in yearMonthDuration array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<yearMonthDuration>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_yearMonthDuration(session, value):
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
        "function": "bag_size<yearMonthDuration>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_yearMonthDuration(session, values=None):
    """
    Makes an array from values

    Takes any number of yearMonthDuration values and returns an array of
    array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<yearMonthDuration>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_yearMonthDuration(session, arg1, arg2):
    """
    Checks for equal

    Determine if yearMonthDuration arg1 is equal to the value of arg2
    converted to the data type of arg1 then return the boolean result. Use
    'eqx' ('===') instead if you want false to be returned if arg1 and arg2's
    data type don't match.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eq<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_yearMonthDuration(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for yearMonthDuration arg1 is equal to the value and data
    type of arg2 then return the boolean result. Use 'eq' ('==') instead if
    you want arg2 to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eqx<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_yearMonthDuration(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for yearMonthDuration arg1 is greater than or equal to
    yearMonthDuration arg2 and return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_yearMonthDuration(session, arg1, arg2):
    """
    Checks for greater than

    Checks for yearMonthDuration arg1 is greater than yearMonthDuration arg2
    and return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_yearMonthDuration(session, array1, array2):
    """
    Returns intersection of two arrays

    Returns an array of yearMonthDuration with the values that are common to
    both array of yearMonthDuration array1 and array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "intersection<yearMonthDuration>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_yearMonthDuration(session, value, array):
    """
    Checks whether a value is in an array

    Checks whether yearMonthDuration value is in array of yearMonthDuration
    array and returns the boolean result.

    Args:
        value (object):

        array (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is_in<yearMonthDuration>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_yearMonthDuration(session, value):
    """
    Checks whether value is dataType yearMonthDuration

    Checks whether value is dataType yearMonthDuration and return the boolean
    result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<yearMonthDuration>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_yearMonthDuration(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for yearMonthDuration arg1 is less than or equal to
    yearMonthDuration arg2 and return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_yearMonthDuration(session, arg1, arg2):
    """
    Checks for less than

    Checks for yearMonthDuration arg1 is less that yearMonthDuration arg2 and
    return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_yearMonthDuration(session, arg1, arg2):
    """
    Checks for not equal

    Determine if yearMonthDuration arg1 is not equal to the value of arg2
    converted to the data type of arg1 then return the boolean result. Use
    'nex' ('!==') instead if you want true to be returned if arg1 and arg2's
    data type don't match.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ne<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_yearMonthDuration(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for yearMonthDuration arg1 is not equal to the value or data
    type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
    you want arg2 to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "nex<yearMonthDuration>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_yearMonthDuration(session, array):
    """
    Converts a one value array to a yearMonthDuration value

    This converts an array of yearMonthDuration values that contains one
    value to a single yearMonthDuration value.

    Args:
        array (list):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "one_and_only<yearMonthDuration>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_yearMonthDuration(session, array1, array2):
    """
    Checks whether two arrays are subsets of each other

    Returns boolean true if yearMonthDuration array1 and yearMonthDuration
    array2 are subsets of each other and return the boolean result.

    Args:
        array1 (list):

        array2 (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "set_equals<yearMonthDuration>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_yearMonthDuration(session, array1, array2):
    """
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in yearMonthDuration array1 are
    all in yearMonthDuration array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "subset<yearMonthDuration>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_yearMonthDuration(session, value):
    """
    Converts value to string

    Converts yearMonthDuration value to string. For array values, the
    to_string() value for each entry is returned separated with commas.

    Args:
        value (object): A yearMonthDuration value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<yearMonthDuration>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_yearMonthDuration(session, arrays):
    """
    Returns union of two or more string arrays

    Returns an array of yearMonthDuration contains all of the unique values
    in two or more array of yearMonthDuration values.

    Args:
        arrays (list): Two or more arrays.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "union<yearMonthDuration>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def yearMonthDuration(session, value):
    """
    Convert to data type yearMonthDuration

    Converts value to data type yearMonthDuration returning yearMonthDuration
    result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "yearMonthDuration",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


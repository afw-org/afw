# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def add_yearMonthDuration_date(session, arg1, arg2):
    """
    Add duration

    Add date arg1 to yearMonthDuration arg2 and return the date result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "add_yearMonthDuration<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def at_least_one_member_of_date(session, array1, array2):
    """
    Checks for at least one value in common

    Returns boolean true if at least one value in date array1 is in date
    array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<date>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_date(session, values=None):
    """
    Makes an array from values

    Takes any number of date values and returns an array of array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<date>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_date(session, value):
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
        "function": "bag_size<date>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def date(session, value):
    """
    Convert to data type date

    Converts value to data type date returning date result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "date",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_date(session, arg1, arg2):
    """
    Checks for equal

    Determine if date arg1 is equal to the value of arg2 converted to the
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
        "function": "eq<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_date(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for date arg1 is equal to the value and data type of arg2
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
        "function": "eqx<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_date(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for date arg1 is greater than or equal to date arg2 and return the
    boolean result.
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_date(session, arg1, arg2):
    """
    Checks for greater than

    Checks for date arg1 is greater than date arg2 and return the boolean
    result.
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_date(session, array1, array2):
    """
    Returns intersection of two arrays

    Returns an array of date with the values that are common to both array of
    date array1 and array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "intersection<date>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_date(session, value):
    """
    Checks whether value is dataType date

    Checks whether value is dataType date and return the boolean result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<date>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_date(session, value, array):
    """
    Checks whether a value is in an array

    Checks whether date value is in array of date array and returns the
    boolean result.

    Args:
        value (object):

        array (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is_in<date>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_date(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for date arg1 is less than or equal to date arg2 and return the
    boolean result.
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_date(session, arg1, arg2):
    """
    Checks for less than

    Checks for date arg1 is less that date arg2 and return the boolean
    result.
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def max_date(session, values):
    """
    Maximum value

    Return the date value that is greater than or equal to the others..
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        values (object):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "max<date>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def min_date(session, values):
    """
    Minimum value

    Return the date value that is less than or equal to the others..
    
    If a date value does not include a time-zone value, then the local
    time-zone value will be assigned.

    Args:
        values (object):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "min<date>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_date(session, arg1, arg2):
    """
    Checks for not equal

    Determine if date arg1 is not equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ne<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_date(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for date arg1 is not equal to the value or data type of arg2
    then return the boolean result. Use 'ne' ('!=') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "nex<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_date(session, array):
    """
    Converts a one value array to a date value

    This converts an array of date values that contains one value to a single
    date value.

    Args:
        array (list):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "one_and_only<date>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_date(session, array1, array2):
    """
    Checks whether two arrays are subsets of each other

    Returns boolean true if date array1 and date array2 are subsets of each
    other and return the boolean result.

    Args:
        array1 (list):

        array2 (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "set_equals<date>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_date(session, array1, array2):
    """
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in date array1 are all in date
    array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "subset<date>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subtract_yearMonthDuration_date(session, arg1, arg2):
    """
    Subtract numbers

    Subtract yearMonthDuration arg2 from date arg1 and return the date
    result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "subtract_yearMonthDuration<date>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_date(session, value):
    """
    Converts value to string

    Converts date value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Args:
        value (object): A date value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<date>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_date(session, arrays):
    """
    Returns union of two or more string arrays

    Returns an array of date contains all of the unique values in two or more
    array of date values.

    Args:
        arrays (list): Two or more arrays.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "union<date>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


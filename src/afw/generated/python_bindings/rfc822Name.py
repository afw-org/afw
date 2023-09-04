# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_rfc822Name(session, array1, array2):
    """
    Checks for at least one value in common

    Returns boolean true if at least one value in rfc822Name array1 is in
    rfc822Name array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<rfc822Name>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_rfc822Name(session, values=None):
    """
    Makes an array from values

    Takes any number of rfc822Name values and returns an array of array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<rfc822Name>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_rfc822Name(session, value):
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
        "function": "bag_size<rfc822Name>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_rfc822Name(session, arg1, arg2):
    """
    Checks for equal

    Determine if rfc822Name arg1 is equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'eqx' ('===')
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
        "function": "eq<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_rfc822Name(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for rfc822Name arg1 is equal to the value and data type of
    arg2 then return the boolean result. Use 'eq' ('==') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eqx<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_rfc822Name(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for rfc822Name arg1 is greater than or equal to rfc822Name arg2 and
    return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_rfc822Name(session, arg1, arg2):
    """
    Checks for greater than

    Checks for rfc822Name arg1 is greater than rfc822Name arg2 and return the
    boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_rfc822Name(session, array1, array2):
    """
    Returns intersection of two arrays

    Returns an array of rfc822Name with the values that are common to both
    array of rfc822Name array1 and array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "intersection<rfc822Name>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_rfc822Name(session, value, array):
    """
    Checks whether a value is in an array

    Checks whether rfc822Name value is in array of rfc822Name array and
    returns the boolean result.

    Args:
        value (object):

        array (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is_in<rfc822Name>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_rfc822Name(session, value):
    """
    Checks whether value is dataType rfc822Name

    Checks whether value is dataType rfc822Name and return the boolean result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<rfc822Name>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_rfc822Name(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for rfc822Name arg1 is less than or equal to rfc822Name arg2 and
    return the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_rfc822Name(session, arg1, arg2):
    """
    Checks for less than

    Checks for rfc822Name arg1 is less that rfc822Name arg2 and return the
    boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def match_rfc822Name(session, arg1, arg2):
    """
    Checks if x500Name matches

    Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
    compared using equal_rfc822Name.

    Args:
        arg1 (object):

        arg2 (str):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "match<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_rfc822Name(session, arg1, arg2):
    """
    Checks for not equal

    Determine if rfc822Name arg1 is not equal to the value of arg2 converted
    to the data type of arg1 then return the boolean result. Use 'nex' ('!==')
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
        "function": "ne<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_rfc822Name(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for rfc822Name arg1 is not equal to the value or data type of
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
        "function": "nex<rfc822Name>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_rfc822Name(session, array):
    """
    Converts a one value array to a rfc822Name value

    This converts an array of rfc822Name values that contains one value to a
    single rfc822Name value.

    Args:
        array (list):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "one_and_only<rfc822Name>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_match_rfc822Name(session, value, regexp):
    """
    Search for a match using a regular expression

    Checks whether rfc822Name value matches the regular expression regexp and
    return the boolean result.

    Args:
        value (object):

        regexp (str):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "regexp_match<rfc822Name>",
        "value": value,
        "regexp": regexp
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def rfc822Name(session, value):
    """
    Convert to data type rfc822Name

    Converts value to data type rfc822Name returning rfc822Name result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "rfc822Name",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_rfc822Name(session, array1, array2):
    """
    Checks whether two arrays are subsets of each other

    Returns boolean true if rfc822Name array1 and rfc822Name array2 are
    subsets of each other and return the boolean result.

    Args:
        array1 (list):

        array2 (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "set_equals<rfc822Name>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_rfc822Name(session, array1, array2):
    """
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in rfc822Name array1 are all in
    rfc822Name array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "subset<rfc822Name>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_rfc822Name(session, value):
    """
    Converts value to string

    Converts rfc822Name value to string. For array values, the to_string()
    value for each entry is returned separated with commas.

    Args:
        value (object): A rfc822Name value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<rfc822Name>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_rfc822Name(session, arrays):
    """
    Returns union of two or more string arrays

    Returns an array of rfc822Name contains all of the unique values in two or
    more array of rfc822Name values.

    Args:
        arrays (list): Two or more arrays.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "union<rfc822Name>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


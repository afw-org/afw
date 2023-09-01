# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def abs_double(session, value):
    """
    Absolute value

    Compute the absolute value of the double value and return the double
    result.

    Args:
        value (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "abs<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def add_double(session, values):
    """
    Add

    Add 2 or more double values and return the double result.

    Args:
        values (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "add<double>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def at_least_one_member_of_double(session, array1, array2):
    """
    Checks for at least one value in common

    Returns boolean true if at least one value in double array1 is in double
    array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<double>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_double(session, values=None):
    """
    Makes an array from values

    Takes any number of double values and returns an array of array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<double>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_double(session, value):
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
        "function": "bag_size<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ceil_double(session, value):
    """
    Round upward to nearest integer

    Determine the smallest integer that is greater then or equal to the
    double value and return the double result.

    Args:
        value (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "ceil<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def divide_double(session, dividend, divisor):
    """
    Divide numbers

    Divide double dividend by double divisor and return the double quotient.

    Args:
        dividend (float):

        divisor (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "divide<double>",
        "dividend": dividend,
        "divisor": divisor
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def double(session, value):
    """
    Convert to data type double

    Converts value to data type double returning double result.

    Args:
        value (object): Value to convert

    Returns:
        float: Converted value
    """

    request = session.Request()

    action = {
        "function": "double",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_double(session, arg1, arg2):
    """
    Checks for equal

    Determine if double arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Args:
        arg1 (float):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eq<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_double(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for double arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (float):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "eqx<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def floor_double(session, number):
    """
    Round downwards to nearest integer

    Determine the largest integer that is smaller then or equal to the double
    value and return the double result.

    Args:
        number (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "floor<double>",
        "number": number
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_double(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for double arg1 is greater than or equal to double arg2 and return
    the boolean result.

    Args:
        arg1 (float):

        arg2 (float):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_double(session, arg1, arg2):
    """
    Checks for greater than

    Checks for double arg1 is greater than double arg2 and return the boolean
    result.

    Args:
        arg1 (float):

        arg2 (float):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_double(session, array1, array2):
    """
    Returns intersection of two arrays

    Returns an array of double with the values that are common to both array
    of double array1 and array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "intersection<double>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_NaN(session, number):
    """
    Is not a number(NaN)

    Checks if the argument 'number' is not a number(NaN) and returns the
    boolean result.

    Args:
        number (float): Number to check

    Returns:
        bool: True if the argument 'number' is not a number.
    """

    request = session.Request()

    action = {
        "function": "is_NaN",
        "number": number
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_double(session, value):
    """
    Checks whether value is dataType double

    Checks whether value is dataType double and return the boolean result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_finite(session, number):
    """
    Is a number finite

    Checks if the argument 'number' is finite and returns the boolean result.

    Args:
        number (float): Number to check

    Returns:
        bool: True if the argument 'number' is finite.
    """

    request = session.Request()

    action = {
        "function": "is_finite",
        "number": number
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_double(session, value, array):
    """
    Checks whether a value is in an array

    Checks whether double value is in array of double array and returns the
    boolean result.

    Args:
        value (float):

        array (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is_in<double>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_double(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for double arg1 is less than or equal to double arg2 and return
    the boolean result.

    Args:
        arg1 (float):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_double(session, arg1, arg2):
    """
    Checks for less than

    Checks for double arg1 is less that double arg2 and return the boolean
    result.

    Args:
        arg1 (float):

        arg2 (float):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def max_double(session, values):
    """
    Maximum value

    Return the double value that is greater than or equal to the others.

    Args:
        values (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "max<double>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def min_double(session, values):
    """
    Minimum value

    Return the double value that is less than or equal to the others.

    Args:
        values (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "min<double>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def multiply_double(session, values):
    """
    Multiply numbers

    Multiply 2 or more double values and return the double result.

    Args:
        values (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "multiply<double>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_double(session, arg1, arg2):
    """
    Checks for not equal

    Determine if double arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Args:
        arg1 (float):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ne<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def negative_double(session, value):
    """
    Negative of a number

    Return negative of double value.

    Args:
        value (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "negative<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_double(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for double arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Args:
        arg1 (float):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "nex<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_double(session, array):
    """
    Converts a one value array to a double value

    This converts an array of double values that contains one value to a
    single double value.

    Args:
        array (list):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "one_and_only<double>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def pow_double(session, base, exponent):
    """
    Base raised to a power

    This returns the value of base raised to a power. Multiple exponents can
    be specified to raise the previous exponent to the power of the latter
    exponent.

    Args:
        base (float): Base value.

        exponent (float): Exponent value.

    Returns:
        float: Base raised to the power.
    """

    request = session.Request()

    action = {
        "function": "pow<double>",
        "base": base,
        "exponent": exponent
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def round_double(session, number):
    """
    Round to nearest integer

    Determine the integer closest to double value and return the double
    result.

    Args:
        number (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "round<double>",
        "number": number
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_double(session, array1, array2):
    """
    Checks whether two arrays are subsets of each other

    Returns boolean true if double array1 and double array2 are subsets of
    each other and return the boolean result.

    Args:
        array1 (list):

        array2 (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "set_equals<double>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_double(session, array1, array2):
    """
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in double array1 are all in
    double array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "subset<double>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subtract_double(session, arg1, arg2):
    """
    Subtract numbers

    Subtract double arg2 from double arg1 and return the double result.

    Args:
        arg1 (float):

        arg2 (float):

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "subtract<double>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_integer_double(session, value):
    """
    Converts to integer

    Truncate double value to a whole number and returns integer result.

    Args:
        value (float):

    Returns:
        int:
    """

    request = session.Request()

    action = {
        "function": "to_integer<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_double(session, value):
    """
    Converts value to string

    Converts double value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Args:
        value (float): A double value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<double>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_double(session, arrays):
    """
    Returns union of two or more string arrays

    Returns an array of double contains all of the unique values in two or
    more array of double values.

    Args:
        arrays (list): Two or more arrays.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "union<double>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_time(session, array1, array2):
    '''
    Checks for at least one value in common

    Returns boolean true if at least one value in time array1 is in time
    array2.

    Parameters:

        array1 (array): The first array.

        array2 (array): The second array.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<time>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_time(session, value):
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
        "function": "bag_size<time>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_time(session, values=None):
    '''
    Makes an array from values

    Takes any number of time values and returns an array of array.

    Parameters:

        values (array):

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "bag<time>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_time(session, arg1, arg2):
    '''
    Checks for equal

    Determine if time arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (time):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_time(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for time arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (time):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_time(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for time arg1 is greater than or equal to time arg2 and return the
    boolean result.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        arg1 (time):

        arg2 (time):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_time(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for time arg1 is greater than time arg2 and return the boolean
    result.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        arg1 (time):

        arg2 (time):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def in_range_time(session, time, startTime, endTime):
    '''
    Checks if time in range

    Checks if time is between startTime and endTime, inclusive. Regardless of
    endTime value, it is always considered to be equal to, but less than 24
    hours greater than startTime. If no time zone is specified for time, the
    default time zone is used. If no time zone is specified for startTime or
    endTime, the time zone of time is used.

    Parameters:

        time (time):

        startTime (time):

        endTime (time):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "in_range<time>",
        "time": time,
        "startTime": startTime,
        "endTime": endTime
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_time(session, array1, array2):
    '''
    Returns intersection of two arrays

    Returns an array of time with the values that are common to both array of
    time array1 and array2.

    Parameters:

        array1 (array): The first array.

        array2 (array): The second array.

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "intersection<time>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_time(session, value, array):
    '''
    Checks whether a value is in an array

    Checks whether time value is in array of time array and returns the
    boolean result.

    Parameters:

        value (time):

        array (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is_in<time>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_time(session, value):
    '''
    Checks whether value is dataType time

    Checks whether value is dataType time and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<time>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_time(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for time arg1 is less than or equal to time arg2 and return the
    boolean result.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        arg1 (time):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_time(session, arg1, arg2):
    '''
    Checks for less than

    Checks for time arg1 is less that time arg2 and return the boolean
    result.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        arg1 (time):

        arg2 (time):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def max_time(session, values):
    '''
    Maximum value

    Return the time value that is greater than or equal to the others.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        values (time):

    Returns:
    time: 
    '''

    request = session.Request()

    action = {
        "function": "max<time>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def min_time(session, values):
    '''
    Minimum value

    Return the time value that is less than or equal to the others.
    
    It is illegal to compare a time that includes a time-zone value with one
    that does not. In such cases, the `in_range<time>` function should be
    used.

    Parameters:

        values (time):

    Returns:
    time: 
    '''

    request = session.Request()

    action = {
        "function": "min<time>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_time(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if time arg1 is not equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (time):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_time(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for time arg1 is not equal to the value or data type of arg2
    then return the boolean result. Use 'ne' ('!=') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (time):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<time>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_time(session, array):
    '''
    Converts a one value array to a time value

    This converts an array of time values that contains one value to a single
    time value.

    Parameters:

        array (array):

    Returns:
    time: 
    '''

    request = session.Request()

    action = {
        "function": "one_and_only<time>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_time(session, array1, array2):
    '''
    Checks whether two arrays are subsets of each other

    Returns boolean true if time array1 and time array2 are subsets of each
    other and return the boolean result.

    Parameters:

        array1 (array):

        array2 (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "set_equals<time>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_time(session, array1, array2):
    '''
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in time array1 are all in time
    array2.

    Parameters:

        array1 (array): The first array.

        array2 (array): The second array.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "subset<time>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def time(session, value):
    '''
    Convert to data type time

    Converts value to data type time returning time result.

    Parameters:

        value (): Value to convert

    Returns:
    time: Converted value
    '''

    request = session.Request()

    action = {
        "function": "time",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_time(session, value):
    '''
    Converts value to string

    Converts time value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Parameters:

        value (time): A time value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<time>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_time(session, arrays):
    '''
    Returns union of two or more string arrays

    Returns an array of time contains all of the unique values in two or more
    array of time values.

    Parameters:

        arrays (array): Two or more arrays.

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "union<time>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


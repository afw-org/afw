# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_yearMonthDuration(session, list1, list2):
    '''
    Checks for at least one value in common

    Returns boolean true if at least one value in yearMonthDuration list1 is
    in yearMonthDuration list2.

    Parameters:

        list1 (array): The first list.

        list2 (array): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<yearMonthDuration>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_yearMonthDuration(session, value):
    '''
    Returns the number of values in a bag

    This returns the integer number of values in list.

    Parameters:

        value (array):

    Returns:
    integer: 
    '''

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
    '''
    Makes a list from values

    Takes any number of yearMonthDuration values and returns a list of list.

    Parameters:

        values (array):

    Returns:
    array: 
    '''

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
    '''
    Checks for equal

    Determine if yearMonthDuration arg1 is equal to the value of arg2
    converted to the data type of arg1 then return the boolean result. Use
    'eqx' ('===') instead if you want false to be returned if arg1 and arg2's
    data type don't match.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 ():

    Returns:
    boolean: 
    '''

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
    '''
    Checks for equal and type

    Determine if for yearMonthDuration arg1 is equal to the value and data
    type of arg2 then return the boolean result. Use 'eq' ('==') instead if
    you want arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 ():

    Returns:
    boolean: 
    '''

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
    '''
    Checks for greater than or equal

    Checks for yearMonthDuration arg1 is greater than or equal to
    yearMonthDuration arg2 and return the boolean result.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 (yearMonthDuration):

    Returns:
    boolean: 
    '''

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
    '''
    Checks for greater than

    Checks for yearMonthDuration arg1 is greater than yearMonthDuration arg2
    and return the boolean result.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 (yearMonthDuration):

    Returns:
    boolean: 
    '''

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

def intersection_yearMonthDuration(session, list1, list2):
    '''
    Returns intersection of two lists

    Returns a list of yearMonthDuration with the values that are common to
    both list of yearMonthDuration list1 and list2.

    Parameters:

        list1 (array): The first list.

        list2 (array): The second list.

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "intersection<yearMonthDuration>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_yearMonthDuration(session, value, array):
    '''
    Checks whether a value is in a list

    Checks whether yearMonthDuration value is in list of yearMonthDuration
    list and returns the boolean result.

    Parameters:

        value (yearMonthDuration):

        array (array):

    Returns:
    boolean: 
    '''

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
    '''
    Checks whether value is dataType yearMonthDuration

    Checks whether value is dataType yearMonthDuration and return the boolean
    result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

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
    '''
    Checks for less than or equal

    Checks for yearMonthDuration arg1 is less than or equal to
    yearMonthDuration arg2 and return the boolean result.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 ():

    Returns:
    boolean: 
    '''

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
    '''
    Checks for less than

    Checks for yearMonthDuration arg1 is less that yearMonthDuration arg2 and
    return the boolean result.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 (yearMonthDuration):

    Returns:
    boolean: 
    '''

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
    '''
    Checks for not equal

    Determine if yearMonthDuration arg1 is not equal to the value of arg2
    converted to the data type of arg1 then return the boolean result. Use
    'nex' ('!==') instead if you want true to be returned if arg1 and arg2's
    data type don't match.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 ():

    Returns:
    boolean: 
    '''

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
    '''
    Checks for not equal value or type

    Determine if for yearMonthDuration arg1 is not equal to the value or data
    type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
    you want arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (yearMonthDuration):

        arg2 ():

    Returns:
    boolean: 
    '''

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
    '''
    Converts a one value list to a yearMonthDuration value

    This converts a list of yearMonthDuration values that contains one value
    to a single yearMonthDuration value.

    Parameters:

        array (array):

    Returns:
    yearMonthDuration: 
    '''

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

def set_equals_yearMonthDuration(session, list1, list2):
    '''
    Checks whether two lists are subsets of each other

    Returns boolean true if yearMonthDuration list1 and yearMonthDuration
    list2 are subsets of each other and return the boolean result.

    Parameters:

        list1 (array):

        list2 (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "set_equals<yearMonthDuration>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_yearMonthDuration(session, list1, list2):
    '''
    Determines if the first list is a subset of second list

    Returns boolean true if the unique values in yearMonthDuration list1 are
    all in yearMonthDuration list2.

    Parameters:

        list1 (array): The first list.

        list2 (array): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "subset<yearMonthDuration>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_yearMonthDuration(session, value):
    '''
    Converts value to string

    Converts yearMonthDuration value to string. For list values, the
    to_string() value for each entry is returned separated with commas.

    Parameters:

        value (yearMonthDuration): A yearMonthDuration value.

    Returns:
    string: The string representation of the value.
    '''

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

def union_yearMonthDuration(session, lists):
    '''
    Returns union of two or more string lists

    Returns a list of yearMonthDuration contains all of the unique values in
    two or more list of yearMonthDuration values.

    Parameters:

        lists (array): Two or more lists.

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "union<yearMonthDuration>",
        "lists": lists
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def yearMonthDuration(session, value):
    '''
    Convert to data type yearMonthDuration

    Converts value to data type yearMonthDuration returning yearMonthDuration
    result.

    Parameters:

        value (): Value to convert

    Returns:
    yearMonthDuration: Converted value
    '''

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


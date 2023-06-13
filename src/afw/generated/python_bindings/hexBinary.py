# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_hexBinary(session, list1, list2):
    '''
    Checks for at least one value in common

    Returns boolean true if at least one value in hexBinary list1 is in
    hexBinary list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<hexBinary>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_hexBinary(session, values=None):
    '''
    Makes a list from values

    Takes any number of hexBinary values and returns a list of list.

    Parameters:

        values (list):

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "bag<hexBinary>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_hexBinary(session, value):
    '''
    Returns the number of values in a bag

    This returns the integer number of values in list.

    Parameters:

        value (list):

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "bag_size<hexBinary>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def decode_to_string_hexBinary(session, value):
    '''
    Decode hexBinary to string

    Decode hexBinary value to string. An error is thrown if decoded value is
    not valid UTF-8.

    Parameters:

        value (hexBinary): The hexBinary value to decode.

    Returns:
    string: The decoded string.
    '''

    request = session.Request()

    action = {
        "function": "decode_to_string<hexBinary>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_hexBinary(session, arg1, arg2):
    '''
    Checks for equal

    Determine if hexBinary arg1 is equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use "eqx" ("===")
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (hexBinary):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_hexBinary(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for hexBinary arg1 is equal to the value and data type of
    arg2 then return the boolean result. Use "eq" ("==") instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (hexBinary):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_hexBinary(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for hexBinary arg1 is greater than or equal to hexBinary arg2 and
    return the boolean result.

    Parameters:

        arg1 (hexBinary):

        arg2 (hexBinary):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_hexBinary(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for hexBinary arg1 is greater than hexBinary arg2 and return the
    boolean result.

    Parameters:

        arg1 (hexBinary):

        arg2 (hexBinary):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def hexBinary(session, value):
    '''
    Convert to data type hexBinary

    Converts value to data type hexBinary returning hexBinary result.

    Parameters:

        value (): Value to convert

    Returns:
    hexBinary: Converted value
    '''

    request = session.Request()

    action = {
        "function": "hexBinary",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_hexBinary(session, list1, list2):
    '''
    Returns intersection of two lists

    Returns a list of hexBinary with the values that are common to both list
    of hexBinary list1 and list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "intersection<hexBinary>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_hexBinary(session, value):
    '''
    Checks whether value is dataType hexBinary

    Checks whether value is dataType hexBinary and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<hexBinary>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_hexBinary(session, value, list):
    '''
    Checks whether a value is in a list

    Checks whether hexBinary value is in list of hexBinary list and returns
    the boolean result.

    Parameters:

        value (hexBinary):

        list (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is_in<hexBinary>",
        "value": value,
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_hexBinary(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for hexBinary arg1 is less than or equal to hexBinary arg2 and
    return the boolean result.

    Parameters:

        arg1 (hexBinary):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_hexBinary(session, arg1, arg2):
    '''
    Checks for less than

    Checks for hexBinary arg1 is less that hexBinary arg2 and return the
    boolean result.

    Parameters:

        arg1 (hexBinary):

        arg2 (hexBinary):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_hexBinary(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if hexBinary arg1 is not equal to the value of arg2 converted
    to the data type of arg1 then return the boolean result. Use "nex"
    ("!==") instead if you want true to be returned if arg1 and arg2's data
    type don't match.

    Parameters:

        arg1 (hexBinary):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_hexBinary(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for hexBinary arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use "ne" ("!=") instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (hexBinary):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<hexBinary>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_hexBinary(session, list):
    '''
    Converts a one value list to a hexBinary value

    This converts a list of hexBinary values that contains one value to a
    single hexBinary value.

    Parameters:

        list (list):

    Returns:
    hexBinary: 
    '''

    request = session.Request()

    action = {
        "function": "one_and_only<hexBinary>",
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_hexBinary(session, list1, list2):
    '''
    Checks whether two lists are subsets of each other

    Returns boolean true if hexBinary list1 and hexBinary list2 are subsets
    of each other and return the boolean result.

    Parameters:

        list1 (list):

        list2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "set_equals<hexBinary>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_hexBinary(session, list1, list2):
    '''
    Determines if the first list is a subset of second list

    Returns boolean true if the unique values in hexBinary list1 are all in
    hexBinary list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "subset<hexBinary>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_hexBinary(session, value):
    '''
    Converts value to string

    Converts hexBinary value to string. For list values, the to_string()
    value for each entry is returned separated with commas.

    Parameters:

        value (hexBinary): A hexBinary value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<hexBinary>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_hexBinary(session, lists):
    '''
    Returns union of two or more string lists

    Returns a list of hexBinary contains all of the unique values in two or
    more list of hexBinary values.

    Parameters:

        lists (list): Two or more lists.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "union<hexBinary>",
        "lists": lists
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

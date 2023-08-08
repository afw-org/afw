# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def at_least_one_member_of_boolean(session, list1, list2):
    '''
    Checks for at least one value in common

    Returns boolean true if at least one value in boolean list1 is in boolean
    list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<boolean>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_boolean(session, values=None):
    '''
    Makes a list from values

    Takes any number of boolean values and returns a list of list.

    Parameters:

        values (list):

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "bag<boolean>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_boolean(session, value):
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
        "function": "bag_size<boolean>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def boolean(session, value):
    '''
    Convert to data type boolean

    Converts value to data type boolean returning boolean result.

    Parameters:

        value (): Value to convert

    Returns:
    boolean: Converted value
    '''

    request = session.Request()

    action = {
        "function": "boolean",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_boolean(session, arg1, arg2):
    '''
    Checks for equal

    Determine if boolean arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (boolean):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_boolean(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for boolean arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (boolean):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_boolean(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for boolean arg1 is greater than or equal to boolean arg2 and
    return the boolean result.

    Parameters:

        arg1 (boolean):

        arg2 (boolean):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_boolean(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for boolean arg1 is greater than boolean arg2 and return the
    boolean result.

    Parameters:

        arg1 (boolean):

        arg2 (boolean):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_boolean(session, list1, list2):
    '''
    Returns intersection of two lists

    Returns a list of boolean with the values that are common to both list of
    boolean list1 and list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "intersection<boolean>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_boolean(session, value):
    '''
    Checks whether value is dataType boolean

    Checks whether value is dataType boolean and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<boolean>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_boolean(session, value, list):
    '''
    Checks whether a value is in a list

    Checks whether boolean value is in list of boolean list and returns the
    boolean result.

    Parameters:

        value (boolean):

        list (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is_in<boolean>",
        "value": value,
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_boolean(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for boolean arg1 is less than or equal to boolean arg2 and return
    the boolean result.

    Parameters:

        arg1 (boolean):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_boolean(session, arg1, arg2):
    '''
    Checks for less than

    Checks for boolean arg1 is less that boolean arg2 and return the boolean
    result.

    Parameters:

        arg1 (boolean):

        arg2 (boolean):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_boolean(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if boolean arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (boolean):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_boolean(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for boolean arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (boolean):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<boolean>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_boolean(session, list):
    '''
    Converts a one value list to a boolean value

    This converts a list of boolean values that contains one value to a
    single boolean value.

    Parameters:

        list (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "one_and_only<boolean>",
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_boolean(session, list1, list2):
    '''
    Checks whether two lists are subsets of each other

    Returns boolean true if boolean list1 and boolean list2 are subsets of
    each other and return the boolean result.

    Parameters:

        list1 (list):

        list2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "set_equals<boolean>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_boolean(session, list1, list2):
    '''
    Determines if the first list is a subset of second list

    Returns boolean true if the unique values in boolean list1 are all in
    boolean list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "subset<boolean>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_boolean(session, value):
    '''
    Converts value to string

    Converts boolean value to string. For list values, the to_string() value
    for each entry is returned separated with commas.

    Parameters:

        value (boolean): A boolean value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<boolean>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_boolean(session, lists):
    '''
    Returns union of two or more string lists

    Returns a list of boolean contains all of the unique values in two or
    more list of boolean values.

    Parameters:

        lists (list): Two or more lists.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "union<boolean>",
        "lists": lists
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


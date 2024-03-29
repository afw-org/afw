# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def add_entries(session, target, source):
    '''
    Add entries of one or more lists to another

    Add the entries of one or more lists to another.

    Parameters:

        target (list): Target list. This list must not be immutable.

        source (list): Source list(s).

    Returns:
    list: The modified target list.
    '''

    request = session.Request()

    action = {
        "function": "add_entries",
        "target": target,
        "source": source
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_list(session, values=None):
    '''
    Makes a list from values

    Takes any number of list values and returns a list of list.

    Parameters:

        values (list):

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "bag<list>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_list(session, value):
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
        "function": "bag_size<list>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def clone_list(session, value):
    '''
    Clone list value

    Deep clone a list value.

    Parameters:

        value (list): The list value to clone.

    Returns:
    list: The cloned list value.
    '''

    request = session.Request()

    action = {
        "function": "clone<list>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_list(session, arg1, arg2):
    '''
    Checks for equal

    Determine if list arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use "eqx" ("===")
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (list):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_list(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for list arg1 is equal to the value and data type of arg2
    then return the boolean result. Use "eq" ("==") instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (list):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_list(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for list arg1 is greater than or equal to list arg2 and return the
    boolean result.

    Parameters:

        arg1 (list):

        arg2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_list(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for list arg1 is greater than list arg2 and return the boolean
    result.

    Parameters:

        arg1 (list):

        arg2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def includes_list(session, list, searchElement, fromIndex=None):
    '''
    Checks whether or not a list contains any value

    Checks whether or not a list contains any value.

    Parameters:

        list (list): The list to search.

        searchElement (): Element to find.

        fromIndex (integer): Index in the list to start search.

    Returns:
    boolean: Indicates if the element is found in list.
    '''

    request = session.Request()

    action = {
        "function": "includes<list>",
        "list": list,
        "searchElement": searchElement
    }

    if fromIndex != None:
        action['fromIndex'] = fromIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_list(session, value):
    '''
    Checks whether value is dataType list

    Checks whether value is dataType list and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<list>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def join(session, value, separator=None):
    '''
    Join the string values of list elements

    Concatenate the string values of the elements of a list with a separator.

    Parameters:

        value (list): A list of values (statements) of any data type.

        separator (string): The separator to use. If not specified, a comma
        (,) is used.

    Returns:
    string: Joined list values.
    '''

    request = session.Request()

    action = {
        "function": "join",
        "value": value
    }

    if separator != None:
        action['separator'] = separator

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_list(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for list arg1 is less than or equal to list arg2 and return the
    boolean result.

    Parameters:

        arg1 (list):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def length_list(session, value):
    '''
    Returns number of codepoints or entries in value

    This is a polymorphic function where list can be any of the supported
    data types. Return the integer number of entries in datatype list or
    codepoints in others.

    Parameters:

        value (list): Returns the number of entries in a list or code points
        in others.

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "length<list>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def list(session, values):
    '''
    Construct a list with 0 or more elements

    Construct a list with 0 or more elements.

    Parameters:

        values (): A value can refer to any adaptable value belonging to any
        data type or a list expression. In the case of a list expression,
        indicated by "..." followed by an expression that results in a list,
        every element within that list is included in the newly created list.

    Returns:
    list: The constructed list.
    '''

    request = session.Request()

    action = {
        "function": "list",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_list(session, arg1, arg2):
    '''
    Checks for less than

    Checks for list arg1 is less that list arg2 and return the boolean
    result.

    Parameters:

        arg1 (list):

        arg2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_list(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if list arg1 is not equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use "nex" ("!==")
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (list):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_list(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for list arg1 is not equal to the value or data type of arg2
    then return the boolean result. Use "ne" ("!=") instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (list):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<list>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def reverse(session, list):
    '''
    Return list with elements reversed

    Reverse the order of the elements in a list. If the list is typed, the
    resulting list will be the same type.

    Parameters:

        list (list): A list to reverse.

    Returns:
    list: A list with elements reversed.
    '''

    request = session.Request()

    action = {
        "function": "reverse",
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def slice(session, list, startIndex=None, endIndex=None):
    '''
    Return a consecutive slice of values from a list

    This function extracts a consecutive slice of values from a list.

    Parameters:

        list (list): The list to slice.

        startIndex (integer): This is the zero based starting index. Use
        negative number to index from the end of the list. If not specified,
        an index of 0 is assumed.

        endIndex (integer): This is the zero based ending index. If positive,
        this is one more than the index of the last value to include in the
        list. If negative, the index is from the end of the list. If not
        specified, the slice is from startIndex up to and including the end
        of the list.

    Returns:
    list: A list containing the selected values. If all of the values are the same data type, the list will be a list of that data type.
    '''

    request = session.Request()

    action = {
        "function": "slice",
        "list": list
    }

    if startIndex != None:
        action['startIndex'] = startIndex

    if endIndex != None:
        action['endIndex'] = endIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_list(session, value):
    '''
    Converts value to string

    Converts list value to string. For list values, the to_string() value for
    each entry is returned separated with commas.

    Parameters:

        value (list): A list value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<list>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


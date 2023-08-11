# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def add_entries(session, target, source):
    '''
    Add entries of one or more arrays to another

    Add the entries of one or more arrays to another.

    Parameters:

        target (array): Target array. This array must not be immutable.

        source (array): Source array(s).

    Returns:
    array: The modified target array.
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

def array(session, values):
    '''
    Construct an array with 0 or more elements

    Construct an array with 0 or more elements.

    Parameters:

        values (): A value can refer to any adaptable value belonging to any
        data type or an array expression. In the case of an array expression,
        indicated by '...' followed by an expression that results in an
        array, every element within that array is included in the newly
        created array.

    Returns:
    array: The constructed array.
    '''

    request = session.Request()

    action = {
        "function": "array",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_array(session, values=None):
    '''
    Makes an array from values

    Takes any number of array values and returns an array of array.

    Parameters:

        values (array):

    Returns:
    array: 
    '''

    request = session.Request()

    action = {
        "function": "bag<array>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_array(session, value):
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
        "function": "bag_size<array>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def clone_array(session, value):
    '''
    Clone array value

    Deep clone a array value.

    Parameters:

        value (array): The array value to clone.

    Returns:
    array: The cloned array value.
    '''

    request = session.Request()

    action = {
        "function": "clone<array>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_array(session, arg1, arg2):
    '''
    Checks for equal

    Determine if array arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use 'eqx' ('===')
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (array):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_array(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for array arg1 is equal to the value and data type of arg2
    then return the boolean result. Use 'eq' ('==') instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (array):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_array(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for array arg1 is greater than or equal to array arg2 and return
    the boolean result.

    Parameters:

        arg1 (array):

        arg2 (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_array(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for array arg1 is greater than array arg2 and return the boolean
    result.

    Parameters:

        arg1 (array):

        arg2 (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def includes_array(session, array, searchElement, fromIndex=None):
    '''
    Checks whether or not an array contains any value

    Checks whether or not an array contains any value.

    Parameters:

        array (array): The array to search.

        searchElement (): Element to find.

        fromIndex (integer): Index in the array to start search.

    Returns:
    boolean: Indicates if the element is found in array.
    '''

    request = session.Request()

    action = {
        "function": "includes<array>",
        "array": array,
        "searchElement": searchElement
    }

    if fromIndex != None:
        action['fromIndex'] = fromIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_array(session, value):
    '''
    Checks whether value is dataType array

    Checks whether value is dataType array and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<array>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def join(session, value, separator=None):
    '''
    Join the string values of array elements

    Concatenate the string values of the elements of an array with a
    separator.

    Parameters:

        value (array): An array of values (statements) of any data type.

        separator (string): The separator to use. If not specified, a comma
        (,) is used.

    Returns:
    string: Joined array values.
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

def le_array(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for array arg1 is less than or equal to array arg2 and return the
    boolean result.

    Parameters:

        arg1 (array):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def length_array(session, value):
    '''
    Returns number of codepoints or entries in value

    This is a polymorphic function where array can be any of the supported
    data types. Return the integer number of entries in datatype array or
    codepoints in others.

    Parameters:

        value (array): Returns the number of entries in an array or code
        points in others.

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "length<array>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_array(session, arg1, arg2):
    '''
    Checks for less than

    Checks for array arg1 is less that array arg2 and return the boolean
    result.

    Parameters:

        arg1 (array):

        arg2 (array):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_array(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if array arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (array):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_array(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for array arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (array):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<array>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def reverse(session, array):
    '''
    Return array with elements reversed

    Reverse the order of the elements in an array. If the array is typed, the
    resulting array will be the same type.

    Parameters:

        array (array): An array to reverse.

    Returns:
    array: An array with elements reversed.
    '''

    request = session.Request()

    action = {
        "function": "reverse",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def slice(session, array, startIndex=None, endIndex=None):
    '''
    Return a consecutive slice of values from an array

    This function extracts a consecutive slice of values from an array.

    Parameters:

        array (array): The array to slice.

        startIndex (integer): This is the zero based starting index. Use
        negative number to index from the end of the array. If not specified,
        an index of 0 is assumed.

        endIndex (integer): This is the zero based ending index. If positive,
        this is one more than the index of the last value to include in the
        array. If negative, the index is from the end of the array. If not
        specified, the slice is from startIndex up to and including the end
        of the array.

    Returns:
    array: An array containing the selected values. If all of the values are the same data type, the array will be an array of that data type.
    '''

    request = session.Request()

    action = {
        "function": "slice",
        "array": array
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

def to_string_array(session, value):
    '''
    Converts value to string

    Converts array value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Parameters:

        value (array): A array value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<array>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

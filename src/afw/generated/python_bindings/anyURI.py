# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def anyURI(session, value):
    """
    Convert to data type anyURI

    Converts value to data type anyURI returning anyURI result.

    Args:
        value (object): Value to convert

    Returns:
        object: Converted value
    """

    request = session.Request()

    action = {
        "function": "anyURI",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def at_least_one_member_of_anyURI(session, array1, array2):
    """
    Checks for at least one value in common

    Returns boolean true if at least one value in anyURI array1 is in anyURI
    array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<anyURI>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_anyURI(session, values=None):
    """
    Makes an array from values

    Takes any number of anyURI values and returns an array of array.

    Args:
        values (list):

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "bag<anyURI>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_anyURI(session, value):
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
        "function": "bag_size<anyURI>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ends_with_anyURI(session, value, subString):
    """
    Checks whether value ends with a string

    Checks whether anyURI value ends with a anyURI and return the boolean
    result.

    Args:
        value (object):

        subString (str):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ends_with<anyURI>",
        "value": value,
        "subString": subString
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_anyURI(session, arg1, arg2):
    """
    Checks for equal

    Determine if anyURI arg1 is equal to the value of arg2 converted to the
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
        "function": "eq<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_anyURI(session, arg1, arg2):
    """
    Checks for equal and type

    Determine if for anyURI arg1 is equal to the value and data type of arg2
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
        "function": "eqx<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_anyURI(session, arg1, arg2):
    """
    Checks for greater than or equal

    Checks for anyURI arg1 is greater than or equal to anyURI arg2 and return
    the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "ge<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_anyURI(session, arg1, arg2):
    """
    Checks for greater than

    Checks for anyURI arg1 is greater than anyURI arg2 and return the boolean
    result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "gt<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def includes_anyURI(session, searchString, subString, position=None):
    """
    Checks whether the anyURI value includes a string

    Checks whether the anyURI value includes a string and return the boolean
    result.

    Args:
        searchString (object): The anyURI to search.

        subString (str): Substring to find.

        position (int): Zero-based position in the search string to start
        search.

    Returns:
        bool: Indicates if the substring is contained in the search string.
    """

    request = session.Request()

    action = {
        "function": "includes<anyURI>",
        "searchString": searchString,
        "subString": subString
    }

    if position != None:
        action['position'] = position

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def index_of_anyURI(session, value, subString, startIndex=None):
    """
    Returns index of first occurrence

    Returns the zero-based index into anyURI value of subString. If subString
    is not found, -1 is returned.

    Args:
        value (object): The anyURI value to search.

        subString (str): Substring to search for.

        startIndex (int): Optional start index for search if different than
        start of string.

    Returns:
        int: Zero-based index of subString or -1 if not found.
    """

    request = session.Request()

    action = {
        "function": "index_of<anyURI>",
        "value": value,
        "subString": subString
    }

    if startIndex != None:
        action['startIndex'] = startIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def intersection_anyURI(session, array1, array2):
    """
    Returns intersection of two arrays

    Returns an array of anyURI with the values that are common to both array
    of anyURI array1 and array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "intersection<anyURI>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_anyURI(session, value):
    """
    Checks whether value is dataType anyURI

    Checks whether value is dataType anyURI and return the boolean result.

    Args:
        value (object): Value to check

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is<anyURI>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_anyURI(session, value, array):
    """
    Checks whether a value is in an array

    Checks whether anyURI value is in array of anyURI array and returns the
    boolean result.

    Args:
        value (object):

        array (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "is_in<anyURI>",
        "value": value,
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def last_index_of_anyURI(session, value, subString, startIndex=None):
    """
    Returns index of last occurrence

    Returns the zero-based index into anyURI value of the last occurrence of
    a subString. If subString is not found, -1 is returned.

    Args:
        value (object): The anyURI value to search.

        subString (str): Substring to search for.

        startIndex (int): Optional start index for search if different than
        start of string.

    Returns:
        int: Zero-based index of subString or -1 if not found.
    """

    request = session.Request()

    action = {
        "function": "last_index_of<anyURI>",
        "value": value,
        "subString": subString
    }

    if startIndex != None:
        action['startIndex'] = startIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_anyURI(session, arg1, arg2):
    """
    Checks for less than or equal

    Checks for anyURI arg1 is less than or equal to anyURI arg2 and return
    the boolean result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "le<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def length_anyURI(session, value):
    """
    Returns number of codepoints or entries in value

    This is a polymorphic function where anyURI can be any of the supported
    data types. Return the integer number of entries in datatype array or
    codepoints in others.

    Args:
        value (object): Returns the number of entries in an array or code
        points in others.

    Returns:
        int:
    """

    request = session.Request()

    action = {
        "function": "length<anyURI>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_anyURI(session, arg1, arg2):
    """
    Checks for less than

    Checks for anyURI arg1 is less that anyURI arg2 and return the boolean
    result.

    Args:
        arg1 (object):

        arg2 (object):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "lt<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_anyURI(session, arg1, arg2):
    """
    Checks for not equal

    Determine if anyURI arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use 'nex' ('!==')
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
        "function": "ne<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_anyURI(session, arg1, arg2):
    """
    Checks for not equal value or type

    Determine if for anyURI arg1 is not equal to the value or data type of
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
        "function": "nex<anyURI>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_anyURI(session, array):
    """
    Converts a one value array to a anyURI value

    This converts an array of anyURI values that contains one value to a
    single anyURI value.

    Args:
        array (list):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "one_and_only<anyURI>",
        "array": array
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_index_of_anyURI(session, value, regexp):
    """
    Returns index of first match of regular expression

    Search anyURI value for a regular expression and return index. If not
    found, -1 is returned.

    Args:
        value (object): The anyURI value to search.

        regexp (str): A regular expression to use for search.

    Returns:
        int: Zero-based index of subString or -1 if not found.
    """

    request = session.Request()

    action = {
        "function": "regexp_index_of<anyURI>",
        "value": value,
        "regexp": regexp
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_match_anyURI(session, value, regexp):
    """
    Search for a match using a regular expression

    Checks whether anyURI value matches the regular expression regexp and
    return the boolean result.

    Args:
        value (object):

        regexp (str):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "regexp_match<anyURI>",
        "value": value,
        "regexp": regexp
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_replace_anyURI(session, value, regexp, replacement, limit=None):
    """
    Replace strings matching a regular expression

    Replace matched values for a regular expression in a anyURI value.

    Args:
        value (object): The original anyURI value.

        regexp (str): A regular expression to use for search.

        replacement (str): The replacement string.

        limit (int): This is the maximum times to replace. The default is 1.
        Specify -1 to replace all occurrences.

    Returns:
        object: A anyURI value with the matched string(s) replaced.
    """

    request = session.Request()

    action = {
        "function": "regexp_replace<anyURI>",
        "value": value,
        "regexp": regexp,
        "replacement": replacement
    }

    if limit != None:
        action['limit'] = limit

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def repeat_anyURI(session, value, times):
    """
    Returns a repeated value

    Repeat a anyURI value a specified number of times.

    Args:
        value (object): The anyURI value to repeat.

        times (int): The number of times to repeat the value.

    Returns:
        object: The repeated anyURI value.
    """

    request = session.Request()

    action = {
        "function": "repeat<anyURI>",
        "value": value,
        "times": times
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def replace_anyURI(session, value, match, replacement, limit=None):
    """
    Replace strings

    Replace string(s) in a anyURI value.

    Args:
        value (object): The original anyURI value.

        match (str): The string to replace.

        replacement (str): The replacement string.

        limit (int): This is the maximum times to replace. The default is 1.
        Specify -1 to replace all occurrences.

    Returns:
        object: A anyURI value with the matched string(s) replaced.
    """

    request = session.Request()

    action = {
        "function": "replace<anyURI>",
        "value": value,
        "match": match,
        "replacement": replacement
    }

    if limit != None:
        action['limit'] = limit

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def set_equals_anyURI(session, array1, array2):
    """
    Checks whether two arrays are subsets of each other

    Returns boolean true if anyURI array1 and anyURI array2 are subsets of
    each other and return the boolean result.

    Args:
        array1 (list):

        array2 (list):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "set_equals<anyURI>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def split_anyURI(session, value, separator=None, limit=None):
    """
    Split at a separator

    Split anyURI value into an array of strings using a separator.

    Args:
        value (object): The anyURI value to split.

        separator (str): The separator to use. If this is an empty string or
        separator is not specified, the value is split between characters.

        limit (int): This is the maximum number of splits. Any remaining part
        of value is ignored.

    Returns:
        list: An array of strings.
    """

    request = session.Request()

    action = {
        "function": "split<anyURI>",
        "value": value
    }

    if separator != None:
        action['separator'] = separator

    if limit != None:
        action['limit'] = limit

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def starts_with_anyURI(session, value, subString):
    """
    Checks whether value starts with a string

    Checks whether anyURI value starts with a subString and return the
    boolean result.

    Args:
        value (object):

        subString (str):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "starts_with<anyURI>",
        "value": value,
        "subString": subString
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_anyURI(session, array1, array2):
    """
    Determines if the first array is a subset of second array

    Returns boolean true if the unique values in anyURI array1 are all in
    anyURI array2.

    Args:
        array1 (list): The first array.

        array2 (list): The second array.

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "subset<anyURI>",
        "array1": array1,
        "array2": array2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def substring_anyURI(session, string, startIndex, endIndex=None):
    """
    Extract a substring

    Returns the anyURI substring of value beginning at zero-based position
    integer startIndex and ending at the position before integer endIndex.
    Specify -1 or omitting endIndex to return up to end of anyURI.

    Args:
        string (object):

        startIndex (int):

        endIndex (int):

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "substring<anyURI>",
        "string": string,
        "startIndex": startIndex
    }

    if endIndex != None:
        action['endIndex'] = endIndex

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_anyURI(session, value):
    """
    Converts value to string

    Converts anyURI value to string. For array values, the to_string() value
    for each entry is returned separated with commas.

    Args:
        value (object): A anyURI value.

    Returns:
        str: The string representation of the value.
    """

    request = session.Request()

    action = {
        "function": "to_string<anyURI>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_anyURI(session, arrays):
    """
    Returns union of two or more string arrays

    Returns an array of anyURI contains all of the unique values in two or
    more array of anyURI values.

    Args:
        arrays (list): Two or more arrays.

    Returns:
        list:
    """

    request = session.Request()

    action = {
        "function": "union<anyURI>",
        "arrays": arrays
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def url_encode_anyURI(session, unencoded):
    """
    URI encode

    URL encode a value or bag of values.

    Args:
        unencoded (object): URL encode a single value. See the url_encode
        method for the data type of more details.

    Returns:
        str: URI encoded string.
    """

    request = session.Request()

    action = {
        "function": "url_encode<anyURI>",
        "unencoded": unencoded
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


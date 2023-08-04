# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def add_string(session, string, values):
    '''
    Add (concatenate) strings

    Add (concatenate) a string with 1 or more values of any data type
    converted to their string value and return the string result.

    Parameters:

        string (string):

        values ():

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "add<string>",
        "string": string,
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def at_least_one_member_of_string(session, list1, list2):
    '''
    Checks for at least one value in common

    Returns boolean true if at least one value in string list1 is in string
    list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "at_least_one_member_of<string>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_size_string(session, value):
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
        "function": "bag_size<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_string(session, values=None):
    '''
    Makes a list from values

    Takes any number of string values and returns a list of list.

    Parameters:

        values (list):

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "bag<string>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def concat(session, values):
    '''
    Convert two or more values to string and concatenate

    Convert two or more values of any data type to string and return the
    concatenated result. A value with an undefined value is represented by
    `<undefined>`.

    Parameters:

        values (): Value to convert

    Returns:
    string: The concatenated string values.
    '''

    request = session.Request()

    action = {
        "function": "concat",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def encode_as_base64Binary_string(session, value):
    '''
    Encode to a base64Binary value

    Encode a value as a base64Binary. The effect is to create a base64Binary
    value with an internal value of the value passed.

    Parameters:

        value (string): The string value to encode.

    Returns:
    base64Binary: A base64Binary value.
    '''

    request = session.Request()

    action = {
        "function": "encode_as_base64Binary<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def encode_as_hexBinary_string(session, value):
    '''
    Encode to a hexBinary value

    Encode a value as a hexBinary. The effect is to create a hexBinary value
    with an internal value of the value passed.

    Parameters:

        value (string): The string value to encode.

    Returns:
    hexBinary: A hexBinary value.
    '''

    request = session.Request()

    action = {
        "function": "encode_as_hexBinary<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ends_with_string(session, value, subString):
    '''
    Checks whether value ends with a string

    Checks whether string value ends with a string and return the boolean
    result.

    Parameters:

        value (string):

        subString (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ends_with<string>",
        "value": value,
        "subString": subString
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_ignore_case_string(session, arg1, arg2):
    '''
    Checks for equal ignoring case

    Checks for string arg1 is equal to string arg2 ignoring case and return
    the boolean result.

    Parameters:

        arg1 (string):

        arg2 (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq_ignore_case<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_string(session, arg1, arg2):
    '''
    Checks for equal

    Determine if string arg1 is equal to the value of arg2 converted to the
    data type of arg1 then return the boolean result. Use "eqx" ("===")
    instead if you want false to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (string):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_string(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for string arg1 is equal to the value and data type of arg2
    then return the boolean result. Use "eq" ("==") instead if you want arg2
    to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (string):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_string(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for string arg1 is greater than or equal to string arg2 and return
    the boolean result.

    Parameters:

        arg1 (string):

        arg2 (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_string(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for string arg1 is greater than string arg2 and return the boolean
    result.

    Parameters:

        arg1 (string):

        arg2 (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def includes_string(session, searchString, subString, position=None):
    '''
    Checks whether the string value includes a string

    Checks whether the string value includes a string and return the boolean
    result.

    Parameters:

        searchString (string): The string to search.

        subString (string): Substring to find.

        position (integer): Zero-based position in the search string to start
        search.

    Returns:
    boolean: Indicates if the substring is contained in the search string.
    '''

    request = session.Request()

    action = {
        "function": "includes<string>",
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

def index_of_string(session, value, subString, startIndex=None):
    '''
    Returns index of first occurrence

    Returns the zero-based index into string value of subString. If subString
    is not found, -1 is returned.

    Parameters:

        value (string): The string value to search.

        subString (string): Substring to search for.

        startIndex (integer): Optional start index for search if different
        than start of string.

    Returns:
    integer: Zero-based index of subString or -1 if not found.
    '''

    request = session.Request()

    action = {
        "function": "index_of<string>",
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

def intersection_string(session, list1, list2):
    '''
    Returns intersection of two lists

    Returns a list of string with the values that are common to both list of
    string list1 and list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "intersection<string>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_in_string(session, value, list):
    '''
    Checks whether a value is in a list

    Checks whether string value is in list of string list and returns the
    boolean result.

    Parameters:

        value (string):

        list (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is_in<string>",
        "value": value,
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_string(session, value):
    '''
    Checks whether value is dataType string

    Checks whether value is dataType string and return the boolean result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def last_index_of_string(session, value, subString, startIndex=None):
    '''
    Returns index of last occurrence

    Returns the zero-based index into string value of the last occurrence of
    a subString. If subString is not found, -1 is returned.

    Parameters:

        value (string): The string value to search.

        subString (string): Substring to search for.

        startIndex (integer): Optional start index for search if different
        than start of string.

    Returns:
    integer: Zero-based index of subString or -1 if not found.
    '''

    request = session.Request()

    action = {
        "function": "last_index_of<string>",
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

def le_string(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for string arg1 is less than or equal to string arg2 and return
    the boolean result.

    Parameters:

        arg1 (string):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def length_string(session, value):
    '''
    Returns number of codepoints or entries in value

    This is a polymorphic function where string can be any of the supported
    data types. Return the integer number of entries in datatype list or
    codepoints in others.

    Parameters:

        value (string): Returns the number of entries in a list or code
        points in others.

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "length<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_string(session, arg1, arg2):
    '''
    Checks for less than

    Checks for string arg1 is less that string arg2 and return the boolean
    result.

    Parameters:

        arg1 (string):

        arg2 (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def max_string(session, values):
    '''
    Maximum value

    Return the string value that is greater than or equal to the others.

    Parameters:

        values (string):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "max<string>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def min_string(session, values):
    '''
    Minimum value

    Return the string value that is less than or equal to the others.

    Parameters:

        values (string):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "min<string>",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_string(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if string arg1 is not equal to the value of arg2 converted to
    the data type of arg1 then return the boolean result. Use "nex" ("!==")
    instead if you want true to be returned if arg1 and arg2's data type
    don't match.

    Parameters:

        arg1 (string):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_string(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for string arg1 is not equal to the value or data type of
    arg2 then return the boolean result. Use "ne" ("!=") instead if you want
    arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (string):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<string>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def normalize_space_string(session, string):
    '''
    Trims whitespace from beginning and end

    Remove whitespace from the beginning and end of a string value.

    Parameters:

        string (string):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "normalize_space<string>",
        "string": string
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def normalize_to_lower_case_string(session, string):
    '''
    Converts to lower case

    Normalize string value to lower case and returns string result.

    Parameters:

        string (string):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "normalize_to_lower_case<string>",
        "string": string
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def one_and_only_string(session, list):
    '''
    Converts a one value list to a string value

    This converts a list of string values that contains one value to a single
    string value.

    Parameters:

        list (list):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "one_and_only<string>",
        "list": list
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_index_of_string(session, value, regexp):
    '''
    Returns index of first match of regular expression

    Search string value for a regular expression and return index. If not
    found, -1 is returned.

    Parameters:

        value (string): The string value to search.

        regexp (string): A regular expression to use for search.

    Returns:
    integer: Zero-based index of subString or -1 if not found.
    '''

    request = session.Request()

    action = {
        "function": "regexp_index_of<string>",
        "value": value,
        "regexp": regexp
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_match_string(session, value, regexp):
    '''
    Search for a match using a regular expression

    Checks whether string value matches the regular expression regexp and
    return the boolean result.

    Parameters:

        value (string):

        regexp (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "regexp_match<string>",
        "value": value,
        "regexp": regexp
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def regexp_replace_string(session, value, regexp, replacement, limit=None):
    '''
    Replace strings matching a regular expression

    Replace matched values for a regular expression in a string value.

    Parameters:

        value (string): The original string value.

        regexp (string): A regular expression to use for search.

        replacement (string): The replacement string.

        limit (integer): This is the maximum times to replace. The default is
        1. Specify -1 to replace all occurrences.

    Returns:
    string: A string value with the matched string(s) replaced.
    '''

    request = session.Request()

    action = {
        "function": "regexp_replace<string>",
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

def repeat_string(session, value, times):
    '''
    Returns a repeated value

    Repeat a string value a specified number of times.

    Parameters:

        value (string): The string value to repeat.

        times (integer): The number of times to repeat the value.

    Returns:
    string: The repeated string value.
    '''

    request = session.Request()

    action = {
        "function": "repeat<string>",
        "value": value,
        "times": times
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def replace_string(session, value, match, replacement, limit=None):
    '''
    Replace strings

    Replace string(s) in a string value.

    Parameters:

        value (string): The original string value.

        match (string): The string to replace.

        replacement (string): The replacement string.

        limit (integer): This is the maximum times to replace. The default is
        1. Specify -1 to replace all occurrences.

    Returns:
    string: A string value with the matched string(s) replaced.
    '''

    request = session.Request()

    action = {
        "function": "replace<string>",
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

def set_equals_string(session, list1, list2):
    '''
    Checks whether two lists are subsets of each other

    Returns boolean true if string list1 and string list2 are subsets of each
    other and return the boolean result.

    Parameters:

        list1 (list):

        list2 (list):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "set_equals<string>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def split_string(session, value, separator=None, limit=None):
    '''
    Split at a separator

    Split string value into an list of strings using a separator.

    Parameters:

        value (string): The string value to split.

        separator (string): The separator to use. If this is an empty string
        or separator is not specified, the value is split between characters.

        limit (integer): This is the maximum number of splits. Any remaining
        part of value is ignored.

    Returns:
    list: An list of strings.
    '''

    request = session.Request()

    action = {
        "function": "split<string>",
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

def starts_with_string(session, value, subString):
    '''
    Checks whether value starts with a string

    Checks whether string value starts with a subString and return the
    boolean result.

    Parameters:

        value (string):

        subString (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "starts_with<string>",
        "value": value,
        "subString": subString
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def string(session, values):
    '''
    Convert one or more values to string and concatenate

    Convert one or more values of any data type to string and return the
    concatenated result. A value with an undefined value is represented by
    `<undefined>`.

    Parameters:

        values (): Value to convert

    Returns:
    string: The concatenated string values.
    '''

    request = session.Request()

    action = {
        "function": "string",
        "values": values
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def subset_string(session, list1, list2):
    '''
    Determines if the first list is a subset of second list

    Returns boolean true if the unique values in string list1 are all in
    string list2.

    Parameters:

        list1 (list): The first list.

        list2 (list): The second list.

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "subset<string>",
        "list1": list1,
        "list2": list2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def substring_string(session, string, startIndex, endIndex=None):
    '''
    Extract a substring

    Returns the string substring of value beginning at zero-based position
    integer startIndex and ending at the position before integer endIndex.
    Specify -1 or omitting endIndex to return up to end of string.

    Parameters:

        string (string):

        startIndex (integer):

        endIndex (integer):

    Returns:
    string: 
    '''

    request = session.Request()

    action = {
        "function": "substring<string>",
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

def to_anyURI_string(session, value):
    '''
    Converts string to anyURI

    Converts string value to anyURI.

    Parameters:

        value (string):

    Returns:
    anyURI: 
    '''

    request = session.Request()

    action = {
        "function": "to_anyURI<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_boolean_string(session, value):
    '''
    Converts string to boolean

    Converts string value to boolean.

    Parameters:

        value (string):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "to_boolean<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_dateTime_string(session, value):
    '''
    Converts string to dateTime

    Converts string value to dateTime.

    Parameters:

        value (string):

    Returns:
    dateTime: 
    '''

    request = session.Request()

    action = {
        "function": "to_dateTime<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_date_string(session, value):
    '''
    Converts string to date

    Converts string value to date.

    Parameters:

        value (string):

    Returns:
    date: 
    '''

    request = session.Request()

    action = {
        "function": "to_date<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_dayTimeDuration_string(session, value):
    '''
    Converts string to dayTimeDuration

    Converts string value to dayTimeDuration.

    Parameters:

        value (string):

    Returns:
    dayTimeDuration: 
    '''

    request = session.Request()

    action = {
        "function": "to_dayTimeDuration<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_dnsName_string(session, value):
    '''
    Converts string to dnsName

    Converts string value to dnsName.

    Parameters:

        value (string):

    Returns:
    dnsName: 
    '''

    request = session.Request()

    action = {
        "function": "to_dnsName<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_double_string(session, value):
    '''
    Converts to double

    Converts string value to double and returns double result.

    Parameters:

        value (string):

    Returns:
    double: 
    '''

    request = session.Request()

    action = {
        "function": "to_double<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_integer_string(session, value):
    '''
    Converts to integer

    Truncate string value to a whole number and returns integer result.

    Parameters:

        value (string):

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "to_integer<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_ipAddress_string(session, value):
    '''
    Converts string to ipAddress

    Converts string value to ipAddress.

    Parameters:

        value (string):

    Returns:
    ipAddress: 
    '''

    request = session.Request()

    action = {
        "function": "to_ipAddress<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_rfc822Name_string(session, value):
    '''
    Converts string to rfc822Name

    Converts string value to rfc822Name.

    Parameters:

        value (string):

    Returns:
    rfc822Name: 
    '''

    request = session.Request()

    action = {
        "function": "to_rfc822Name<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_string(session, value):
    '''
    Converts value to string

    Converts string value to string. For list values, the to_string() value
    for each entry is returned separated with commas.

    Parameters:

        value (string): A string value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_time_string(session, value):
    '''
    Converts string to time

    Converts string value to time.

    Parameters:

        value (string):

    Returns:
    time: 
    '''

    request = session.Request()

    action = {
        "function": "to_time<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_x500Name_string(session, value):
    '''
    Converts string to x500Name

    Converts string value to x500Name.

    Parameters:

        value (string):

    Returns:
    x500Name: 
    '''

    request = session.Request()

    action = {
        "function": "to_x500Name<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_yearMonthDuration_string(session, value):
    '''
    Converts string to yearMonthDuration

    Converts string value to yearMonthDuration.

    Parameters:

        value (string):

    Returns:
    yearMonthDuration: 
    '''

    request = session.Request()

    action = {
        "function": "to_yearMonthDuration<string>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def union_string(session, lists):
    '''
    Returns union of two or more string lists

    Returns a list of string contains all of the unique values in two or more
    list of string values.

    Parameters:

        lists (list): Two or more lists.

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "union<string>",
        "lists": lists
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def url_decode(session, encoded):
    '''
    URL decode

    URL decode a value or bag of values.

    Parameters:

        encoded (string): URL decode a single string or a bag of string.

    Returns:
    string: A string or bag of strings.
    '''

    request = session.Request()

    action = {
        "function": "url_decode",
        "encoded": encoded
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def url_encode_string(session, unencoded):
    '''
    URI encode

    URL encode a value or bag of values.

    Parameters:

        unencoded (string): URL encode a single value. See the url_encode
        method for the data type of more details.

    Returns:
    string: URI encoded string.
    '''

    request = session.Request()

    action = {
        "function": "url_encode<string>",
        "unencoded": unencoded
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


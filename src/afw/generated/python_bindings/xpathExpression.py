# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def bag_size_xpathExpression(session, value):
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
        "function": "bag_size<xpathExpression>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def bag_xpathExpression(session, values=None):
    '''
    Makes a list from values

    Takes any number of xpathExpression values and returns a list of list.

    Parameters:

        values (list):

    Returns:
    list: 
    '''

    request = session.Request()

    action = {
        "function": "bag<xpathExpression>"
    }

    if values != None:
        action['values'] = values

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def compile_xpathExpression(session, source, listing=None):
    '''
    Compile xpathExpression value

    Compile xpathExpression value and return either an unevaluated adaptive
    value or a string containing the compiler listing.

    Parameters:

        source (xpathExpression): xpathExpression string to compile

        listing (): If specified, a compiler listing is produced instead of
        an unevaluated expression value.
        
        This parameter can be an integer between 0 and 10 of a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    unevaluated: 
    '''

    request = session.Request()

    action = {
        "function": "compile<xpathExpression>",
        "source": source
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eq_xpathExpression(session, arg1, arg2):
    '''
    Checks for equal

    Determine if xpathExpression arg1 is equal to the value of arg2 converted
    to the data type of arg1 then return the boolean result. Use "eqx"
    ("===") instead if you want false to be returned if arg1 and arg2's data
    type don't match.

    Parameters:

        arg1 (xpathExpression):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eq<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def eqx_xpathExpression(session, arg1, arg2):
    '''
    Checks for equal and type

    Determine if for xpathExpression arg1 is equal to the value and data type
    of arg2 then return the boolean result. Use "eq" ("==") instead if you
    want arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (xpathExpression):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "eqx<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def evaluate_xpathExpression(session, source, additionalUntrustedQualifiedVariables=None):
    '''
    Evaluate xpathExpression value

    Compile and evaluate xpathExpression value.

    Parameters:

        source (xpathExpression): xpathExpression string to compile and
        evaluate

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    unevaluated: 
    '''

    request = session.Request()

    action = {
        "function": "evaluate<xpathExpression>",
        "source": source
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ge_xpathExpression(session, arg1, arg2):
    '''
    Checks for greater than or equal

    Checks for xpathExpression arg1 is greater than or equal to
    xpathExpression arg2 and return the boolean result.

    Parameters:

        arg1 (xpathExpression):

        arg2 (xpathExpression):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ge<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def gt_xpathExpression(session, arg1, arg2):
    '''
    Checks for greater than

    Checks for xpathExpression arg1 is greater than xpathExpression arg2 and
    return the boolean result.

    Parameters:

        arg1 (xpathExpression):

        arg2 (xpathExpression):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "gt<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def is_xpathExpression(session, value):
    '''
    Checks whether value is dataType xpathExpression

    Checks whether value is dataType xpathExpression and return the boolean
    result.

    Parameters:

        value (): Value to check

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "is<xpathExpression>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def le_xpathExpression(session, arg1, arg2):
    '''
    Checks for less than or equal

    Checks for xpathExpression arg1 is less than or equal to xpathExpression
    arg2 and return the boolean result.

    Parameters:

        arg1 (xpathExpression):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "le<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def lt_xpathExpression(session, arg1, arg2):
    '''
    Checks for less than

    Checks for xpathExpression arg1 is less that xpathExpression arg2 and
    return the boolean result.

    Parameters:

        arg1 (xpathExpression):

        arg2 (xpathExpression):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "lt<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def ne_xpathExpression(session, arg1, arg2):
    '''
    Checks for not equal

    Determine if xpathExpression arg1 is not equal to the value of arg2
    converted to the data type of arg1 then return the boolean result. Use
    "nex" ("!==") instead if you want true to be returned if arg1 and arg2's
    data type don't match.

    Parameters:

        arg1 (xpathExpression):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "ne<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def nex_xpathExpression(session, arg1, arg2):
    '''
    Checks for not equal value or type

    Determine if for xpathExpression arg1 is not equal to the value or data
    type of arg2 then return the boolean result. Use "ne" ("!=") instead if
    you want arg2 to be converted to the data type of arg1 before comparison.

    Parameters:

        arg1 (xpathExpression):

        arg2 ():

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "nex<xpathExpression>",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def to_string_xpathExpression(session, value):
    '''
    Converts value to string

    Converts xpathExpression value to string. For list values, the
    to_string() value for each entry is returned separated with commas.

    Parameters:

        value (xpathExpression): A xpathExpression value.

    Returns:
    string: The string representation of the value.
    '''

    request = session.Request()

    action = {
        "function": "to_string<xpathExpression>",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def xpathExpression(session, value):
    '''
    Convert to data type xpathExpression

    Converts value to data type xpathExpression returning xpathExpression
    result.

    Parameters:

        value (): Value to convert

    Returns:
    xpathExpression: Converted value
    '''

    request = session.Request()

    action = {
        "function": "xpathExpression",
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def xpath_node_count(session, nodeset):
    '''
    Number of nodes in node-set

    The number of nodes in a node-set.

    Parameters:

        nodeset (xpathExpression):

    Returns:
    integer: 
    '''

    request = session.Request()

    action = {
        "function": "xpath_node_count",
        "nodeset": nodeset
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def xpath_node_eq(session, arg1, arg2):
    '''
    Checks for equal

    Checks for xpathExpression arg1 is equal to xpathExpression arg2 and
    return the boolean result.

    Parameters:

        arg1 (xpathExpression):

        arg2 (xpathExpression):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "xpath_node_eq",
        "arg1": arg1,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def xpath_node_match(session, nodeset2, arg2):
    '''
    Checks whether two node-sets or their children have an equal node

    Returns true if any of the nodes matched by nodeset1 are equal to any of
    the nodes matched by nodeset2 or their corresponding children.

    Parameters:

        nodeset2 (xpathExpression):

        arg2 (xpathExpression):

    Returns:
    boolean: 
    '''

    request = session.Request()

    action = {
        "function": "xpath_node_match",
        "nodeset2": nodeset2,
        "arg2": arg2
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

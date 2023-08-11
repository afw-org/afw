# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def assign(session, name, value):
    '''
    Assign a value to a defined variable

    Assign a value to the innermost structured block definition of a
    variable. If the variable is not defined, the variable is defined in the
    innermost structured block. An error is thrown if not called from an
    array of values (statements) in a structured function.

    Parameters:

        name (string): Variable name

        value (): This is the value to assign to the variable.

    Returns:
    None: The value assigned.
    '''

    request = session.Request()

    action = {
        "function": "assign",
        "name": name,
        "value": value
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def break_(session, value=None):
    '''
    Break out of a block

    This is a special function that can be called to break out of the body of
    a loop. If called outside of a loop body, an error is thrown.

    Parameters:

        value (): The value to evaluate that the enclosing loop will return.
        If not specified, the last evaluated value or a null value will be
        returned.

    Returns:
    None: This function returns from the body of a loop with the last evaluated value.
    '''

    request = session.Request()

    action = {
        "function": "break"
    }

    if value != None:
        action['value'] = value

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def const(session, name, value, type=None):
    '''
    Define one or more constants in the current block

    Define one or more statically scoped constants local to the current
    script block with a permanent value. These constants can be accessed from
    the current block and inner blocks, but can not be assigned a different
    value.

    Parameters:

        name (array): The name of one or more constants to defined in the
        current block.

        value (): This is the value of the constant(s).

        type (object): The type of the constant(s).

    Returns:
    None: The value assigned.
    '''

    request = session.Request()

    action = {
        "function": "const",
        "name": name,
        "value": value
    }

    if type != None:
        action['type'] = type

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def continue_(session):
    '''
    Continue at beginning of a loop

    This is a special function that can be called in the body of a loop
    function to test the condition and, if true, start evaluating the body
    again. If called outside of a loop body, an error is thrown.

    Parameters:

    Returns:
    None: This function does not return.
    '''

    request = session.Request()

    action = {
        "function": "continue"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def do_while(session, condition, body):
    '''
    Evaluate an array of values (statements) at least once while a condition is true

    This creates a new structured block with a new nested variable scope.
    
    This function will evaluate an array of values (statements) at least once
    while a condition is true. See the related functions 'break', 'continue',
    'return' and 'throw'.

    Parameters:

        condition (boolean): While this condition is true, the loop will
        continue. This is evaluated in the loop's scope.

        body (array): This is an array of values (statements) that are
        evaluated for each iteration of the loop. Each value in body is
        evaluated in order until the end of the array or until a 'break',
        'continue', 'return' or 'throw' function is encountered.

    Returns:
    None: The last value evaluated in body or null if the body is empty.
    '''

    request = session.Request()

    action = {
        "function": "do_while",
        "condition": condition,
        "body": body
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def for_(session, initial=None, condition=None, increment=None, body=None):
    '''
    Evaluate an array of values (statements) while a condition is true with an array of initial and increment values

    This creates a new structured block with a new nested variable scope.
    
    This function loops while condition is true. If the condition is false
    for the first iteration, the loop returns a null value.

    Parameters:

        initial (array): This is an array of values (statements) to evaluate
        before the loop starts. The values will normally be a call to the
        'assign' function.

        condition (boolean): While this condition is true, the loop will
        continue.

        increment (array): This is an array of values (statements) to
        evaluate after each iteration of the loop. The values will normally
        be a call to the 'assign' function.

        body (array): This is an array of values (statements) that are
        evaluated for each iteration of the loop. Each value in body is
        evaluated in order until the end of the array or until a 'break',
        'continue', 'return' or 'throw' function is encountered.

    Returns:
    None: The last value evaluated in body or null if condition evaluates to false the first time.
    '''

    request = session.Request()

    action = {
        "function": "for"
    }

    if initial != None:
        action['initial'] = initial

    if condition != None:
        action['condition'] = condition

    if increment != None:
        action['increment'] = increment

    if body != None:
        action['body'] = body

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def foreach(session, name, value, body=None):
    '''
    Evaluate an array of values (statements) while a condition is true with an array of initial and increment values

    This creates a new structured block with a new nested variable scope.
    
    This function will evaluate an array of values (statements) while a
    condition is true with initial and increment values. The condition is
    tested at the beginning of the loop. If the condition is false for the
    first iteration, the loop returns a null value.

    Parameters:

        name (array): Variable name(s).

        value (): Any array, object or single value.

        body (array): This is an array of values (statements) that are
        evaluated for each iteration of the loop. Each value in body is
        evaluated in order until the end of the array or until a 'break',
        'continue', 'return' or 'throw' function is encountered.

    Returns:
    None: The last value evaluated in body or null if condition evaluates to false the first time.
    '''

    request = session.Request()

    action = {
        "function": "foreach",
        "name": name,
        "value": value
    }

    if body != None:
        action['body'] = body

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def if_(session, condition, then, _else=None):
    '''
    If/then/else function

    Evaluate one of two different values depending on test condition.

    Parameters:

        condition (boolean): If true, parameter 'then' is evaluated for
        result. If false, parameter 'else' is evaluated.

        then (array): This is the body of a structured block that is
        evaluated if 'condition' is true. See the 'body' parameter of the
        'block' function for information on how the body is processed.

        else (array): This is the body of a structured block that is
        evaluated if 'condition' is false. If not specified and condition is
        false, a null value is returned. See the 'body' parameter of the
        'block' function for information on how the body is processed.

    Returns:
    None: The result of evaluating 'then' or 'else'
    '''

    request = session.Request()

    action = {
        "function": "if",
        "condition": condition,
        "then": then
    }

    if _else != None:
        action['else'] = _else

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def let(session, name, value=None, type=None):
    '''
    Declare one or more variables in the current block

    Declare one or more statically scoped variable locations local to the
    current script block and optionally assign them an initial value. These
    variables can be accessed and assigned different values from the current
    block and inner blocks.

    Parameters:

        name (array): The name of one or more variables to declared in the
        current block.

        value (): This is the initial value of the variable(s). If not
        specified, the variable will have a value of undefined.

        type (object): The type of the variable(s).

    Returns:
    None: The value assigned.
    '''

    request = session.Request()

    action = {
        "function": "let",
        "name": name
    }

    if value != None:
        action['value'] = value

    if type != None:
        action['type'] = type

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def rethrow(session):
    '''
    rethrow error from inside case block

    This is a special function that can be called to rethrow an error inside
    of a catch block. If called outside of a catch body, an error is thrown.

    Parameters:

    Returns:
    None: This function rethrows the current error in a catch block.
    '''

    request = session.Request()

    action = {
        "function": "rethrow"
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def return_(session, value=None):
    '''
    Return from the outermost structured block

    Return from the outermost structured block. If the expression of a lambda
    function is a block function, this will effectively return from the
    lambda function. If called outside of a structured block, an error is
    thrown.

    Parameters:

        value (): The value to evaluate that the outermost block will return.
        If not specified, the last evaluated value or a null value will be
        returned.

    Returns:
    None: This function returns from the outermost structured block with the last evaluated value.
    '''

    request = session.Request()

    action = {
        "function": "return"
    }

    if value != None:
        action['value'] = value

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def switch(session, predicate, value1, case_clause):
    '''
    All combinations true

    Support for switch statement.

    Parameters:

        predicate (function): The predicate is passed two parameters and must
        return a boolean. The first parameter passed is the evaluated value
        of the value1 parameter and the second is the value2 from a case
        clause. This predicate will often be 'eqx' to use the exactly equal
        function but can also be any other function such as 'regexp_match' or
        a lambda function.

        value1 (): The first parameter passed to the predicate.

        case_clause (): This is one or more case clauses which are pairs of a
        value2 parameter followed by a statement list or undefined parameter.
        One value2 can be undefined to indicate the default case clause.
        
        For the first value2 that is undefined or calling the predicate
        returns true, the statement list followed by any statement lists of
        subsequent case clauses are executed until a break or return is
        encountered. The predicate is called with value1 and the case
        clause's value2.

    Returns:
    None: 
    '''

    request = session.Request()

    action = {
        "function": "switch",
        "predicate": predicate,
        "value1": value1,
        "case_clause": case_clause
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def throw(session, message, additional=None):
    '''
    Throws an error

    This throws an error that can be caught by a try/catch block. An error
    object of object type _AdaptiveError_ will be available in the catch
    block. Its 'id' property will be set to 'throw'. The other properties set
    based on the parameters specified and where this function is called.

    Parameters:

        message (string): This is the message that will be included in the
        _AdaptiveError_ error object available in the catch block.

        additional (): Optional additional information that will be available
        as a 'additional' property in the error object.

    Returns:
    void: 
    '''

    request = session.Request()

    action = {
        "function": "throw",
        "message": message
    }

    if additional != None:
        action['additional'] = additional

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def try_(session, body, _finally=None, catch=None, error=None):
    '''
    Evaluate an array of values (statements) as a try block with optional catch and finally statements

    This creates a new structured block with a new nested variable scope.
    
    This function will evaluate the body statements. If an error is thrown
    and there is an optional catch, the error will be 'caught' and the
    associated statements will be evaluated. The optional finally statements
    are always evaluated after the body and catch statements. See the related
    functions 'break', 'continue', 'return' and 'throw'.

    Parameters:

        body (array): This is an array of values (statements) that are
        evaluated. Each value in body is evaluated in order until the end of
        the list or until a 'break', 'continue', 'return' or 'throw' function
        is encountered.

        finally (array): This is an array of values (statements) that are
        evaluated after the try and catch statements even if an error occurs.
        Each value in body is evaluated in order until the end of the list or
        until a 'break', 'continue', 'return' or 'throw' function is
        encountered.

        catch (array): This is an array of values (statements) that are
        evaluated when an error is thrown while evaluating the body. Each
        value in body is evaluated in order until the end of the list or
        until a 'break', 'continue', 'return' or 'throw' function is
        encountered.

        error (object): The error object thrown. This is only available in
        the catch block. See adaptive object type _AdaptiveObjectType_ for
        details.

    Returns:
    None: The last value evaluated in body.
    '''

    request = session.Request()

    action = {
        "function": "try",
        "body": body
    }

    if _finally != None:
        action['finally'] = _finally

    if catch != None:
        action['catch'] = catch

    if error != None:
        action['error'] = error

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def while_(session, condition, body):
    '''
    Evaluate an array of values (statements) while a condition is true

    This creates a new structured block with a new nested variable scope.
    
    This function will evaluate an array of values (statements) while a
    condition is true. The condition is tested at the beginning of the loop.
    If the condition is false for the first iteration, the loop returns a
    null value. See the related functions 'break', 'continue', 'return' and
    'throw'.

    Parameters:

        condition (boolean): While this condition is true, the loop will
        continue. This is evaluated in the loop's scope.

        body (array): This is an array of values (statements) that are
        evaluated for each iteration of the loop. Each value in body is
        evaluated in order until the end of the list or until a 'break',
        'continue', 'return' or 'throw' function is encountered.

    Returns:
    None: The last value evaluated in body or null if condition evaluates to false the first time.
    '''

    request = session.Request()

    action = {
        "function": "while",
        "condition": condition,
        "body": body
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']


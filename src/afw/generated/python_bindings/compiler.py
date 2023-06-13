# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def assert_(session, assertion, reason=None):
    '''
    Assert a value

    Assert that a value is true. If not, an assertion_failed error is thrown.

    Parameters:

        assertion (boolean): This is the assertion to make. If not true, an
        assertion_failed error is thrown.

        reason (string): This is an optional reason to include in the
        assertion_failed message.

    Returns:
    null: 
    '''

    request = session.Request()

    action = {
        "function": "assert",
        "assertion": assertion
    }

    if reason != None:
        action['reason'] = reason

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def compile_expression_tuple(session, expression_tuple, listing=None):
    '''
    Compile expression tuple value

    Compile a string containing adaptive expression tuple syntax and return
    either an unevaluated expression tuple adaptive value or a string
    containing the compiler listing.

    Parameters:

        expression_tuple (list): expression tuple to compile.

        listing (): If specified, a compiler listing is produced instead of
        an unevaluated expression tuple value.
        
        This parameter can be an integer between 0 and 10 of a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    None: An unevaluated expression tuple value ready for use by function evaluate() or a string containing the compiler listing.
    '''

    request = session.Request()

    action = {
        "function": "compile_expression_tuple",
        "expression_tuple": expression_tuple
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def compile_json(session, json, listing=None):
    '''
    Compile JSON

    Compile a string containing adaptive JSON syntax and return either an
    unevaluated JSON adaptive value or a string containing the compiler
    listing.

    Parameters:

        json (string): JSON string to compile.

        listing (): If specified, a compiler listing is produced instead of
        an unevaluated JSON value.
        
        This parameter can be an integer between 0 and 10 of a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    None: An unevaluated JSON value ready for use by function evaluate() or a string containing the compiler listing.
    '''

    request = session.Request()

    action = {
        "function": "compile_json",
        "json": json
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def compile_relaxed_json(session, json, listing=None):
    '''
    Compile relaxed JSON

    Compile an adaptive relaxed JSON syntax string and return an adaptive
    value.

    Parameters:

        json (string): Adaptive relaxed JSON syntax string to compile.

        listing (): If specified, a compiler listing is produced instead of
        an unevaluated relaxed JSON value.
        
        This parameter can be an integer between 0 and 10 of a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    None: An unevaluated relaxed JSON value ready for use by function evaluate() or a string containing the compiler listing.
    '''

    request = session.Request()

    action = {
        "function": "compile_relaxed_json",
        "json": json
    }

    if listing != None:
        action['listing'] = listing

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def convert_syntax_hybrid_to_expression(session, hybrid, whitespace=None):
    '''
    Convert a hybrid to a expression.

    Convert a string containing adaptive hybrid syntax, which can be an
    adaptive template or adaptive expression tuple, to adaptive expression
    syntax.

    Parameters:

        hybrid (string): The hybrid to convert.

        whitespace (): Add whitespace for readability if present and not 0.
        This parameter can be an integer between 0 and 10 or a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    expression: The converted value in adaptive expression syntax.
    '''

    request = session.Request()

    action = {
        "function": "convert_syntax_hybrid_to_expression",
        "hybrid": hybrid
    }

    if whitespace != None:
        action['whitespace'] = whitespace

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def decompile(session, value, whitespace=None):
    '''
    Decompile value

    Decompile an adaptive value to string.

    Parameters:

        value (): Value to decompile.

        whitespace (): Add whitespace for readability if present and not 0.
        This parameter can be an integer between 0 and 10 or a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    string: Decompiled value.
    '''

    request = session.Request()

    action = {
        "function": "decompile",
        "value": value
    }

    if whitespace != None:
        action['whitespace'] = whitespace

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def evaluate_expression_tuple(session, expression_tuple, additionalUntrustedQualifiedVariables=None):
    '''
    Evaluate expression tuple

    Compile a string containing adaptive expression tuple syntax and then
    evaluate the result.

    Parameters:

        expression_tuple (string): Expression tuple to compile and evaluate.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    None: Evaluated adaptive expression tuple.
    '''

    request = session.Request()

    action = {
        "function": "evaluate_expression_tuple",
        "expression_tuple": expression_tuple
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def evaluate_value(session, value, additionalUntrustedQualifiedVariables=None):
    '''
    Evaluate value

    Evaluate an adaptive value.

    Parameters:

        value ():

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    None: Evaluated adaptive value.
    '''

    request = session.Request()

    action = {
        "function": "evaluate_value",
        "value": value
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def evaluate_with_retry(session, value, limit):
    '''
    Evaluate value with retry

    Evaluate a value and retry up to a limit if an exception occurs.

    Parameters:

        value (): Value to evaluated

        limit (integer): Maximum number to retry if an exception occurs.

    Returns:
    None: Evaluated value.
    '''

    request = session.Request()

    action = {
        "function": "evaluate_with_retry",
        "value": value,
        "limit": limit
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def qualifier(session, qualifier, forTesting=None):
    '''
    Access variables of a qualifier as an object

    This function allows the active variables for a qualifier to be accessed
    as the properties of an object.

    Parameters:

        qualifier (string): This is the qualifier whose variables are to be
        accessed as properties of the returned object.

        forTesting (boolean): If specified and true, the object returned will
        be suitable to pass as the additionalUntrustedQualifiedVariables
        parameter of evaluate*() functions. This is intended for testing
        purposes and should not be used in production.

    Returns:
    object: Each property is the name of a variable with the value influenced by the forTesting property.
    '''

    request = session.Request()

    action = {
        "function": "qualifier",
        "qualifier": qualifier
    }

    if forTesting != None:
        action['forTesting'] = forTesting

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def qualifiers(session, forTesting=None):
    '''
    Access qualifiers as an object

    This function allows the active qualifiers to be accessed as properties
    of an object. The value of each of these properties is an object whose
    properties are the variables for the corresponding qualifier.

    Parameters:

        forTesting (boolean): If specified and true, the object returned will
        be suitable to pass as the additionalUntrustedQualifiedVariables
        parameter of evaluate*() functions. This is intended for testing
        purposes and should not be used in production.

    Returns:
    object: Each property is the name of a qualifier with a value that is an object whose properties are the variables of that qualifier. The value of the variable properties is influenced by the forTesting property.
    '''

    request = session.Request()

    action = {
        "function": "qualifiers"
    }

    if forTesting != None:
        action['forTesting'] = forTesting

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def safe_evaluate(session, value, error):
    '''
    Safe evaluate

    Return the evaluated adaptive value. If an exception occurs, return
    evaluated error instead.

    Parameters:

        value (): Value to evaluated

        error (): Value to evaluate and return if exception occurs. If an
        error occurs evaluating this value, the exception will continue.

    Returns:
    None: Evaluated adaptive value or error value.
    '''

    request = session.Request()

    action = {
        "function": "safe_evaluate",
        "value": value,
        "error": error
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def stringify(session, value, replacer=None, whitespace=None):
    '''
    Evaluate and decompile a value

    Evaluate and decompile an adaptive value to string. For most values this
    has the effect of producing a string containing json.

    Parameters:

        value (): Value to stringify.

        replacer (): Optional replacer function.

        whitespace (): Add whitespace for readability if present and not 0.
        This parameter can be an integer between 0 and 10 or a string that is
        used for indentation. If 0 is specified, no whitespace is added to
        the resulting string. If 1 through 10 is specified, that number of
        spaces is used.

    Returns:
    string: Evaluated and decompiled value.
    '''

    request = session.Request()

    action = {
        "function": "stringify",
        "value": value
    }

    if replacer != None:
        action['replacer'] = replacer

    if whitespace != None:
        action['whitespace'] = whitespace

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_expression(session, id, description, expression, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test expression

    Compile and evaluate an adaptive expression and compare the results to an
    expected value. Return object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        expression (string): Expression to compile and evaluate.

        expected (): Expected result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_expression",
        "id": id,
        "description": description,
        "expression": expression,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_expression_tuple(session, id, description, expression, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test expression tuple

    Compile and evaluate an adaptive expression tuple and compare the results
    to an expected value. Return object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        expression (string): Expression tuple to compile and evaluate.

        expected (): Expected result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_expression_tuple",
        "id": id,
        "description": description,
        "expression": expression,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_hybrid(session, id, description, hybrid, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test hybrid

    Compile and evaluate a string containing adaptive hybrid syntax which can
    be an adaptive template or adaptive expression tuple and then compare the
    results to an expected value. Return object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        hybrid (string): Hybrid to compile and evaluate.

        expected (): Expected evaluated result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_hybrid",
        "id": id,
        "description": description,
        "hybrid": hybrid,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_script(session, id, description, script, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test an adaptive script

    Compile and evaluate an adaptive script and compare the results to an
    expected value. Return object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        script (string): Script to compile and evaluate.

        expected (): Expected result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_script",
        "id": id,
        "description": description,
        "script": script,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_script_runtime_support(session, testScriptObject):
    '''
    Internal test script runtime support

    This is a function called internally as the result of a test_script
    compile. This function is not intended to be called directly.

    Parameters:

        testScriptObject (object): A test script results object with the
        required evaluation result properties missing. The sources will be
        evaluated and the corresponding test result properties will be set.

    Returns:
    object: The testScriptObject object with test result properties set.
    '''

    request = session.Request()

    action = {
        "function": "test_script_runtime_support",
        "testScriptObject": testScriptObject
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_template(session, id, description, template, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test template

    Compile and evaluate an adaptive template and compare the results to an
    expected value. Return object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        template (string): Template to compile and evaluate.

        expected (): Expected evaluated result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_template",
        "id": id,
        "description": description,
        "template": template,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def test_value(session, id, description, value, expected, additionalUntrustedQualifiedVariables=None):
    '''
    Test value

    Evaluate an adaptive value and compare it to an expected value. Return
    object with the test's results.

    Parameters:

        id (string): Id of test

        description (string): Description of test

        value (string): Value to evaluate

        expected (): Expected result.

        additionalUntrustedQualifiedVariables (object): This parameter
        supplies additional qualified variables that can be accessed during
        evaluation. These variables will not be used by anything that needs
        to ensure its qualified variables must come from a trusted source,
        such as authorization. This parameter is intended to be used for
        testing only and should not be used for anything running in
        production.

    Returns:
    object: Test results.
    '''

    request = session.Request()

    action = {
        "function": "test_value",
        "id": id,
        "description": description,
        "value": value,
        "expected": expected
    }

    if additionalUntrustedQualifiedVariables != None:
        action['additionalUntrustedQualifiedVariables'] = additionalUntrustedQualifiedVariables

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']
